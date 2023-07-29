#include "../include/asm/system.h"
#include "../include/linux/kernel.h"
#include "../include/linux/mm.h"
#include "../include/linux/task.h"
#include "../include/string.h"

/**
 *  一个pdt 4k 0x1000
 *  4g内存需要这么大页表来完整映射内存：0x1000 * 0x1000 + 0x1000
 */

// 页表从0x20000开始存
#define PDT_START_ADDR 0x20000

// 线性地址从2M开始用
#define VIRTUAL_MEM_START 0x200000

extern task_t* current;

void* virtual_memory_init() {
    int* pdt = (int*)PDT_START_ADDR;

    // 清零
    memset(pdt, 0, PAGE_SIZE);

    for (int i = 0; i < 4; ++i) {
        // pdt里面的每项，即pde，内容是ptt + 尾12位的权限位
        int ptt = (int)PDT_START_ADDR + ((i + 1) * 0x1000);
        int pde = 0b00000000000000000000000000000111 | ptt;

        pdt[i] = pde;

        int* ptt_arr = (int*)ptt;

        if (0 == i) {
            // 第一块映射区，给内核用
            for (int j = 0; j < 0x400; ++j) {
                int* item = &ptt_arr[j];

                int virtual_addr = j * 0x1000;
                *item = 0b00000000000000000000000000000111 | virtual_addr;
            }
        } else {
//            for (int j = 0; j < 0x400; ++j) {
//                int* item = &ptt_arr[j];
                virtual_addr = virtual_addr + i * 0x400 * 0x1000;                *item = 0b00000000000000000000000000000111 | virtual_addr;
//            }
        }
    }

    BOCHS_DEBUG_MAGIC

    set_cr3((uint)pdt);

    enable_page();

    BOCHS_DEBUG_MAGIC

    printk("pdt addr: 0x%p\n", pdt);

    return pdt;
}



