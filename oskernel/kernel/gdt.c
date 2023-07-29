#include "../include/asm/system.h"
#include "../include/linux/kernel.h"
#include "../include/linux/traps.h"
#include "../include/linux/task.h"
#include "../include/string.h"

#define GDT_SIZE    256

u64 gdt[GDT_SIZE] = {0};

xdt_ptr_t gdt_ptr;

int r0_code_selector = 1 << 3;
int r0_data_selector = 2 << 3;
int r3_code_selector;
int r3_data_selector;
int tss_selector;

tss_t tss;

static void r3_gdt_code_item(int gdt_index, int base, int limit) {
    // 在实模式时已经构建了4个全局描述符，所以从4开始
    if (gdt_index < 4) {
        printk("the gdt_index:%d has been used...\n", gdt_index);
        return;
    }

    gdt_item_t* item = &gdt[gdt_index];

    item->limit_low = limit & 0xffff;
    item->base_low = base & 0xffffff;
    item->type = 0b1000;
    item->segment = 1;
    item->DPL = 0b11;
    item->present = 1;
    item->limit_high = limit >> 16 & 0xf;
    item->available = 0;
    item->long_mode = 0;
    item->big = 1;
    item->granularity = 1;
    item->base_high = base >> 24 & 0xf;
}

static void r3_gdt_data_item(int gdt_index, int base, int limit) {
    // 在实模式时已经构建了4个全局描述符，所以从4开始
    if (gdt_index < 4) {
        printk("the gdt_index:%d has been used...\n", gdt_index);
        return;
    }

    gdt_item_t* item = &gdt[gdt_index];

    item->limit_low = limit & 0xffff;
    item->base_low = base & 0xffffff;
    item->type = 0b0010;
    item->segment = 1;
    item->DPL = 0b11;
    item->present = 1;
    item->limit_high = limit >> 16 & 0xf;
    item->available = 0;
    item->long_mode = 0;
    item->big = 1;
    item->granularity = 1;
    item->base_high = base >> 24 & 0xf;
}

void init_tss_item(int gdt_index, int base, int limit) {
    printk("init tss...\n");

    tss.ss0 = r0_data_selector;
    tss.esp0 = kmalloc(4096) + PAGE_SIZE;
    tss.iobase = sizeof(tss);

    gdt_item_t* item = &gdt[gdt_index];

    item->base_low = base & 0xffffff;
    item->base_high = (base >> 24) & 0xff;
    item->limit_low = limit & 0xffff;
    item->limit_high = (limit >> 16) & 0xf;
    item->segment = 0;     // 系统段
    item->granularity = 0; // 字节
    item->big = 0;         // 固定为 0
    item->long_mode = 0;   // 固定为 0
    item->present = 1;     // 在内存中
    item->DPL = 0;         // 用于任务门或调用门
    item->type = 0b1001;   // 32 位可用 tss

    asm volatile("ltr ax;"::"a"(tss_selector));
}

void gdt_init() {
    printk("init gdt...\n");

    __asm__ volatile ("sgdt gdt_ptr;");

    memcpy(&gdt, gdt_ptr.base, gdt_ptr.limit);

    // 创建r3用的段描述符：代码段、数据段
    r3_gdt_code_item(4, 0, 0xfffff);
    r3_gdt_data_item(5, 0, 0xfffff);

    // 创建r3用的选择子：代码段、数据段
    r3_code_selector = 4 << 3 | 0b011;
    r3_data_selector = 5 << 3 | 0b011;
    tss_selector = 6 << 3;

    gdt_ptr.base = &gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    __asm__ volatile ("lgdt gdt_ptr;");

    init_tss_item(6, &tss, sizeof(tss_t) - 1);
}