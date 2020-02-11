#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "svec.h"

void chomp(char *text)
{
   // int n = strlen(text);
   // if(text[n-1] == "\n")
   //     text[n-1] = 0;
   strtok(text, "\n");
}

char *read_line(char *text)
{
   char *rv = fgets(text, 1000, stdin);
   if (!rv)
   {
      exit(0);
   }
   return text;
}

char *
read_argument(const char *text, long ii)
{
   long nn = 0;
   while (!isblank(text[ii + nn]))
   {
      ++nn;
   }

   char *arg = malloc(nn + 1);
   memcpy(arg, text + ii, nn);
   arg[nn] = 0;
   return arg;
}

svec *
tokenize(char *text)
{
   svec *sv = make_svec();
   //Length of the text
   long len = strlen(text);
   long ii = 0;

   while (ii < len)
   {
      if (isspace(text[ii]))
      {
         ++ii;
         continue;
      }

      if ((text[ii] == '|' && text[ii + 1] == '|') || (text[ii] == '&' && text[ii + 1] == '&'))
      {
         char op[] = "xx";
         op[0] = text[ii];
         op[1] = text[ii + 1];
         svec_push_back(sv, op);
         ii += 2;
         continue;
      }

      if (text[ii] == '<' || text[ii] == '>' || text[ii] == ';' || text[ii] == '|' || text[ii] == '&')
      {
         char op[] = "x";
         op[0] = text[ii];
         svec_push_back(sv, op);
         ++ii;
         continue;
      }

      char *arg = read_argument(text, ii);
      svec_push_back(sv, arg);
      ii += strlen(arg);
      free(arg);
   }
   return sv;
}

int main(int argc, char const *argv[])
{
   char text[1024];
   //Create an array that stores 1024 chars
   while (1)
   {
      printf("tokens$ ");
      fflush(stdout);
      chomp(text);
      char *line = read_line(text);
      svec *toks = tokenize(line);
      printf("value of string: %s", line);
      rev_print_vec(toks);
   }

   return 0;
}

/* TODO:

   while (1) {
     printf("tokens$ ");
     fflush(stdout);
     line = read_line()
     if (that was EOF) {
        exit(0);
     }
     tokens = tokenize(line);
     foreach token in reverse(tokens):
       puts(token)
   }

*/