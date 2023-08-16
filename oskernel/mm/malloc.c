#include "../include/linux/mm.h"
#include "../include/asm/system.h"

struct bucket_desc {	/* 16 bytes */
    void			    *page;          // 管理的物理页
    struct bucket_desc	*next;          // 下一个bucket地址
    void			    *freeptr;       // 下一个可供分配的
    unsigned short		refcnt;         // 引用计数，释放物理页时要用，应该是按page使用量记录的吧
    unsigned short		bucket_size;    // 每个桶的大小
};

struct _bucket_dir {	/* 8 bytes */
    int			size;
    struct bucket_desc	*chain;
};

struct _bucket_dir bucket_dir[] = {
        { 16,	(struct bucket_desc *) 0},
        { 32,	(struct bucket_desc *) 0},
        { 64,	(struct bucket_desc *) 0},
        { 128,	(struct bucket_desc *) 0},
        { 256,	(struct bucket_desc *) 0},
        { 512,	(struct bucket_desc *) 0},
        { 1024,	(struct bucket_desc *) 0},
        { 2048, (struct bucket_desc *) 0},
        { 4096, (struct bucket_desc *) 0},
        { 0,    (struct bucket_desc *) 0}
};   /* End of list marker */

/*
 最终指向空闲链表的头节点
 */
struct bucket_desc *free_bucket_desc = (struct bucket_desc *) 0;

/*
 * This routine initializes a bucket description page.
 */
static inline void init_bucket_desc()
{
    struct bucket_desc *bdesc, *first;
    int	i;

    first = bdesc = (struct bucket_desc *) get_free_page(); //这里分配物理内存，装页框们
    if (!bdesc)
        return NULL;
    for (i = PAGE_SIZE/sizeof(struct bucket_desc); i > 1; i--) {
        bdesc->next = bdesc+1;
        bdesc++;
    }
    /*
     * This is done last, to avoid race conditions in case
     * get_free_page() sleeps and this routine gets called again....
     */
    bdesc->next = free_bucket_desc;
    free_bucket_desc = first; 
}

void* kmalloc(size_t len) {
    struct _bucket_dir	*bdir;
    struct bucket_desc	*bdesc;
    void			*retval;

    /*
     * First we search the bucket_dir to find the right bucket change
     * for this request.
     */
    for (bdir = bucket_dir; bdir->size; bdir++)
        if (bdir->size >= len)
            break;
    if (!bdir->size) {
        printk("malloc called with impossibly large argument (%d)\n",
               len);
        return NULL;
    }
    /*
     * Now we search for a bucket descriptor which has free space
     */
    CLI	/* Avoid race conditions */
    for (bdesc = bdir->chain; bdesc; bdesc = bdesc->next)
        if (bdesc->freeptr)
            break;
    /*
     * If we didn't find a bucket with free space, then we'll
     * allocate a new one.
     */
    if (!bdesc) {
        char		*cp;
        int		i;

        if (!free_bucket_desc)
            init_bucket_desc();
        bdesc = free_bucket_desc;
        free_bucket_desc = bdesc->next;
        bdesc->refcnt = 0;
        bdesc->bucket_size = bdir->size;
        bdesc->page = bdesc->freeptr = (void *) (cp = (char *) get_free_page());
        if (!cp)
            return NULL;
        /* Set up the chain of free objects,cut the 4KN to  bdir->size*/
        for (i=PAGE_SIZE/bdir->size; i > 1; i--) {
            *((char **) cp) = cp + bdir->size; //cp是指向page的指针,这里的cp是当一个二维数组来用的
            cp += bdir->size;
        }
        *((char **) cp) = 0;
        bdesc->next = bdir->chain; /* OK, link it in! */
        bdir->chain = bdesc;
    }
    retval = (void *) bdesc->freeptr;
    bdesc->freeptr = *((void **) retval); // 指向下一个空闲块,这里的retval是当一个二维数组来用的
    bdesc->refcnt++; //这个应该是按page使用量记录的吧
    STI	/* OK, we're safe again */
    return(retval);
}

void kfree_s(void *obj, int size)
{
    void		*page;
    struct _bucket_dir	*bdir;
    struct bucket_desc	*bdesc, *prev;
    bdesc = prev = 0;
    /* Calculate what page this object lives in */
    page = (void *)  ((unsigned long) obj & 0xfffff000);
    /* Now search the buckets looking for that page */
    for (bdir = bucket_dir; bdir->size; bdir++) {
        prev = 0;
        /* If size is zero then this conditional is always false */
        if (bdir->size < size)
            continue;
        for (bdesc = bdir->chain; bdesc; bdesc = bdesc->next) {
            if (bdesc->page == page)
                goto found;
            prev = bdesc;
        }
    }
    return;
found:
    CLI
    *((void **)obj) = bdesc->freeptr;
    bdesc->freeptr = obj;
    bdesc->refcnt--;
    if (bdesc->refcnt == 0) {
        /*
         * We need to make sure that prev is still accurate.  It
         * may not be, if someone rudely interrupted us....
	     * 这里是怕prev因为中断已经不是原来的值了，所以需要重新判断一次
         */
        if ((prev && (prev->next != bdesc)) ||
            (!prev && (bdir->chain != bdesc)))
            for (prev = bdir->chain; prev; prev = prev->next)
                if (prev->next == bdesc)
                    break;
        if (prev)
            prev->next = bdesc->next;
        else {
            if (bdir->chain != bdesc) //必不可能情况，所以退出
                return;
            bdir->chain = bdesc->next;
        }
        free_page((unsigned long) bdesc->page);
        bdesc->next = free_bucket_desc;
        free_bucket_desc = bdesc;
    }
    STI
    return;
}