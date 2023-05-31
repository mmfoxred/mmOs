//
// Created by ziya on 2022/5/11.
//

#include "floppy.h"
#include "common.h"

Floppy* create_floppy() {
    Floppy* floppy = calloc(1, sizeof(Floppy));

    floppy->size = 2 * 80 * 18 * 512;
//    floppy->size = 18 * 512;
    floppy->content = calloc(1, floppy->size);

    return floppy;
}

void write_bootloader(Floppy* floppy, Fileinfo* fileinfo) {
    if (NULL == floppy || NULL == fileinfo) {
        ERROR_PRINT("NULL pointer\n");
        return;
    }

    memcpy(floppy->content, fileinfo->content, fileinfo->size);
}

void write_floppy(Floppy* floppy, char* str, int face, int track, int section) {
    if (NULL == floppy) {
        ERROR_PRINT("NULL pointer\n");
        return;
    }

    char* offset = floppy->content + (section - 1) * 512;

    memcpy(offset, str, strlen(str));
}

void write_floppy_fileinfo(Floppy* floppy, Fileinfo* fileinfo, int face, int track, int section) {
    if (NULL == floppy || NULL == fileinfo) {
        ERROR_PRINT("NULL pointer\n");
        return;
    }

    char* offset = 0;

//    if (fileinfo->size > 512) {
        printf("[%s] 文件大小超过 512: %d\n", fileinfo->name, fileinfo->size);
//    }

    offset = floppy->content + (section - 1) * 512;

    memcpy(offset, fileinfo->content, fileinfo->size);
}

void create_image(const char* name, Floppy* floppy) {
    if (NULL == name || NULL == floppy) {
        ERROR_PRINT("NULL pointer\n");
        return;
    }

    FILE* file = fopen(name, "w+");
    if (NULL == file) {
        perror("fopen fail");
        exit(-1);
    }

    fwrite(floppy->content, 1, floppy->size, file);
}




