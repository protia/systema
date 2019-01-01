#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[]) {
    /* local variables */
    char *if_name = NULL;
    char *of_name = NULL;
    int i, ret;
    FILE *fin, *fout;
    /* read arguments */
    for (i = 0; i < argc; i++) {
        if (argv[i][0] == 'i' &&
            argv[i][1] == 'n' &&
            argv[i][2] == '=') {
              /* input file */
              if_name = &argv[i][3];
        }
        if (argv[i][0] == 'o' &&
            argv[i][1] == 'u' &&
            argv[i][2] == 't' &&
            argv[i][3] == '=') {
              /* output file */
              of_name = &argv[i][4];
        }
    }
    /* arguments are valid? */
    if (!if_name || !of_name) {
        fprintf(stderr, "Usage: %s in=<input-file> out=<out-file>\n", argv[0]);
        return -1;
    }
    /* attempt to open input file */
    fin = fopen(if_name, "r");
    if (!fin) {
        fprintf(stderr, "Can't open %s for read.\n", if_name);
        return -1;
    }
    /* attempt to open output file */
    fout = fopen(of_name, "w");
    if (!fout) {
        fprintf(stderr, "Can't open %s for write.\n", of_name);
        return -1;
    }
    /* initialize input layer */
    io_init(fin);
    /* initialize output layer */
    emit_init(fout);
    /* start the parser */
    parse_file();
    /* get final error code */
    ret = get_err();
    /* close the files */
    fclose(fin);
    fclose(fout);
    /* done */
    return ret;
}

