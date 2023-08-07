// This header file contains a bunch of networking functions

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "funcs.h"

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
   clear();
   echo();

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
      if (input == '\n'){
         ip_addr_string[i] = 0; // Takes the \0 terminating string character out
         break;
      }

      ip_addr_string[i] = input;
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
         if (input == '\n'){
            ip_addr_string[i] = 0; // Takes the \0 terminating string character out
            break;
         }
         ip_addr_string_two[i] = input;
      }

      inet_pton(AF_INET, ip_addr_string_two, &(client->sin_addr));
   }

   noecho();
}

void server_send(int sockfd)
{
   echo();
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

      path[i] = input;
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

   char file_data[file_size]; // This holds the contents of the file
   char tmp_buffer[file_size];
   for (size_t i = 0; i < file_size; i++) // Makes sure both strings are NULL
   {
      file_data[i] = 0;
      tmp_buffer[i] = 0;
   }

   if (is_ascii == true)
   {
      while (!feof(file)) {
         strcat(file_data, tmp_buffer);
         fgets(tmp_buffer, sizeof tmp_buffer, file);
      }
   }

   else if (is_ascii == false)
      fread(file_data, file_size, 1, file);

   char file_size_buffer[sizeof(size_t)];
   for (size_t i = 0; i < sizeof file_size_buffer; i++) // Makes sure the string is NULL
      file_size_buffer[i] = 0;

   snprintf(file_size_buffer, sizeof file_size_buffer, "%zu", file_size);

   char ascii_buffer[1];
   if (is_ascii == true)
      ascii_buffer[0] = '1';
   else if (is_ascii == false)
      ascii_buffer[0] = '0';

   send(sockfd, ascii_buffer, sizeof ascii_buffer, 0); // Tells the client whether the file is ascii or binary
   send(sockfd, file_name, sizeof file_name, 0); // Tells the client the name of the file
   system("sleep 0.005s"); // This sleep is here to make sure the client has time to prepare for recving
   send(sockfd, file_size_buffer, sizeof file_size_buffer, 0); // Tells the client the size of the file
  
   long n = 0;
   size_t total = 0;
   while (total < file_size)
   {
      n = send(sockfd, &file_data[total], file_size, 0); // Sends the actual file for the client
      total = total + n;

      if (n == -1) {
         perror("send() error"); 
         getch();
         break;
      }
   }

   clear();
   move(LINES /2, (COLS /2 -6));
   printw("Bytes sent: %zu\n", total);
   getch();

   noecho();
}

void client_recv(int sockfd)
{
   clear();
   print_center("CONNECTED TO THE SERVER!", 0, 24);
   print_center("SERVER IS CHOOSING A FILE TO SEND...", 1, 34);

   char ascii_buffer[1];
   char file_name[255];
   char file_size_buffer[sizeof(size_t)];

   recv(sockfd, ascii_buffer, sizeof ascii_buffer, 0); // Receives the ascii bool
   recv(sockfd, file_name, sizeof file_name, 0); // Receives the file name
   recv(sockfd, file_size_buffer, sizeof file_size_buffer, 0); // Receives the file size from the server
         
   size_t file_size = atol(file_size_buffer);

   char file_data[file_size]; // This holds the contents of the file
   for (long unsigned int i = 0; i < sizeof file_data; i++) // Makes sure the array is empty
      file_data[i] = 0;

   long n = 0;
   size_t total = 0;
   while (total < file_size)
   {
      n = recv(sockfd, &file_data[total], file_size, 0); // Receives the file from the server
      total = total + n;

      if (n == -1) {
         perror("recv(): error");
         getchar();
         break;
      }
   }

   clear();
   move(LINES /2, (COLS /2 -6));
   printw("Bytes recv: %zu\n", total);
   getch();

   bool is_ascii;
   if (ascii_buffer[0] == '1')
      is_ascii = true;
   else if (ascii_buffer[0] == '0')
      is_ascii = false;

   char *mode;

   if (is_ascii == true)
      mode = "w";
   else if (is_ascii == false)
      mode = "wb";

   FILE *file = fopen(file_name, mode);
   if (is_ascii == true)
      fprintf(file, file_data);

   else if (is_ascii == false)
      fwrite(file_data, file_size, 1, file);
}

void connect_hosts(bool is_server, struct sockaddr_in server, struct sockaddr_in client) // Handles connections of the hosts
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
