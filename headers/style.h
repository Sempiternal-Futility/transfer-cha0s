// This header file contains functions that boost the sex appeal of the program 

#ifndef STYLE_H
#define STYLE_H

#include "config.h"
#include <stdlib.h>
#include <ncurses.h>

void init_colors()
{
   start_color();
   init_pair(1, COLOR_WHITE, COLOR_BLACK);
   init_pair(2, COLOR_BLACK, COLOR_WHITE);
   attron(COLOR_PAIR(1));
   bkgd(COLOR_PAIR(1));
}

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

void intro_manual()
{
   clear();
   print_center("PRESS ANY KEY TO READ THE MANUAL", 0, 32);
   getch();

   clear();
   printw("STEPS OF THE PROGRAM:\n\n");
   printw("1 STEP: ANSWER IF THE HOST IS A SERVER OR A CLIENT.\n");
   printw("THE SERVER IS THE ONE WHO SENDS, AND THE CLIENT IS THE ONE WHO RECEIVES.\n\n");
   printw("2 STEP: TYPE IN YOUR IP ADDRESS.\n");
   printw("IF YOU DON'T KNOW IT, OPEN A TERMINAL AND TYPE 'ip address'\n\n");
   printw("3 STEP: TYPE THE PATH OF THE FILE TO BE SENT.");
   getch();

   clear();
   printw("OPTIONS:\n\n");
   printw("FOR NOW ONLY 1 OPTION IS AVAILABLE:\n");
   printw("YOU CAN CHOOSE TO SAVE THE LAST GIVEN IP ADDRESS TO THIS MACHINE, INSTEAD OF TYPING.\n");
   printw("(disabled by default, since it's EXTREMELY buggy, recommend if only 2 hosts are going to be ever used)");
   getch();

   clear();
   printw("KNOWN BUGS:\n\n");
   printw("1 BUG: IF YOU TYPE A THE PATH TO A FILE THAT DOESN'T EXIST, A SEGFAULT TAKES PLACE.\n\n");
   printw("2 BUG: IF THE OPTION TO SAVE IP ADDRESES IS ENABLED, BOTH MACHINES MUST HAVE THE IP CONF FILE AT THE SAME STATE.\n");
   printw("(so if i delete that file on the server-side, and not delete it on the client-side, a segfault takes place)\n");
   printw("(if you mess that file up, just return all settings back to default)");
   getch();
}

