#include "../include/linux/kernel.h"
#include "../include/linux/hd.h"
#include "../include/linux/fs.h"
#include "../include/linux/task.h"
#include "../include/shell.h"
#include "./blk_drv/fat32/xdisk.h"
#include "./blk_drv/fat32/xfat.h"
#include "../include/stdio.h"

extern task_t* current;
extern xdisk_driver_t vdisk_driver;
xdisk_t disk;
xdisk_part_t disk_part;



task_t* wait_for_request = NULL;

int disk_part_test() {
    u32_t count, i;
    xfat_err_t err = FS_ERR_OK;

    printf("partition read test...\n");

    err = xdisk_get_part_count(&disk, &count);
    if (err < 0) {
        printf("partion count detect failed!\n");
        return err;
    }
    printf("partition count:%d\n", count);

    for (i = 0; i < count; i++) {
        xdisk_part_t part;
        int err;

        err = xdisk_get_part(&disk, &part, i);
        if (err == -1) {
            printf("read partion in failed:%d\n", i);
            return -1;
        }

        printf("no %d: start: %d, count: %d, capacity:%.0f M\n",
               i, part.start_sector, part.total_sector,
               part.total_sector * disk.sector_size / 1024 / 1024.0);
    }
    return 0;
}



void kernel_thread_fun(void* arg) {
    hd_init();

    char str[512] = "mmos";
    bwrite(1, 0, str, 512);
    disk_part_test();
    buffer_head_t* buff1 = bread(1, 0, 1);
    printk("%s\n", buff1->data);

    active_shell();

    while (true);
}