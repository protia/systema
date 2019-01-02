#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

   /* local variables */
   char infiles[10000];
   char outfile[1000];
   char command[1000];
   int  i;
   int  err;

   /* initialize local variables */
   infiles[0] = 0;
   outfile[0] = 0;
   command[0] = 0;
   err        = 0;
   i          = 1;

   /* loop over parameters */
   while (i < argc) {
      if (
         argv[i][0] == 'i' &&
         argv[i][1] == 'n' &&
         argv[i][2] == '='
      ){
         if (infiles[0] == 0) {
            sprintf(infiles, "%s", &argv[i][3]);
         } else {
            sprintf(infiles, "%s %s", infiles, &argv[i][3]);
         }
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
   if (infiles[0] == 0 || outfile[0] == 0) {
      printf("Usage: %s in=<infile1> [in=<infile2>] ... out=<outfile>\n", argv[0]);
      err = 1;
   };

   /* now process the cmd */
   if (err == 0) {
      sprintf(command, "gcc -o %s %s", outfile, infiles);
      err = system(command);
   };

   /* done */
   return err;

};

