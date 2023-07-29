#ifndef _SHELL_H
#define _SHELL_H

#include "linux/types.h"

bool shell_is_active();
void active_shell();
void close_shell();

void run_shell(char ch);
void exec_shell();
void del_shell();

#endif //_SHELL_H
