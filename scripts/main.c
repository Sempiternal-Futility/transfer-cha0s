#include <ncurses.h>
#include <locale.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../headers/network.h" // This header contains all of these networking functions

//TODO: Fix file_buf in client-size (is corrupted for ascii files)
//      Use "is_file_ascii()" on server-side and send the bool instead of the path
//      break the for loop in "is_file_ascii" after knowing for sure the file type
//      Once i am able to copy folders, add little animation while the program is busy 

int main()
{
   setlocale(LC_ALL, "");
   initscr(); 
   curs_set(0);
   noecho();

   struct sockaddr_in *server = malloc(sizeof(struct sockaddr_in));
   struct sockaddr_in *client = malloc(sizeof(struct sockaddr_in));
   server->sin_family = AF_INET;
   server->sin_port = htons(9898);

   bool is_server = ask_host_type();
   get_host_ipaddr(is_server, server, client);

   if (is_server == true)
   {
      client->sin_family = AF_INET;
      client->sin_port = htons(9898);
   }

   connect_hosts(is_server, *server, *client); // Here I dereference the structs because I can't "bind()" the server struct as a pointer :(

   getch();

   free(server);
   free(client);
   endwin();
   return 0;
}
