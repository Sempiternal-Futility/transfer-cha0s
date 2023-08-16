// This header file contains functions that get some kind of information from the user

#ifndef INFO_H
#define INFO_H

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "config.h"
#include "style.h"

bool ask_host_type() // Asks if the host is a server or a client
{
   clear();
   print_center("IS THIS MACHINE A SERVER?", 0, 25);
   print_center("(y/n)", 1, 5);

   char answer = getch();
   switch(answer)
   {
      case 'y':
         return true;
         break;

      case 'Y':
         return true;
         break;

      case 'n':
         return false;
         break;

      case 'N':
         return false;
         break;

      default:
         ask_host_type(); // Loop function
   }
}

void get_host_ipaddr(bool is_server, struct sockaddr_in *server, struct sockaddr_in *client) // Gets the IPv4 address of the host
{
   bool empty_file = true;

   if (enable_ip_save == true) // If ip saving option is enabled
   {
      FILE *ip_file = fopen("./.config/transfer-of-cha0s-conf/ip_addr.conf", "r+");
      struct stat st;
      if (stat("./.config/transfer-of-cha0s-conf/ip_addr.conf", &st) == -1) // stat() returns -1 if the file doesn't exist
      { 
         system("touch ./.config/transfer-of-cha0s-conf/ip_addr.conf"); // Creates the file
         stat("./.config/transfer-of-cha0s-conf/ip_addr.conf", &st); // Reruns stat, so that it's not corrupted
         ip_file = fopen("./.config/transfer-of-cha0s-conf/ip_addr.conf", "r+"); // Reruns fopen, so that it's not corrupted
      }
      else 
      {
         if (st.st_size == 0)
            empty_file = true;
   
         else if (st.st_size != 0)
            empty_file = false;
      }
   }

   if (empty_file == true) // Checks if the .conf file is empty
   {
      clear();
      ip_config_empty = true;

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

         /*ip_addrs = malloc(40);
         strcat(ip_addrs, ip_addr_string);
         ip_addrs[strlen(ip_addrs)] = '\n';
         strcat(ip_addrs, ip_addr_string_two);*/
         assign_ip_addrs(ip_addr_string, ip_addr_string_two); 
      }
   }

   else if (empty_file == false) // Checks if the file is not empty
   {
      FILE *ip_file = fopen("./.config/transfer-of-cha0s-conf/ip_addr.conf", "r+");
      ip_config_empty = false;
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

#endif
