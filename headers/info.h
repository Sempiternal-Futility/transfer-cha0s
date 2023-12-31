// This header file contains functions that get some kind of information from the user

#ifndef INFO_H
#define INFO_H

#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "style.h"

bool ask_host_type() // Asks if the host is a server or a client
{
   clear();
   print_center("IS THIS MACHINE A SERVER OR A CLIENT?", 0, 36);

   attron(COLOR_PAIR(2));
   print_center("SERVER", 2, 20);

   attron(COLOR_PAIR(1));
   print_center("CLIENT", 2, -8);

   int input = 0;
   int cur_pos = 0;
   keypad(stdscr, true);
   while(input != '\n')
   {
      input = getch();

      if (input == 'l' && cur_pos == 0) {
         navigate_button(false, "SERVER", 2, 20);
         navigate_button(true, "CLIENT", 2, -8);
         cur_pos = 1;
      }

      else if (input == KEY_RIGHT && cur_pos == 0) {
         navigate_button(false, "SERVER", 2, 20);
         navigate_button(true, "CLIENT", 2, -8);
         cur_pos = 1;
      }

      else if (input == 'h' && cur_pos == 1) {
         navigate_button(false, "CLIENT", 2, -8);
         navigate_button(true, "SERVER", 2, 20);
         cur_pos = 0;
      }

      else if (input == KEY_LEFT && cur_pos == 1) {
         navigate_button(false, "CLIENT", 2, -8);
         navigate_button(true, "SERVER", 2, 20);
         cur_pos = 0;
      }
   }

   if (cur_pos == 0)
      return true;

   else
      return false;
}

void get_host_ipaddr(bool is_server, struct sockaddr_in *server, struct sockaddr_in *client) // Gets the IPv4 address of the host
{
   _is_ip_conf_empty = true;

   if (_conf_enable_ip_save == true) // If ip saving option is enabled
   {
      FILE *ip_file = fopen(_path_ipconf, "r+");
      struct stat st;
      if (stat(_path_ipconf, &st) == -1) // stat() returns -1 if the file doesn't exist
      { 
         system("touch ./.config/transfer-cha0s/ip_addr.conf"); // Creates the file
         stat(_path_ipconf, &st); // Reruns stat, so that it's not corrupted
         ip_file = fopen(_path_ipconf, "r+"); // Reruns fopen, so that it's not corrupted
      }

      else 
      {
         if (st.st_size == 0)
            _is_ip_conf_empty = true;
   
         else if (st.st_size != 0)
            _is_ip_conf_empty = false;
      }
   }

   if (_is_ip_conf_empty == true) // Checks if the .conf file is empty
   {
      clear();
      keypad(stdscr, false); // I (for some reason) have to disable keypad if i want to use backsapce to erase

      if (is_server == true)
         print_center("WHAT IS THE IP ADDRESS OF THIS MACHINE?", 0, 39);
   
      else
         print_center("WHAT IS THE IP ADDRESS OF THE SERVER?", 0, 37);

      print_center("(IPv4)", 1, 6);

      move((LINES /2 +3), (COLS /2 -6));

      char input = ' ';
      char ip_addr_string[16];

      for (short i = 0; i < 16; i++) // Fills the array with the ip address
      {
         input = getch();

         if (input == '\n') {
            ip_addr_string[i] = 0; // Takes the \0 terminating string character out
            break;
         }

         else if (input == 8 || input == 127) { // Erases if backspace is pressed
            if (i > 0) {
               i -= 1;
               ip_addr_string[i] = 0;
               move(LINES /2 +3, COLS /2 -6 +i);
               printw("%c", ' ');
               i -= 1;
            }

            else
               i -= 1;
         }

         else {
            move(LINES /2 +3, COLS /2 -6 +i);
            printw("%c", input);
            ip_addr_string[i] = input;
         }
      }

      inet_pton(AF_INET, ip_addr_string, &(server->sin_addr));

      if (is_server == true) // If the host is a server, this will ask the ip of the client too
      {
         clear();
         print_center("WHAT IS THE IP ADDRESS OF THE CLIENT?", 0, 37);
         print_center("(IPv4)", 1, 6);

         move((LINES /2 +3), (COLS /2 -6));
   
         input = ' ';
         char ip_addr_string_two[16];

         for (short i = 0; i < 16; i++)
         {
            input = getch();
            if (input == '\n') {
               ip_addr_string_two[i] = 0; // Takes the \0 terminating string character out
               break;
            }

            else if (input == 8 || input == 127) {
               if (i > 0) {
                  i -= 1;
                  ip_addr_string_two[i] = 0;
                  move(LINES /2 +3, COLS /2 -6 +i);
                  printw("%c", ' ');
                  i -= 1;
               }         

               else
                  i -= 1;
            }

            else {
               move(LINES /2 +3, COLS /2 -6 +i);
               printw("%c", input);
               ip_addr_string_two[i] = input;
            }
         }


         inet_pton(AF_INET, ip_addr_string_two, &(client->sin_addr));

         assign_ip_addrs(ip_addr_string, ip_addr_string_two); 
      }
   }

   else if (_is_ip_conf_empty == false) // Checks if the file is not empty
   {
      FILE *ip_file = fopen(_path_ipconf, "r+");
      char ip_buffer[36];
      char second_buffer[36]; // Contains the second ip address

      memset(ip_buffer, 0, sizeof ip_buffer);
      memset(second_buffer, 0, sizeof second_buffer);

      while (!feof(ip_file)) {
         strcat(ip_buffer, second_buffer);
         fgets(second_buffer, sizeof second_buffer, ip_file);
      }
      
      char *first_ip = strtok(ip_buffer, "\n"); // Contains the first ip address
      inet_pton(AF_INET, first_ip, &(server->sin_addr));
      inet_pton(AF_INET, second_buffer, &(client->sin_addr));
   }
}

