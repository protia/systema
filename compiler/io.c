#include <stdio.h>
#include <string.h>

/* low-level I/O with systema file */

#define MAX_BUF  4096

char cur_line[MAX_BUF] = "";
int line_size = 0;
int off = 0;

char file_name[MAX_BUF] = "dumb.c";
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
    int eof = 0;
    if (line_size == off) {
        /* we need to read new line */
        if (!fgets(cur_line, MAX_BUF-1, fd)) {
            /* EOF */
            eof = 1;
        } else {
            line_size = strlen(cur_line);
            last_col = col;
            off = 0;
            col = 0;
            lineno++;
        }
    }
    /* read from line buffer */
    if (!eof) {
        off++;
        col++;
        return cur_line[off-1];
    } else {
        return EOF;
    }
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
