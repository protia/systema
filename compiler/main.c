#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[]) {
    char *if_name = argv[1];
    char *of_name = argv[2];
    int ret;
    FILE *fin = fopen(if_name, "r");
    FILE *fout = fopen(of_name, "w");
    if (!fin) {
        fprintf(stderr, "Can't open %s for read.\n", if_name);
        return -1;
    }
    if (!fout) {
        fprintf(stderr, "Can't open %s for write.\n", of_name);
        return -1;
    }
    io_init(fin);
    emit_init(fout);
    parse_file();
    fclose(fin);
    fclose(fout);
    ret = get_err();
    return ret;
}

