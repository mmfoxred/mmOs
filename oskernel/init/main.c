#include "../include/linux/tty.h"
#include "../include/linux/traps.h"
#include "../include/linux/mm.h"
#include "../include/linux/task.h"
#include "../include/stdio.h"
#include "../include/unistd.h"

extern void clock_init();

void user_mode() {
    char* str = "welcome";
    printf("%s, %d\n", str, 11);

    pid_t pid = fork();
    if (pid > 0) {
        printf("pid=%d, ppid=%d\n", getpid(), getppid());
    } else if (0 == pid) {
        printf("pid=%d, ppid=%d\n", getpid(), getppid());

        for (int i = 0; i < 10; ++i) {
            printf("%d\n", i);
        }
//        while (true);
    }
}

void kernel_main(void) {
    console_init();
    clock_init();

    // 内存管理模块一定要放在主功能前面,因为主功能都要用到
    print_check_memory_info();
    memory_init();
    memory_map_int();

    gdt_init();
    idt_init();

    task_init();

    __asm__("sti;");

    while (true);
}