int display_start_menu() // Displays the start menu when the program is opened
{
   if (conf_enable_startmenu == true)
   {
      clear();

      move(0, COLS /2 -33); printw("████████╗██████╗  █████╗ ███╗   ██╗███████╗███████╗███████╗██████╗\n");
      move(1, COLS /2 -33); printw("╚══██╔══╝██╔══██╗██╔══██╗████╗  ██║██╔════╝██╔════╝██╔════╝██╔══██╗\n");
      move(2, COLS /2 -33); printw("   ██║   ██████╔╝███████║██╔██╗ ██║███████╗█████╗  █████╗  ██████╔╝\n");
      move(3, COLS /2 -33); printw("   ██║   ██╔══██╗██╔══██║██║╚██╗██║╚════██║██╔══╝  ██╔══╝  ██╔══██╗\n");
      move(4, COLS /2 -33); printw("   ██║   ██║  ██║██║  ██║██║ ╚████║███████║██║     ███████╗██║  ██║\n");
      move(5, COLS /2 -33); printw("   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝\n");
      move(6, COLS /2 -33); printw("             ██████╗██╗  ██╗ █████╗  ██████╗ ███████╗              \n");
      move(7, COLS /2 -33); printw("            ██╔════╝██║  ██║██╔══██╗██╔═████╗██╔════╝              \n");
      move(8, COLS /2 -33); printw("            ██║     ███████║███████║██║██╔██║███████╗              \n");
      move(9, COLS /2 -33); printw("            ██║     ██╔══██║██╔══██║████╔╝██║╚════██║              \n");
      move(10, COLS /2 -33); printw("            ╚██████╗██║  ██║██║  ██║╚██████╔╝███████║              \n");
      move(11, COLS /2 -33); printw("             ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝              \n");
      
      print_center("TRANSFER!", +2, 9);
      print_center("OPTIONS", +4, 7);
      print_center("READ MANUAL", +6, 11);
      print_center("QUIT", +8, 4);

      attron(COLOR_PAIR(2));
      print_center("TRANSFER!", +2, 9);

      short cur_pos = 0; // this is 0 if cursor is on top of "TRANSFER!", this is 1 if on top of "OPTINS", and so on
      int input = 0;
      keypad(stdscr, true);
      while (input != '\n')
      {
         input = getch();

         if (input == 'j' && cur_pos == 0) {
            attron(COLOR_PAIR(1));
            print_center("TRANSFER!", +2, 9);

            attron(COLOR_PAIR(2));
            print_center("OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == KEY_DOWN && cur_pos == 0) {
            attron(COLOR_PAIR(1));
            print_center("TRANSFER!", +2, 9);

            attron(COLOR_PAIR(2));
            print_center("OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == 'k' && cur_pos == 1) {
            attron(COLOR_PAIR(1));
            print_center("OPTIONS", +4, 7);

            attron(COLOR_PAIR(2));
            print_center("TRANSFER!", +2, 9);
            cur_pos = 0;
         }

         else if (input == KEY_UP && cur_pos == 1) {
            attron(COLOR_PAIR(1));
            print_center("OPTIONS", +4, 7);

            attron(COLOR_PAIR(2));
            print_center("TRANSFER!", +2, 9);
            cur_pos = 0;
         }

         else if (input == 'j' && cur_pos == 1) {
            attron(COLOR_PAIR(1));
            print_center("OPTIONS", +4, 7);

            attron(COLOR_PAIR(2));
            print_center("READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == KEY_DOWN && cur_pos == 1) {
            attron(COLOR_PAIR(1));
            print_center("OPTIONS", +4, 7);

            attron(COLOR_PAIR(2));
            print_center("READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == 'k' && cur_pos == 2) {
            attron(COLOR_PAIR(1));
            print_center("READ MANUAL", +6, 11);

            attron(COLOR_PAIR(2));
            print_center("OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == KEY_UP && cur_pos == 2) {
            attron(COLOR_PAIR(1));
            print_center("READ MANUAL", +6, 11);

            attron(COLOR_PAIR(2));
            print_center("OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == 'j' && cur_pos == 2) {
            attron(COLOR_PAIR(1));
            print_center("READ MANUAL", +6, 11);

            attron(COLOR_PAIR(2));
            print_center("QUIT", +8, 4);
            cur_pos = 3;
         }

         else if (input == KEY_DOWN && cur_pos == 2) {
            attron(COLOR_PAIR(1));
            print_center("READ MANUAL", +6, 11);

            attron(COLOR_PAIR(2));
            print_center("QUIT", +8, 4);
            cur_pos = 3;
         }

         else if (input == 'k' && cur_pos == 3) {
            attron(COLOR_PAIR(1));
            print_center("QUIT", +8, 4);

            attron(COLOR_PAIR(2));
            print_center("READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == KEY_UP && cur_pos == 3) {
            attron(COLOR_PAIR(1));
            print_center("QUIT", +8, 4);

            attron(COLOR_PAIR(2));
            print_center("READ MANUAL", +6, 11);
            cur_pos = 2;
         }
      }

      attron(COLOR_PAIR(1));

      if (cur_pos == 0) // "TRANSFER!" button
         return 0;

      else if (cur_pos == 1) // "OPTIONS" button
      {
         clear();
         print_center("PRETEND THIS FEATURE IS IMPLEMENTED :)", 0, 38);
         getch();
      }

      else if (cur_pos == 2) // "READ MANUAL" button
         intro_manual();

      else if (cur_pos == 3) // "QUIT" button
      {
         endwin();
         exit(0);
      }

      return 1;
   }
}

#endif
