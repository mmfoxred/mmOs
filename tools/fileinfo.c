//
// Created by ziya on 2022/5/11.
//

#include "fileinfo.h"
#include "common.h"

Fileinfo* read_file(const char* filename) {
    if (NULL == filename) {
        return NULL;
    }

    // 1 创建对象
    Fileinfo* fileinfo = calloc(1, sizeof(Fileinfo));
    if (NULL == fileinfo) {
        perror("calloc fail: ");
        exit(-1);
    }

    fileinfo->name = filename;

    // 2 打开文件
    FILE* file = NULL;
    if (NULL == (file = fopen(filename, "rb"))) {
        perror("fopen fail");
        exit(1);
    }

    // 3 获取文件大小
    if (0 != fseek(file, 0, SEEK_END)) {
        perror("fseek fail");
        exit(1);
    }

    fileinfo->size = (int)ftell(file);
    if (-1 == fileinfo->size) {
        perror("ftell fail");
        exit(1);
    }

    // 文件指针还原
    fseek(file, 0, SEEK_SET);

    // 4 申请内存
    fileinfo->content = calloc(1, fileinfo->size);
    if (NULL == fileinfo->content) {
        perror("calloc fail: ");
        exit(-1);
    }

    // 5 文件读入
    int readsize = fread(fileinfo->content, sizeof(char), fileinfo->size, file);
    if (readsize != fileinfo->size) {
        perror("fread fail: ");
        exit(-1);
    }

    // 6 关闭文件
    fclose(file);

    return fileinfo;
}





