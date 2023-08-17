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

void print_transfer_message(size_t file_size)
{
   if (file_size > 226547080)
   {
      clear(); 
      print_center("TRANSFERING...", 0, 14);
      print_center("(this may take some time)", 1, 25);
   }

   else
   {
      clear(); 
      print_center("TRANSFERING...", 0, 14);
   }
}

#endif
