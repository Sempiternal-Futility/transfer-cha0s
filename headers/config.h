// This header file handles things about config files
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>

void print_center(char *msg, int y, int x);

const char *_path_mainconf = "./.config/transfer-cha0s/transfer-cha0s.conf";
const char *_path_ipconf = "./.config/transfer-of-cha0s/ip_addr.conf";

bool _failed_bind = false;
char *_ip_addrs;
bool _is_ip_conf_empty;

bool _conf_enable_ip_save = false; // If true, ip addresses will get saved to a file, so user doesn't have to type again (disabled default since buggy)
bool _conf_enable_startmenu = false;
bool _conf_enable_transfer_again = false;

void check_main_config() // Checks if the main config file exists (if not, then creates it with default settings)
{
   FILE *config = fopen(_path_mainconf, "r+");
   struct stat st;
   if (stat(_path_mainconf, &st) == -1) // stat() returns -1 if the file doesn't exist
   {
      system("touch ./.config/transfer-cha0s/transfer-cha0s.conf");
      config = fopen(_path_mainconf, "w");
      
      fprintf(config, "010");
   }

   fclose(config);
}

void write_main_config() // Checks the value of the conf bools, and then writes the config file
{
   char config_buffer_write[4];
   memset(config_buffer_write, 0, 4);

   switch (_conf_enable_ip_save)
   {
      case true: config_buffer_write[0] = '1';
                 break;

      case false: config_buffer_write[0] = '0';
                  break;
   }

   switch (_conf_enable_startmenu)
   {
      case true: config_buffer_write[1] = '1';
                 break;

      case false: config_buffer_write[1] = '0';
                  break;
   }

   switch (_conf_enable_transfer_again)
   {
      case true: config_buffer_write[2] = '1';
                 break;

      case false: config_buffer_write[2] = '0';
                  break;
   }

   FILE *config = fopen(_path_mainconf, "w");
   fprintf(config, config_buffer_write);
   fclose(config);
}

void read_main_config() // Reads the main config file and assigns the bool accordingly
{
   check_main_config();
   FILE *config = fopen(_path_mainconf, "r");

   char config_buffer_read[4];
   char tmp_buffer[4];

   memset(config_buffer_read, 0, sizeof config_buffer_read);
   memset(tmp_buffer, 0, sizeof tmp_buffer);

   while (!feof(config))
   {
      fgets(tmp_buffer, sizeof tmp_buffer, config);
      strcat(config_buffer_read, tmp_buffer);
   }

   switch (config_buffer_read[0])
   {
      case '1': _conf_enable_ip_save = true;
                break;

      case '0': _conf_enable_ip_save = false;
                break;
   }

   switch (config_buffer_read[1])
   {
      case '1': _conf_enable_startmenu = true;
                break;

      case '0': _conf_enable_startmenu = false;
                break;
   }

   switch (config_buffer_read[2])
   {
      case '1': _conf_enable_transfer_again = true;
                break;

      case '0': _conf_enable_transfer_again = false;
                break;
   }

   fclose(config);
}

void assign_ip_addrs(char *ip_addr_string, char *ip_addr_string_two) // Assings the ip addresses to "ip_addrs"
{
   _ip_addrs = malloc(40);
   memset(_ip_addrs, 0, strlen(_ip_addrs));
   strcat(_ip_addrs, ip_addr_string);
   _ip_addrs[strlen(_ip_addrs)] = '\n';
   strcat(_ip_addrs, ip_addr_string_two);
}

void send_ip(int sockfd) // Sends the ip addresses to the client (so he can save it on his machine), and also writes the ip to this machine
{
   send(sockfd, _ip_addrs, 40, 0); // Sends both ip addresses to the client, so he can write on his machine too
   FILE *ip_file = fopen(_path_ipconf, "w");
   fprintf(ip_file, _ip_addrs);
}

void recv_ip(int sockfd) // Receives the ip addresses from the server, and also writes the ip to this machine
{
   FILE *ip_file = fopen(_path_ipconf, "w");
   char *ip_config = malloc(40);
   recv(sockfd, ip_config, 40, 0);
   fprintf(ip_file, ip_config);
   free(ip_config);
}

#endif
