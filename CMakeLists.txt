cmake_minimum_required(VERSION 3.22)
project(ziya-os-research C)

set(CMAKE_C_STANDARD 99)

add_executable(ziya-os-research main.c tools/floppy.c tools/fileinfo.c)
