#ifndef _TTY_H
#define _TTY_H

#include "types.h"

void console_init(void);
int console_write(char *buf, u32 count);

#endif //_TTY_H
