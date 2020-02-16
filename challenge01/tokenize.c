#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "svec.h"
#include "helper.h"

int is_special(char x)
{
   if (x == '|' || x == '&' || x == ';' || x == '<' || x == '>')
      return 1;
   return 0;
}

char *
read_argument(const char *text, long ii)
{
   long nn = 0;
   while (!isblank(text[ii + nn]) && !is_special(text[ii + nn]))
   {
      ++nn;
   }

   char *arg = calloc(nn + 1,sizeof(char));
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
      if (isspace(text[ii]) || text[ii] == 0 || text[ii] == '\n')
      {
         // puts("space");
         ++ii;
         continue;
      }

      if ((text[ii] == '|' && text[ii + 1] == '|') || (text[ii] == '&' && text[ii + 1] == '&'))
      {
         // puts("doubles");
         char spec[] = "xx";
         spec[0] = text[ii];
         spec[1] = text[ii + 1];
         svec_push_back(sv, spec);
         ii += 2;
         continue;
      }

      if (text[ii] == '<' || text[ii] == '>' || text[ii] == ';' || text[ii] == '|' || text[ii] == '&')
      {
         // puts("singles");
         char spec[] = "x";
         spec[0] = text[ii];
         svec_push_back(sv, spec);
         ++ii;
         continue;
      }

      if (text[ii] != 0)
      {
         char *arg = read_argument(text, ii);
         chomp(arg);
         svec_push_back(sv, arg);
         ii += strlen(arg);
         // printf("Arg: (%s)\n", arg);
         free(arg);

         continue;
      }
   }
   return sv;
}