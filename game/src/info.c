#include "info.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *buf[INFO_WIN_HEIGHT];
static char **ptr = &buf[0];

void
addInfo(const char *format, ...)
{
    if (ptr == &buf[INFO_WIN_HEIGHT]) {
        for (int i = 1; i < INFO_WIN_HEIGHT; i++) {
            strcpy(buf[i-1], buf[i]);
        }
        ptr--;
    }
    *ptr = (char *) malloc(INFO_WIN_WIDTH * 2 * sizeof(char));

    va_list local_argv;
    va_start(local_argv, format);
    vsprintf(*ptr, format, local_argv);
    va_end(local_argv);

    ptr++;

    wclear(infoWin);
    for (char **beg = &buf[0]; beg < ptr; beg++)
        waddstr(infoWin, *beg);
    wrefresh(infoWin);
}