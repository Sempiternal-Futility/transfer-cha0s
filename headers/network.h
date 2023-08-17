// This header file contains functions that handle networking

#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "config.h"
#include "info.h"
#include "style.h"

void server_send(int sockfd);
void client_recv(int sockfd);

void connect_hosts(bool is_server, struct sockaddr_in server, struct sockaddr_in client)
{
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);

   const int enable = 1;
   setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)); // Allows the TCP socket to be reused

   if (is_server == false)
   {
      if (connect(sockfd, (struct sockaddr *)&server, sizeof server) < 0) {
         move(LINES /2, (COLS /2 -8));
         fprintf(stderr, "ERROR ON CONNECT!\n");
         getch();
         endwin();
         exit(1);
      }

      else
         client_recv(sockfd);
   }

   else if (is_server == true)
   {
      if (bind(sockfd, (struct sockaddr *)&server, sizeof server) < 0) {
         move(LINES /2, (COLS /2 -7));
         fprintf(stderr, "ERROR ON BIND!\n");
         failed_bind = true;
         getch();
         endwin();
         exit(1);
      }

      clear(); 
      print_center("WAITING FOR CLIENT TO CONNECT...", 0, 32);
      listen(sockfd, 1);

      socklen_t *size = malloc(sizeof(socklen_t));
      *size = sizeof(client);
      int sockfd_client = accept(sockfd, (struct sockaddr *)&client, size);

      if (sockfd_client != -1)
         server_send(sockfd_client);

      close(sockfd_client);
      free(size);
   }

   close(sockfd);
}

void server_send(int sockfd)
{
   clear();
   print_center("CLIENT CONNECTED!", 0, 17);
   print_center("TYPE THE PATH OF A FILE TO SEND:", 1, 30);

   move((LINES /2 +3), (COLS /2 -16));
   char input;
   char path[510];
   for (short i = 0; i < 255; i++)
   {
      input = getch();
      if (input == '\n') {
         path[i] = 0;  // Takes the \0 terminating string character out
         break;
      }

      else if (input == 8 || input == 127) { // Erases input if backspace is pressed
         if (i > 0) {
            i -= 1;
            path[i] = 0;
            move(LINES /2 +3, COLS /2 -16 +i);
            printw("%c", ' ');
            i -= 1;
         }

         else
            i -= 1;
      }

      else {
         move(LINES /2 +3, COLS /2 -16 +i);
         printw("%c", input);
         path[i] = input;
      }
   }

   char *file_name = basename(path);
   bool is_ascii = is_file_ascii(path);
   char *mode;

   if (is_ascii == true)
      mode = "r";
   else if (is_ascii == false)
      mode = "rb";

   FILE *file = fopen(path, mode);

   struct stat st;
   stat(path, &st);
   size_t file_size = st.st_size;

   clear(); print_center("ALLOCATING BUFFERS...", 0, 21);

   char *file_data = malloc(file_size); // This holds the contents of the file
   char *tmp_buffer = 0; 
   if (is_ascii == true)
      tmp_buffer = malloc(file_size);

   memset(file_data, 0, file_size);
   if (is_ascii == true)
      memset(tmp_buffer, 0, file_size);

   clear(); print_center("READING FILE...", 0, 15);

   if (is_ascii == true)
   {
      while (!feof(file)) {
         strcat(file_data, tmp_buffer);
         fgets(tmp_buffer, sizeof tmp_buffer, file);
      }

      file_data[strlen(file_data) -1] = '\0'; // This fix a bug where ascii files are outputted corrupted
   }

   else if (is_ascii == false)
      fread(file_data, 1, file_size, file);

   char ascii_buffer[1];
   if (is_ascii == true)
      ascii_buffer[0] = '1';
   else if (is_ascii == false)
      ascii_buffer[0] = '0';

   print_transfer_message(file_size);

   if (enable_ip_save == true && is_ip_conf_empty == true && failed_bind == false) // These are global vars btw
      send_ip(sockfd); // Sends the ip addresses to the client (so he can save it on his machine), and also writes the ip's to this machine

   send(sockfd, ascii_buffer, sizeof ascii_buffer, 0); // Tells the client whether the file is ascii or binary
   send(sockfd, file_name, sizeof file_name, 0); // Tells the client the name of the file
   system("sleep 0.005s"); // This sleep is here to make sure the client has time to prepare for recving
   send(sockfd, &file_size, sizeof file_size, 0); // Tells the client the size of the file
  
   long n = 0;
   size_t total = 0;
   while (total < file_size)
   {
      n = send(sockfd, &file_data[total], file_size - total, 0); // Sends the actual file for the client
      total = total + n;

      if (n == -1) {
         perror("send() error"); 
         getch();
         break;
      }
   }

   free(file_data);
   free(tmp_buffer);
}

void client_recv(int sockfd)
{
   clear();
   print_center("CONNECTED TO THE SERVER!", 0, 24);
   print_center("SERVER IS CHOOSING A FILE TO SEND...", 1, 34);

   char ascii_buffer[1];
   char file_name[255];
   size_t file_size;

   if (enable_ip_save == true && is_ip_conf_empty == true && failed_bind == false) // These are global vars btw
      recv_ip(sockfd); // Receives the ip addresses from the server and writes it to this machine

   recv(sockfd, ascii_buffer, sizeof ascii_buffer, 0); // Receives the ascii bool
   recv(sockfd, file_name, sizeof file_name, 0); // Receives the file name
   recv(sockfd, &file_size, sizeof file_size, 0); // Receives the file size from the server

   clear(); print_center("ALLOCATING BUFFERS...", 0, 21);

   char *file_data = malloc(file_size); // This holds the contents of the file
   for (long unsigned int i = 0; i < sizeof file_data; i++) // Makes sure the array is empty
      file_data[i] = 0;

   print_transfer_message(file_size);

   long n = 0;
   size_t total = 0;
   while (total < file_size)
   {
      n = recv(sockfd, &file_data[total], file_size - total, 0); // Receives the file from the server
      total = total + n;

      if (n == -1) {
         perror("recv(): error");
         getchar();
         break;
      }
   }

   bool is_ascii;
   if (ascii_buffer[0] == '1')
      is_ascii = true;
   else if (ascii_buffer[0] == '0')
      is_ascii = false;

   if (is_ascii == true) // These 2 lines fix a bug where ascii files are outputted corrupted
   {
      file_data[strlen(file_data) +1] = '\0';
      file_data[strlen(file_data)] = '\n';
   }

   char *mode;
   if (is_ascii == true)
      mode = "w";
   else if (is_ascii == false)
      mode = "wb";

   clear(); print_center("WRITING FILE...", 0, 18);

   FILE *file = fopen(file_name, mode);
   if (is_ascii == true)
      fprintf(file, file_data);

   else if (is_ascii == false)
      fwrite(file_data, 1, file_size, file);

   free(file_data);
}

#endif
