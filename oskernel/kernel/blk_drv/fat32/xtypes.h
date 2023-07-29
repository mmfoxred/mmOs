#ifndef XTYPES_H
#define XTYPES_H


typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef unsigned int xfile_size_t;
typedef long xfile_ssize_t;

typedef enum _xfat_err_t {
    FS_ERR_EOF = 1,
    FS_ERR_OK = 0,
    FS_ERR_IO = -1,
    FS_ERR_PARAM = -2,
    FS_ERR_NONE = -3,
    FS_ERR_FSTYPE = -4,
    FS_ERR_READONLY = -5,
    FS_ERR_DISK_FULL = -6,
    FS_ERR_EXISTED = -7,
    FS_ERR_NAME_USED = -8,
    FS_ERR_NOT_EMPTY = -9,
    FS_ERR_NOT_MOUNT = -10,
    FS_ERR_INVALID_FS = -11,
    FS_ERR_NO_BUFFER = -12
}xfat_err_t;

#endif

