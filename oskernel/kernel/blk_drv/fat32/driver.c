//#include <time.h>
#include "xdisk.h"
#include "xfat.h"
#include "../../../include/linux/fs.h"
#include "../../../include/linux/mm.h"
#include "../../../include/stdio.h"
#include "../../../include/string.h"


/**
 * 初始化磁盘设备
 * @param disk 初始化的设备
 * @param name 设备的名称
 * @return
 */
static xfat_err_t xdisk_hw_open(xdisk_t *disk, void * init_data) {
    disk->sector_size = 512;
    const int sector_cnt = 524288; //256M
    disk->total_sector = disk->sector_size * sector_cnt;
    return FS_ERR_OK;
}

/**
 * 关闭存储设备
 * @param disk
 * @return
 */
static xfat_err_t xdisk_hw_close(xdisk_t * disk) {
    return FS_ERR_OK;
}

/**
 * 从设备中读取指定扇区数量的数据
 * @param disk 读取的磁盘
 * @param buffer 读取数据存储的缓冲区
 * @param start_sector 读取的起始扇区
 * @param count 读取的扇区数量
 * @return
 */
static xfat_err_t xdisk_hw_read_sector(xdisk_t *disk, u8_t *buffer, u32_t start_sector, u32_t count) {
    buffer_head_t *bh = bread(0,start_sector,count);
    u32_t total_bytes = count * disk->sector_size;
    memcpy(buffer,bh->data,total_bytes);
    kfree_s(bh->data,total_bytes);
    kfree_s(bh, sizeof(bh));
    return FS_ERR_OK;
}

/**
 * 向设备中写指定的扇区数量的数据
 * @param disk 写入的存储设备
 * @param buffer 数据源缓冲区
 * @param start_sector 写入的起始扇区
 * @param count 写入的扇区数
 * @return
 */
static xfat_err_t xdisk_hw_write_sector(xdisk_t *disk, u8_t *buffer, u32_t start_sector, u32_t count) {
    bwrite(0,start_sector,buffer,count);
    return FS_ERR_OK;
}

/**
 * 获取当前时间
 * @param timeinfo 时间存储的数据区
 * @return
 */
 /*
static xfat_err_t xdisk_hw_curr_time(xdisk_t *disk, xfile_time_t *timeinfo) {
    time_t raw_time;
    struct tm * local_time;

    // 获取本地时间
    time(&raw_time);
    local_time = localtime(&raw_time);

    // 拷贝转换
    timeinfo->year = local_time->tm_year + 1900;
    timeinfo->month = local_time->tm_mon + 1;
    timeinfo->day = local_time->tm_mday;
    timeinfo->hour = local_time->tm_hour;
    timeinfo->minute = local_time->tm_min;
    timeinfo->second = local_time->tm_sec;

    return FS_ERR_OK;
}
 */

/**
 * 虚拟磁盘驱动结构
 */
xdisk_driver_t vdisk_driver = {
    .open = xdisk_hw_open,
    .close = xdisk_hw_close,
    .read_sector = xdisk_hw_read_sector,
    .write_sector = xdisk_hw_write_sector,
//    .curr_time = xdisk_hw_curr_time,
};