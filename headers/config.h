// This header file handles things about config files
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ncurses.h>

void print_center(char *msg, int y, int x);

bool failed_bind = false;
char *ip_addrs;
bool is_ip_conf_empty;

bool conf_enable_ip_save = false; // If true, ip addresses will get saved to a file, so user doesn't have to type again (disabled default since buggy)
bool conf_enable_startmenu = false;

void check_main_config() // Checks if the main config file exists (if not, then creates it with default settings)
{
   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r+");
   struct stat st;
   if (stat("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", &st) == -1) // stat() returns -1 if the file doesn't exist
   {
      system("touch ./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf");
      config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      
      fprintf(config, "01");
   }

   fclose(config);
}

void write_main_config() // Checks the value of the conf bools, and then writes the config file
{
   char config_buffer_write[3];
   memset(config_buffer_write, 0, strlen(config_buffer_write));

   switch(conf_enable_ip_save)
   {
      case true: config_buffer_write[0] = '1';
                 break;

      case false: config_buffer_write[0] = '0';
                  break;
   }

   switch(conf_enable_startmenu)
   {
      case true: config_buffer_write[1] = '1';
                 break;

      case false: config_buffer_write[1] = '0';
                  break;
   }

   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
   fprintf(config, config_buffer_write);
   fclose(config);
}

void read_main_config() // Reads the main config file and assigns the bool accordingly
{
   check_main_config();
   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");

   char config_buffer_read[3];
   char tmp_buffer[3];

   memset(config_buffer_read, 0, sizeof config_buffer_read);
   memset(tmp_buffer, 0, sizeof tmp_buffer);

   while (!feof(config))
   {
      fgets(tmp_buffer, sizeof tmp_buffer, config);
      strcat(config_buffer_read, tmp_buffer);
   }

   switch(config_buffer_read[0])
   {
      case '1': conf_enable_ip_save = true;
                break;

      case '0': conf_enable_ip_save = false;
                break;
   }

   switch(config_buffer_read[1])
   {
      case '1': conf_enable_startmenu = true;
                break;

      case '0': conf_enable_startmenu = false;
                break;
   }

   fclose(config);
}

void assign_ip_addrs(char *ip_addr_string, char *ip_addr_string_two) // Assings the ip addresses to "ip_addrs"
{
   ip_addrs = malloc(40);
   memset(ip_addrs, 0, strlen(ip_addrs));
   strcat(ip_addrs, ip_addr_string);
   ip_addrs[strlen(ip_addrs)] = '\n';
   strcat(ip_addrs, ip_addr_string_two);
}

void send_ip(int sockfd) // Sends the ip addresses to the client (so he can save it on his machine), and also writes the ip to this machine
{
   send(sockfd, ip_addrs, 40, 0); // Sends both ip addresses to the client, so he can write on his machine too
   FILE *ip_file = fopen("./.config/transfer-of-cha0s-conf/ip_addr.conf", "w");
   fprintf(ip_file, ip_addrs);
}

void recv_ip(int sockfd) // Receives the ip addresses from the server, and also writes the ip to this machine
{
   FILE *ip_file = fopen("./.config/transfer-of-cha0s-conf/ip_addr.conf", "w");
   char *ip_config = malloc(40);
   recv(sockfd, ip_config, 40, 0);
   fprintf(ip_file, ip_config);
   free(ip_config);
}

void settings_menu()
{
   clear();

   attron(COLOR_PAIR(2));
   print_center("RESTORE SETTINGS BACK TO DEFAULT", -8, 32);
   attron(COLOR_PAIR(1));
   print_center("SAVE IP ADDRESS", -4, 15);
   print_center("MENU SCREEN", -2, 11);
   
   int input = 0;
   int cur_pos = 0;
   while (input != '\n')
   {
      input = getch();
      if (input == 'j' && cur_pos == 0) {
         navigate_button(false, "RESTORE SETTINGS BACK TO DEFAULT", -8, 32);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == KEY_DOWN && cur_pos == 0) {
         navigate_button(false, "RESTORE SETTINGS BACK TO DEFAULT", -8, 32);
         navigate_button(true, "SAVE IP ADDRESS", -4, 15);
         cur_pos = 1;
      }

      else if (input == 'k' && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "RESTORE SETTINGS BACK TO DEFAULT", -8, 32);
         cur_pos = 0;
      }

      else if (input == KEY_UP && cur_pos == 1) {
         navigate_button(false, "SAVE IP ADDRESS", -4, 15);
         navigate_button(true, "RESTORE SETTINGS BACK TO DEFAULT", -8, 32);
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
   }
}

#endif
