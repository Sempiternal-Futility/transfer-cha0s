// This header file contains a bunch of networking functions

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

bool ask_host_type() // Asks if the host is a server or a client
{
   clear();

   move(LINES /2, (COLS /2 - 12));
   printw("IS THIS MACHINE A SERVER?");

   move((LINES /2 +1), (COLS /2 - 3));
   printw("(y/n)");
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
   {
      move(LINES /2, (COLS /2 -19));
      printw("WHAT IS THE IP ADDRESS OF THIS MACHINE?");
   }
   
   else
   {
      move(LINES /2, (COLS /2 -18));
      printw("WHAT IS THE IP ADDRESS OF THE SERVER?");
   }

   move((LINES /2 +1), (COLS /2 -3));
   printw("(IPv4)");

   move((LINES /2 +3), (COLS /2 -6));

   char input = ' ';
   char ip_addr_string[16];

   for (short i = 0; i < 16; i++) // Fills the array with the ip address
   {
      input = getch();
      if (input == '\n'){
         ip_addr_string[i] = NULL; // Takes the \0 terminating string character out
         break;
      }
      ip_addr_string[i] = input;
   }

   inet_pton(AF_INET, ip_addr_string, &(server->sin_addr));

   // If the host is a server, this will ask the ip_addr of the client
   if (is_server == true)
   {
      clear();
      move(LINES /2, (COLS /2 -18));
      printw("WHAT IS THE IP ADDRESS OF THE CLIENT?");   
      
      move((LINES /2 +1), (COLS /2 -3));
      printw("(IPv4)");
   
      move((LINES /2 +3), (COLS /2 -6));
   
      input = ' ';
      char ip_addr_string_two[16];

      for (short i = 0; i < 16; i++)
      {
         input = getch();
         if (input == '\n'){
            ip_addr_string[i] = NULL; // Takes the \0 terminating string character out
            break;
         }
         ip_addr_string_two[i] = input;
      }

      inet_pton(AF_INET, ip_addr_string_two, &(client->sin_addr));
   }

   noecho();
}

void connect_hosts(bool is_server, struct sockaddr_in server, struct sockaddr_in client) // Handles connections of the hosts
{
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (is_server == true)
   {
      if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
         move(LINES /2, (COLS /2 -7));
         fprintf(stderr, "ERROR ON BIND!\n");
         getch();
         endwin();
         exit(1);
      }

      clear(); move(LINES /2, (COLS /2 -16)); printw("WAITING FOR CLIENT TO CONNECT..."); refresh();
      listen(sockfd, 1);

      socklen_t *size = malloc(sizeof(socklen_t));
      *size = sizeof(client);
      int sockfd_client = accept(sockfd, (struct sockaddr *)&client, size);

      char buffer[255];
      clear(); move(LINES /2, (COLS /2 -9)); printw("WAITING FOR RECV...");
      recv(sockfd_client, buffer, sizeof(buffer), 0);

      printw("%s\n", buffer);

      close(sockfd_client);
      free(size);
   }

   else if (is_server == false)
   {
      if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
         move(LINES /2, (COLS /2 -8));
         fprintf(stderr, "ERROR ON CONNECT!\n");
         getch();
         endwin();
         exit(1);
      }

      char buffer[255] = "hello-there";
      send(sockfd, buffer, sizeof(buffer), 0);
   }

   close(sockfd);
}
