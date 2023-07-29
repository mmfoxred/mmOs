#ifndef _IDE_H
#define _IDE_H

#include "types.h"

/* Hd controller regs. Ref: IBM AT Bios-listing */
#define HD_DATA		0x1f0	/* _CTL when writing */
#define HD_ERROR	0x1f1	/* see err-bits */
#define HD_NSECTOR	0x1f2	/* nr of sectors to read/write */
#define HD_SECTOR	0x1f3	/* starting sector */
#define HD_LCYL		0x1f4	/* starting cylinder */
#define HD_HCYL		0x1f5	/* high byte of starting cyl */
#define HD_CURRENT	0x1f6	/* 101dhhhh , d=drive, hhhh=head */
#define HD_STATUS	0x1f7	/* see status-bits */
#define HD_PRECOMP HD_ERROR	/* same io address, read=error, write=precomp */
#define HD_COMMAND HD_STATUS	/* same io address, read=status, write=cmd */

#define HD_CMD		0x3f6

/* Bits of HD_STATUS */
#define ERR_STAT	0x01    // 1表示发生了错误,错误代码已放置在错误寄存器中
#define INDEX_STAT	0x02    // 1表示控制器检测到索引标记(啥意思?)
#define ECC_STAT	0x04	// 1 表示控制器必须通过使用 ECC 字节来纠正数据（纠错码：扇区末尾的额外字节，允许验证其完整性，有时还可以纠正错误）
#define DRQ_STAT	0x08    // 1 表示控制器正在等待数据（用于写入）或正在发送数据（用于读取）。该位为 0 时不要访问数据寄存器。
#define SEEK_STAT	0x10    // 1 表示读/写磁头就位（搜索完成）
#define WRERR_STAT	0x20    // 1 表示控制器检测到写入故障
#define READY_STAT	0x40    // 1 表示控制器已准备好接受命令，并且驱动器以正确的速度旋转
#define BUSY_STAT	0x80    // 1 表示控制器正忙于执行命令。设置该位时，不应访问任何寄存器（数字输出寄存器除外）

/* Values for HD_COMMAND */
#define WIN_RESTORE		0x10
#define WIN_READ		0x20
#define WIN_WRITE		0x30
#define WIN_VERIFY		0x40
#define WIN_FORMAT		0x50
#define WIN_INIT		0x60
#define WIN_SEEK 		0x70
#define WIN_DIAGNOSE	0x90
#define WIN_SPECIFY		0x91

/* Bits for HD_ERROR */
#define MARK_ERR	0x01	/* Bad address mark ? */
#define TRK0_ERR	0x02	/* couldn't find track 0 */
#define ABRT_ERR	0x04	/* ? */
#define ID_ERR		0x10	/* ? */
#define ECC_ERR		0x40	/* ? */
#define	BBD_ERR		0x80	/* ? */

/*
- cld指令是将方向标志位清零，确保数据是按照从低地址到高地址的顺序写入缓冲区。
- rep指令是重复执行后面的指令，根据nr参数指定的次数进行循环。
- insw指令是从端口中读取一个字（16位数据）到缓冲区中，并将端口号和缓冲区地址分别传递给寄存器d和寄存器D。
因此，这段代码的含义是通过汇编指令从指定端口读取nr个字（16位数据）到缓冲区buf中。
 * */

#define port_read(port, buf, nr) \
__asm__("cld;rep;insw"::"d" (port),"D" (buf),"c" (nr))

#define port_write(port, buf, nr) \
__asm__("cld;rep;outsw"::"d" (port),"S" (buf),"c" (nr))

typedef void (*dev_handler_fun_t)(void);

typedef struct _hd_partition_t {

} __attribute__((packed)) hd_partition_t;

typedef struct _hd_channel_t hd_channel_t;

typedef struct _hd_t {
    u8              dev_no;
    u8              is_master;      // 是否是主设备 1是 0否
    hd_partition_t  partition[4];   // 暂不考虑逻辑分区
    hd_channel_t*   channel;

    // 数据来源：硬盘identify命令返回的结果
    char number[10 * 2 + 1];    // 硬盘序列号    最后一个字节是补字符串结束符0用的,从硬盘读取的是没有结束符的
    char model[20 * 2 + 1];     // 硬盘型号
    int sectors;                // 扇区数 一个扇区512字节
} __attribute__((packed)) hd_t;

void hd_init();

void do_hd_request();

hd_t* get_hd_info(u8 dev);

void print_disk_info(hd_t* info);

#endif //_IDE_H
