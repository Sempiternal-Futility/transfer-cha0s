// This header file contains functions that boost the sex appeal of the program 

#ifndef STYLE_H
#define STYLE_H

#include "config.h"
#include <stdlib.h>
#include <stdbool.h>
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

void navigate_button(bool highlight_button, char *button_name, int posY, int posX)
{
   if (highlight_button == false)
      attron(COLOR_PAIR(1));

   else if (highlight_button == true)
      attron(COLOR_PAIR(2));

   print_center(button_name, posY, posX);
   attron(COLOR_PAIR(1));
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
   print_center("STEPS OF THE PROGRAM:", -6, 21);

   print_center("STEP 1: ANSWER IF THE HOST IS A SERVER OR A CLIENT.", -4, 51);
   print_center("(the server is the one who sends, and the client is the one who receives).", -3, 74);

   print_center("STEP 2: TYPE IN YOUR IP ADDRESS.", 0, 32);
   print_center("(if you don't know it, open a terminal and type \"ip address\")", 1, 61);

   print_center("STEP 3: TYPE THE PATH OF THE FILE TO BE SENT.", 4, 45);
   getch();

   clear();
   print_center("OPTIONS:", -10, 8);
   print_center("(these are options that you can enable/disable to your liking)", -9, 62);

   print_center("OPTION 1: WHETHER OR NOT YOU WANT TO SAVE THE IP ADDRESSES FOR QUICK ACCESS", -6, 75);
   print_center("(disabled by default, since it's very buggy)", -5, 44);

   print_center("OPTION 2: ENABLE OR DISABLE MENU SCREEN TUI", -2, 43);
   print_center("(if disabled, you won't have the tui menu. so you lose the \"OPTIONS\" button. TAKE CARE)", -1, 89);

   print_center("OPTION 3: WHETHER OR NOT YOU WANT TO SEND MORE FILES OR TO QUIT THE PROGRAM", 2, 79);
   print_center("(disabled by default)", 3, 21);

   print_center("REMEMBER! IF YOU SCREW EVERYTHING UP AND NEED THE DEFAULT SETTINGS", 6, 66);
   print_center("JUST DELETE THE MAIN CONFIG FILE AND LAUNCH THE PROGRAM AGAIN", 7, 61);
   getch();

   clear();
   print_center("KNOWN BUGS:", -11, 11);
   
   print_center("BUG 1: SUPPLYING THE PATH TO A FILE THAT DOESN'T EXIST WILL RESULT IN A SEGFAULT", -8, 80);

   print_center("BUG 2: IF THE IP ADDR SAVE OPTION IS ENABLED, BOTH HOSTS MUST HAVE THE CONF FILE AT THE SAME STATE", -5, 98);
   print_center("what i mean by \"state\" is, either both hosts have the config file empty", -4, 64);
   print_center("or both hosts have the config file filled with valid data", -3, 50);

   print_center("BUG 3: SUPPLYING A FILE THAT HAS A NAME THAT HAS A SIZE BIGGER THAN 255 BYTES WILL CAUSE A CRASH", 0, 96);
   print_center("(this is because i malloc the \"file_name\" buffer in the client-side with only 255 bytes)", 1, 88);

   print_center("BUG 4: SUPPLYING A FILE THAT HAS A SIZE BIGGER THAN", 4, 51);
   print_center("THE AMOUNT OF RAM ON THE SYSTEM WILL CAUSE A TOTAL CRASH", 5, 56);

   print_center("BE AWARE THAT THESE ARE ONLY THE KNOWN BUGS. THAT ARE THE UNKNOWN TOO :)", 8, 72);
   getch();
}

