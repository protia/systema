#include "common.h"

int unsignedf = 0;

int set_unsignedf(int newval) {
    int oldval;
    oldval = unsignedf;
    unsignedf = newval;
    return oldval;
}

void reset_unsignedf(int oldval) {
    unsignedf = oldval;
}

int get_unsignedf() {
    return unsignedf;
}

