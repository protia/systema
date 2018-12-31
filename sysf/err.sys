#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

int err = 0;

void print_err(char *err_msg, char *data) {
    char *cur_line = get_cur_line();
    int i, count;
    err = 1;
    printf("%s:L%d: Error: ", get_file_name(), get_lineno());
    printf(err_msg, data);
    printf("\n");
    printf("%s", cur_line);
    count = get_col()-strlen(lex.val);
    if (cur_line[strlen(cur_line)-1] != '\n') {
        printf("\n");
        if (count > strlen(cur_line))
            count = strlen(cur_line);
    } else {
        if (count > strlen(cur_line)-1)
            count = strlen(cur_line)-1;
    }
    for (i = 0; i < count; i++) {
        printf(" ");
    }
    printf("^\n");
}

int get_err() {
    int ret;
    if (err) {
        ret = -1;
    } else {
        ret = 0;
    }
    return ret;
}