void settings_menu()
{
   clear();

   attron(COLOR_PAIR(2));
   print_center("RESTORE SETTINGS BACK TO DEFAULT", -9, 32);
   attron(COLOR_PAIR(1));
   print_center("OPTIONS:", -6, 8);
   print_center("SAVE IP ADDRESS", -4, 15);
   print_center("MENU SCREEN", -2, 11);
   print_center("\"TRANSFER AGAIN\" PROMPT", 0, 23);
   
   int input = 0;
   int cur_pos = 0;
   while (input != '\n')
   {
      input = getch();
      if (input == 'j' && cur_pos == 0) {
         navigate_button(false, "RESTORE SETTINGS BACK TO DEFAULT", -9, 32);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == KEY_DOWN && cur_pos == 0) {
         navigate_button(false, "RESTORE SETTINGS BACK TO DEFAULT", -9, 32);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == 'k' && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "RESTORE SETTINGS BACK TO DEFAULT", -9, 32);
         cur_pos = 0;
      }

      else if (input == KEY_UP && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "RESTORE SETTINGS BACK TO DEFAULT", -9, 32);
         cur_pos = 0;
      }

      else if (input == 'j' && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "MENU SCREEN", -2, 11);
         cur_pos = 2;
      }

      else if (input == KEY_DOWN && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "MENU SCREEN", -2, 11);
         cur_pos = 2;
      }

      else if (input == 'k' && cur_pos == 2) {
         navigate_button(false, "MENU SCREEN", -2, 11);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == KEY_UP && cur_pos == 2) {
         navigate_button(false, "MENU SCREEN", -2, 11);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == 'j' && cur_pos == 2) {
         navigate_button(false, "MENU SCREEN", -2, 11);
         navigate_button(true, "\"TRANSFER AGAIN\" PROMPT", 0, 23);
         cur_pos = 3;
      }

      else if (input == KEY_DOWN && cur_pos == 2) {
         navigate_button(false, "MENU SCREEN", -2, 11);
         navigate_button(true, "\"TRANSFER AGAIN\" PROMPT", 0, 23);
         cur_pos = 3;
      }

      else if (input == 'k' && cur_pos == 3) {
         navigate_button(false, "\"TRANSFER AGAIN\" PROMPT", 0, 23);
         navigate_button(true, "MENU SCREEN", -2, 11);
         cur_pos = 2;
      }

      else if (input == KEY_UP && cur_pos == 3) {
         navigate_button(false, "\"TRANSFER AGAIN\" PROMPT", 0, 23);
         navigate_button(true, "MENU SCREEN", -2, 11);
         cur_pos = 2;
      }
   }

   if (cur_pos == 0)
   {
      FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      fprintf(config, "010"); // Prints the default config to the file
      fclose(config);

      clear();
      print_center("SETTINGS CHANGED!", 0, 17);
      print_center("(changes will take effect only once the program is restarted)", 1, 61);
      getch();
   }

   else if (cur_pos == 1)
   {
      char buffer[4];
      FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");
      fgets(buffer, sizeof(buffer), config);

      if (_conf_enable_ip_save == true)
         buffer[0] = '0'; // If the config is enabled, then it gets disabled

      else if (_conf_enable_ip_save == false)
         buffer[0] = '1'; // If config is disabled, then it gets enabled
   
      config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      fprintf(config, buffer);
      fclose(config);

      clear();
      if (buffer[0] == '0')
         print_center("FEATURE DISABLED!", 0, 17);

      else if (buffer[0] == '1')
         print_center("FEATURE ENABLED!", 0, 16);

      print_center("(changes will take effect only once the program is restarted)", 1, 61);
      getch();
   }

   else if (cur_pos == 2)
   {
      char buffer[4];
      FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");
      fgets(buffer, sizeof(buffer), config);

      if (_conf_enable_startmenu == true)
         buffer[1] = '0'; // If the config is enabled, then it gets disabled

      else if (_conf_enable_startmenu == false)
         buffer[1] = '1'; // If config is disabled, then it gets enabled
   
      config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      fprintf(config, buffer);
      fclose(config);

      clear();
      if (buffer[1] == '0')
         print_center("FEATURE DISABLED!", 0, 17);

      else if (buffer[1] == '1')
         print_center("FEATURE ENABLED!", 0, 16);

      print_center("(changes will take effect only once the program is restarted)", 1, 61);
      getch();
   }

   else if (cur_pos == 3) {
      char buffer[4];
      FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");
      fgets(buffer, sizeof(buffer), config);

      if (_conf_enable_transfer_again == true)
         buffer[2] = '0'; // If the config is enabled, then it gets disabled

      else if (_conf_enable_transfer_again == false)
         buffer[2] = '1'; // If config is disabled, then it gets enabled
   
      config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      fprintf(config, buffer);
      fclose(config);

      clear();
      if (buffer[2] == '0')
         print_center("FEATURE DISABLED!", 0, 17);

      else if (buffer[2] == '1')
         print_center("FEATURE ENABLED!", 0, 16);

      print_center("(changes will take effect only once the program is restarted)", 1, 61);
      getch();
   }
}

int display_start_menu() // Displays the start menu when the program is opened
{
   if (_conf_enable_startmenu == true)
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
            navigate_button(false, "TRANSFER!", +2, 9);
            navigate_button(true, "OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == KEY_DOWN && cur_pos == 0) {
            navigate_button(false, "TRANSFER!", +2, 9);
            navigate_button(true, "OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == 'k' && cur_pos == 1) {
            navigate_button(false, "OPTIONS", +4, 7);
            navigate_button(true, "TRANSFER!", +2, 9);
            cur_pos = 0;
         }

         else if (input == KEY_UP && cur_pos == 1) {
            navigate_button(false, "OPTIONS", +4, 7);
            navigate_button(true, "TRANSFER!", +2, 9);
            cur_pos = 0;
         }

         else if (input == 'j' && cur_pos == 1) {
            navigate_button(false, "OPTIONS", +4, 7);
            navigate_button(true, "READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == KEY_DOWN && cur_pos == 1) {
            navigate_button(false, "OPTIONS", +4, 7);
            navigate_button(true, "READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == 'k' && cur_pos == 2) {
            navigate_button(false, "READ MANUAL", +6, 11);
            navigate_button(true, "OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == KEY_UP && cur_pos == 2) {
            navigate_button(false, "READ MANUAL", +6, 11);
            navigate_button(true, "OPTIONS", +4, 7);
            cur_pos = 1;
         }

         else if (input == 'j' && cur_pos == 2) {
            navigate_button(false, "READ MANUAL", +6, 11);
            navigate_button(true, "QUIT", +8, 4);
            cur_pos = 3;
         }

         else if (input == KEY_DOWN && cur_pos == 2) {
            navigate_button(false, "READ MANUAL", +6, 11);
            navigate_button(true, "QUIT", +8, 4);
            cur_pos = 3;
         }

         else if (input == 'k' && cur_pos == 3) {
            navigate_button(false, "QUIT", +8, 4);
            navigate_button(true, "READ MANUAL", +6, 11);
            cur_pos = 2;
         }

         else if (input == KEY_UP && cur_pos == 3) {
            navigate_button(false, "QUIT", +8, 4);
            navigate_button(true, "READ MANUAL", +6, 11);
            cur_pos = 2;
         }
      }

      attron(COLOR_PAIR(1));

      if (cur_pos == 0) // "TRANSFER!" button
         return 0;

      else if (cur_pos == 1) // "OPTIONS" button
      {
         settings_menu(); 
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
