#include <ncurses.h>
#include <locale.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../headers/config.h"
#include "../headers/network.h"
#include "../headers/info.h"

int main()
{
   setlocale(LC_ALL, "");
   initscr(); 
   curs_set(0);
   noecho();

   read_main_config(); // Reads the main .conf file

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
   free(ip_addrs);
   endwin();
   return 0;
}
