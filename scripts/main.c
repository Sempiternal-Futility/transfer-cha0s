#include <ncurses.h>
#include <locale.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>

bool ask_host_type();
void get_host_ipaddr(bool is_server, struct sockaddr_in server, struct sockaddr_in client);

int main()
{
   setlocale(LC_ALL, "");
   initscr(); 
   curs_set(0);
   noecho();

   struct sockaddr_in server;
   struct sockaddr_in client;

   bool is_server = ask_host_type();
   get_host_ipaddr(is_server, server, client);

   getch();

   endwin();
   return 0;
}

bool ask_host_type()
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

void get_host_ipaddr(bool is_server, struct sockaddr_in server, struct sockaddr_in client)
{
   clear();
   echo();

   move(LINES /2, (COLS /2 -12));
   printw("WHAT IS YOUR IP ADDRESS?");
   
   move((LINES /2 +1), (COLS /2 -3));
   printw("(IPv4)");

   move((LINES /2 +3), (COLS /2 -6));

   char input = ' ';
   char ip_addr_string[16];

   for (short i = 0; i < 16; i++) // Fills the array with the ip address
   {
      input = getch();
      if (input == '\n')
         break;
      ip_addr_string[i] = input;
   }

   if (is_server == true)
      inet_pton(AF_INET, ip_addr_string, &(server.sin_addr));

   else if (is_server == false)
      inet_pton(AF_INET, ip_addr_string, &(client.sin_addr));

   noecho();
}
