// This header file handles things about config files
#define CONFIG_H
#ifdef CONFIG_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

bool enable_ip_save = false; // If true, ip addresses will get saved to a file, so user doesn't have to type again (disabled default since buggy)

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

void read_main_config() // Reads the main config file and assigns the bool accordingly
{
   check_main_config();
   FILE *config = fopen("./.config/transfer-of-cha0s-conf/transfer-of-cha0s.conf", "r");

   char config_buffer[10];
   char tmp_buffer[10];

   memset(config_buffer, 0, sizeof config_buffer);
   memset(tmp_buffer, 0, sizeof tmp_buffer);

   while (!feof(config))
   {
      fgets(tmp_buffer, sizeof tmp_buffer, config);
      strcat(config_buffer, tmp_buffer);
   }

   if (config_buffer[0] == '0')
      enable_ip_save = false;

   else if (config_buffer[0] == '1')
      enable_ip_save = true;

   fclose(config);
}

#endif