bool is_file_ascii(char *path)
{
   // I have no clue what happens in this func
   // Just simply Ctrl-C'd and Ctrl-V'd this code: https://stackoverflow.com/questions/17071702/c-language-read-from-stdout

   int  stdout_bk; //is fd for stdout backup

   stdout_bk = dup(fileno(stdout));

   int pipefd[2];
   pipe2(pipefd, 0); // O_NONBLOCK);

   // What used to be stdout will now go to the pipe.
   dup2(pipefd[1], fileno(stdout));

   char buffer[510];
   snprintf(buffer, sizeof buffer, "file -b %s", path);
   system(buffer);
   fflush(stdout);//flushall();
   write(pipefd[1], "good-bye", 9); // null-terminated string!
   close(pipefd[1]);

   dup2(stdout_bk, fileno(stdout));//restore

   char buf[101];
   read(pipefd[0], buf, 100);

   bool ascii = false;
   for (size_t i = 0; i < strlen(buf); i++)
   {
      if (buf[i] == 't')
         if (buf[i+1] == 'e')
            if (buf[i+2] == 'x')
               if (buf[i+3] == 't')
                  ascii = true;
   }

   return ascii;
}

bool ask_user_transfer_again()
{
   if (_conf_enable_transfer_again == true)
   {
      clear();
      print_center("DO YOU WANT TO TRANSFER AGAIN?", 0, 30);

      attron(COLOR_PAIR(2));
      print_center("YES", 2, 8);
      attron(COLOR_PAIR(1));
      print_center("NO", 2, -8);

      int input = 0;
      int cur_pos = 0;
      keypad(stdscr, true);
      while (input != '\n')
      {
         input = getch();

         if (input == 'l' && cur_pos == 0) {
            navigate_button(false, "YES", 2, 8);
            navigate_button(true, "NO", 2, -8);
            cur_pos = 1;
         }

         else if (input == KEY_RIGHT && cur_pos == 0) {
            navigate_button(false, "YES", 2, 8);
            navigate_button(true, "NO", 2, -8);
            cur_pos = 1;
         }

         else if (input == 'h' && cur_pos == 1) {
            navigate_button(false, "NO", 2, -8);
            navigate_button(true, "YES", 2, 8);
            cur_pos = 0;
         }

         else if (input == KEY_LEFT && cur_pos == 1) {
            navigate_button(false, "NO", 2, -8);
            navigate_button(true, "YES", 2, 8);
            cur_pos = 0;
         }
      }

      if (cur_pos == 0)
         return true;

      else
         return false;
   }
}

bool is_path_file(const char *path)
{   
   int  stdout_bk; //is fd for stdout backup

   stdout_bk = dup(fileno(stdout));

   int pipefd[2];
   pipe2(pipefd, 0); // O_NONBLOCK);

   // What used to be stdout will now go to the pipe.
   dup2(pipefd[1], fileno(stdout));

   char buffer[510];
   snprintf(buffer, sizeof buffer, "file -b %s", path);
   system(buffer);
   fflush(stdout);//flushall();
   write(pipefd[1], "good-bye", 9); // null-terminated string!
   close(pipefd[1]);

   dup2(stdout_bk, fileno(stdout));//restore

   char buf[101];
   read(pipefd[0], buf, 100);

   bool is_file = true;
   for (size_t i = 0; i < strlen(buf); i++)
   {
      if (buf[i] == 'd')
         if (buf[i+1] == 'i')
            if (buf[i+2] == 'r')
               if (buf[i+3] == 'e')
                  if (buf[i+4] == 'c')
                     if (buf[i+5] == 't')
                        if (buf[i+6] == 'o')
                           if (buf[i+7] == 'r')
                              if (buf[i+8] == 'y')
                                 is_file = false;
   }

   return is_file;
}

#endif
