// This header file handles things about config files
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "style.h"

bool failed_bind = false;
char *ip_addrs;
bool is_ip_conf_empty;

bool conf_enable_ip_save = false; // If true, ip addresses will get saved to a file, so user doesn't have to type again (disabled default since buggy)

void check_main_config() // Checks if the main config file exists (if not, then creates it with default settings)
{
   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r+");
   struct stat st;
   if (stat("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", &st) == -1) // stat() returns -1 if the file doesn't exist
   {
      system("touch ./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf");
      config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
      
      fprintf(config, "0");
   }

   fclose(config);
}

void write_main_config() // Checks the value of the conf bools, and then writes the config file
{
   char config_buffer[2];
   memset(config_buffer, 0, strlen(config_buffer));

   switch(conf_enable_ip_save)
   {
      case true: config_buffer[0] = '1';
                 break;

      case false: config_buffer[0] = '0';
                  break;
   }

   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "w");
   fprintf(config, config_buffer);
   fclose(config);
}

void read_main_config() // Reads the main config file and assigns the bool accordingly
{
   check_main_config();
   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");

   char config_buffer[2];
   char tmp_buffer[2];

   memset(config_buffer, 0, sizeof config_buffer);
   memset(tmp_buffer, 0, sizeof tmp_buffer);

   while (!feof(config))
   {
      fgets(tmp_buffer, sizeof tmp_buffer, config);
      strcat(config_buffer, tmp_buffer);
   }

   switch(config_buffer[0])
   {
      case '1': conf_enable_ip_save = true;
                break;

      case '0': conf_enable_ip_save = false;
                break;
   }

   fclose(config);
   write_main_config();
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

#endif
