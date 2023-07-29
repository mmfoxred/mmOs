#ifndef _SYS_H
#define _SYS_H

#include "types.h"

extern ssize_t sys_write(int fd, const void *buf, size_t count);
extern int sys_exit(int status);
extern int sys_fork();
extern pid_t sys_get_pid();
extern pid_t sys_get_ppid();

#endif //_SYS_H
