#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

   /* local variables */
   char infile[1000];
   char outfile[1000];
   char command[1000];
   int  i;
   int  err;

   /* initialize local variables */
   infile[0]  = 0;
   outfile[0] = 0;
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
      sprintf(command, "gcc -c -x assembler -o %s %s", outfile, infile);
      err = system(command);
   };

   /* done */
   return err;

};

