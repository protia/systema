#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

   /* local variables */
   char infile[1000];
   char outfile[1000];
   char command[1000];
   char incdirs[1000];
   int  i;
   int  err;

   /* initialize local variables */
   infile[0]  = 0;
   outfile[0] = 0;
   incdirs[0] = 0;
   command[0] = 0;
   i          = 1;
   err        = 0;

   /* loop over parameters */
   i = 1;
   while (i < argc) {
      if (
         argv[i][0] == 'i' &&
         argv[i][1] == 'n' &&
         argv[i][2] == '='
      ){
         sprintf(infile, "%s", &argv[i][3]);
      }
      else if(
         argv[i][0] == 'o' &&
         argv[i][1] == 'u' &&
         argv[i][2] == 't' &&
         argv[i][3] == '='
      ){
         sprintf(outfile, "%s", &argv[i][4]);
      }
      else if(
         argv[i][0] == 'i' &&
         argv[i][1] == 'n' &&
         argv[i][2] == 'c' &&
         argv[i][3] == '='
      ){
         if (incdirs[0] == 0) {
           sprintf(incdirs, "-I%s", &argv[i][4]);
         } else {
           sprintf(incdirs, "%s -I%s", incdirs, &argv[i][4]);
         }
      }
      else
      {
         printf("Unknown argument: %s, ignored.\n", argv[i]);
      }
      i++;
   };

   /* infile & outfile initialized? */
   if (infile[0] == 0 || outfile[0] == 0) {
      printf("Usage: %s in=<infile> out=<outfile>\n", argv[0]);
      err = 1;
   };

   /* now process the cmd */
   if (err == 0) {
      sprintf(command, "gcc -E -x c %s -o %s %s", incdirs, outfile, infile);
      err = system(command);
   };

   /* done */
   return err;

};

