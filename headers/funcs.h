// This header file contains handy functions

bool is_file_ascii(char *path)
{
   // I have no clue what happens in this func
   // Just simply Ctrl-C'd and Ctrl-V'd this code: https://stackoverflow.com/questions/17071702/c-language-read-from-stdout

   int  stdout_bk; //is fd for stdout backup

   stdout_bk = dup(fileno(stdout));

   int pipefd[2];
   pipe2(pipefd, 0); // O_NONBLOCK);

   // What used to be stdout will now go to the pipe.
   dup2(pipefd[1], fileno(stdout));

   char buffer[510];
   snprintf(buffer, sizeof buffer, "file -b %s", path);
   system(buffer);
   fflush(stdout);//flushall();
   write(pipefd[1], "good-bye", 9); // null-terminated string!
   close(pipefd[1]);

   dup2(stdout_bk, fileno(stdout));//restore

   char buf[101];
   read(pipefd[0], buf, 100);

   bool ascii = false;
   for (size_t i = 0; i < strlen(buf); i++)
   {
      if (buf[i] == 't')
         if (buf[i+1] == 'e')
            if (buf[i+2] == 'x')
               if (buf[i+3] == 't')
                  ascii = true;
   }

   return ascii;
}
