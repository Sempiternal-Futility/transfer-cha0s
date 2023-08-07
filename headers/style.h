// This header file contains functions that boost the sex appeal of the program 

#ifndef STYLE_H
#define STYLE_H

#include <ncurses.h>

void print_center(char *msg, int y, int x)
{
   move(LINES /2 +y, (COLS /2 -x/2));
   printw(msg);
   refresh();
}

#endif
