#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF  4096

char cur_line[MAX_BUF] = "";
int line_size = 0;
int off = 0;

char *file_name = "dumb.c";
int lineno = 0;
int col = 0;
int last_col = 0;

FILE *fd;
int has_ungetc = 0; /* unget char */
int last_chr;

void io_init(FILE *fin) {
    fd = fin;
}

char *get_cur_line() {
    return cur_line;
}

int get_off() {
    return off;
}

char *get_file_name() {
    return file_name;
}

int get_lineno() {
    return lineno;
}

int get_col() {
    return col;
}

int get_char() {
    int ret = 0, i;
    char *eptr;
    if (line_size == off) {
        /* we need to read new line */
        if (!fgets(cur_line, MAX_BUF-1, fd)) {
            /* EOF */
            ret = EOF;
        } else {
            /* hash lines have special treatment */
            if (cur_line[0] == '#') {
                /* parse hash line */
                lineno = strtol(&cur_line[2], &eptr, 10)-1;
                /* get file name */
                file_name = malloc(1024);
                for (i = 2; eptr[i] != '"'; i++) {
                    file_name[i-2] = eptr[i];
                }
                file_name[i] = 0;
                /* get next token */
                ret = get_char();
            } else {
                line_size = strlen(cur_line);
                last_col = col;
                off = 1;
                col = 1;
                lineno++;
                ret = cur_line[0];
            }
        }
    } else {
        ret = cur_line[off];
        off++;
        col++;
    }
    return ret;
}

int next_char() {
    if (has_ungetc == 1) {
        has_ungetc = 0;
        col++;
    } else {
        last_chr = get_char();
    }
    return last_chr;

}

void unread_char() {
    col--;
    if (col == 0) {
        col = last_col;
        lineno--;
    }
    has_ungetc = 1;
}

