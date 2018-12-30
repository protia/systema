#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

#define STATE_START     0
#define STATE_CHAR      1
#define STATE_STRING    2
#define STATE_OP        3
#define STATE_ALPHA     4
#define STATE_NUM       5
#define STATE_OCT       6
#define STATE_HEX       7
#define STATE_EOF       8

int has_ungetl = 0; /* unget lexeme   */
lexeme_t lex;       /* current lexeme */
int state = 0;

char *ops[] = {
    "[",
    "]", 
    "(",
    ")", 
    "{",
    "}", 
    ";",
    ".",
    ",",
    "..", 
    "...", 
    "++",
    "--",
    "&",
    "@",
    "*",
    "+",
    "-",
    "~",
    "!",
    "/",
    "%",
    "<<",
    ">>",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "^",
    "|",
    "?",
    ":",
    ":=",
    "*=",
    "/=",
    "%=",
    "+=",
    "-=",
    "<<=",
    ">>=",
    "&=",
    "^=",
    "|=" ,
    "="
};

char *keywords[] = {
    "def",
    "dec",
    "typ",
    "sub",
    "local",
    "as",
    "in",
    "code",
    "rodata",
    "data",
    "bss",
    "stack",
    "register",
    "byte",
    "twin",
    "four",
    "eigh",
    "func",
    "text",
    "record",
    "returns",
    "begin",
    "assembly",
    "sizeof",
    "typeof",
    "if",
    "unsigned",
    "and",
    "or",
    "then",
    "elsif",
    "else",
    "endif",
    "case",
    "when",
    "others",
    "esac",
    "for",
    "from",
    "down",
    "to",
    "step",
    "do",
    "next",
    "while",
    "loop",
    "return",
    "end",
};

int isop(char *str) {
    int i = 0;
    int found = 0;
    for (i = 0; i < sizeof(ops)/sizeof(ops[i]) && !found; i++) {
        if (!strcmp(ops[i], str)) {
            found = 1;
        }
    }
    return found;
}

int iskeyword(char *str) {
    int i = 0;
    int found = 0;
    for (i = 0; i < sizeof(keywords)/sizeof(keywords[i])&&!found; i++) {
        if (!strcmp(keywords[i], str)) {
            found = 1;
        }
    }
    return found;
}

int isnum(char chr) {
    return chr >= '0' && chr <= '9';
}

int isoct(char chr) {
    return chr >= '0' && chr <= '7';
}

int ishex(char chr) {
    return (chr >= '0' && chr <= '9')||
           (chr >= 'a' && chr <= 'f')||
           (chr >= 'A' && chr <= 'F');
}

int isnondigit(char chr) {
    return (chr >= 'a' && chr <= 'z') ||
           (chr >= 'A' && chr <= 'Z') ||
           (chr == '_');
}

void get_lexeme() {
    int pos = 0;
    int chr;
    int f = 0;
    int escape = 0;
    if (has_ungetl) {
        has_ungetl = 0;
    } else {
        /* get next character */
        chr = next_char();
        /* white space? */
        while (isspace(chr)) {
            chr = next_char();
        }
        /* lexeme contains at least this first char */
        lex.val[0] = chr;
        lex.val[1] = 0;
        /* reset f */
        f = 0;
        /* start at unknown state */
        state = STATE_START;
        /* loop over input characters to read full lexeme */
        while(!f /* not done yet */) {
            /* check chr */
            switch (state) {
                case STATE_START:
                    if (chr == '\'') {
                        /* read character */
                        state = STATE_CHAR;
                    } else if (chr == '\"') {
                        /* read string */
                        state = STATE_STRING;
                    } else if (isop(lex.val)) {
                        /* operator */
                        state = STATE_OP;
                    } else if (isnum(chr)) {
                        /* numeric */
                        state = STATE_NUM;
                    } else if (isnondigit(chr)) {
                        /* alphabetic */
                        state = STATE_ALPHA;
                    } else if (chr == EOF) {
                        /* end of file */
                        state = STATE_EOF;
                    } else {
                        /* invalid token */
                        f = 1;
                    }
                    break;
                case STATE_CHAR:
                    if ((pos == 2 && !(lex.val[1] == '\\')) || pos == 3) {
                        if (chr != '\'') {
                            /* invalid token */
                            unread_char();
                            f = 1;
                        } else {
                            /* done */
                            f = 2;
                        }
                    }
                    if (!f && chr == EOF) {
                        /* invalid token */
                        unread_char();
                        f = 1;
                    }
                    break;
                case STATE_STRING:
                    if (chr == '\"' && !escape) {
                        /* done */
                        f = 2;
                    } else if (chr == EOF) {
                        /* invalid token */
                        unread_char();
                        f = 1;
                    } else if (chr == '\n') {
                        /* invalid token */
                        unread_char();
                        f = 1;
                    }
                    if (escape) {
                        escape = 0;
                    } else if (chr == '\\') {
                        escape = 1;
                    }
                    break;
                case STATE_OP:
                    /* append char and test */
                    lex.val[pos] = chr;
                    lex.val[pos+1] = 0;
                    if (!isop(lex.val)) {
                        /* done */
                        unread_char();
                        f = 2;
                    }
                    break;
                case STATE_ALPHA:
                    if (!isnondigit(chr) && !isnum(chr)) {
                        /* done */
                        unread_char();
                        f = 2;
                    }
                    break;
                case STATE_NUM:
                    if (!isnum(chr)) {
                        if ((chr == 'o' || chr == 'O') &&
                            pos == 1 && lex.val[0] == '0') {
                            /* octal number */
                            state = STATE_OCT;
                        } else if ((chr == 'x' || chr == 'X') &&
                            pos == 1 && lex.val[0] == '0') {
                            /* hexadecimal number */
                            state = STATE_HEX;
                        } else {
                            /* done */
                            unread_char();
                            f = 2;
                        }
                    }
                    break;
                case STATE_OCT:
                    if (!isoct(chr)) {
                        unread_char();
                        if (pos > 2) {
                            /* done */
                            f = 2;
                        } else {
                            /* invalid token */
                            unread_char();
                            f = 1;
                        }
                    }
                    break;
                case STATE_HEX:
                    if (!ishex(chr)) {
                        unread_char();
                        if (pos > 2) {
                            /* done */
                            f = 2;
                        } else {
                            /* invalid token */
                            unread_char();
                            f = 1;
                        }
                    }
                    break;
                case STATE_EOF:
                    /* done */
                    f = 2;
                    break;
            }
            /* what's next? */
            if (f == 0) {
                /* store chr and move to next */            
                lex.val[pos++] = chr;
                chr = next_char();
            } else if (f == 1) {
                /* invalid token */
                if (pos) {
                    lex.val[pos] = 0;
                } else {
                    pos = 0;
                }
                lex.type = LEX_INVALID;
                print_err("invalid token: %s", lex.val);
            } else {
                /* append to token if string or char */
                if (state == STATE_CHAR || state == STATE_STRING) {            
                    lex.val[pos++] = chr;                
                } else if (state == STATE_EOF) {
                    lex.val[0] = 0;
                }
                /* done */
                lex.val[pos] = 0;
                /* determine type of lexeme */
                switch(state) {
                    case STATE_CHAR:
                        lex.type = LEX_CHAR_LITERAL;
                        break;
                    case STATE_STRING:
                        lex.type = LEX_STR_LITERAL;
                        break;
                    case STATE_OP:
                        lex.type = LEX_OP;
                        break;
                    case STATE_ALPHA:
                        if (iskeyword(lex.val)) {
                            lex.type = LEX_KEYWORD;
                        } else {
                            lex.type = LEX_IDENTIFIER;
                        }
                        break;
                    case STATE_NUM:
                    case STATE_OCT:
                    case STATE_HEX:
                        lex.type = LEX_INT_LITERAL;
                        break;
                    case STATE_EOF:
                        lex.type = LEX_EOF;
                        break;
                }
            }
        }
    }
}

void unget_lexeme() {
    has_ungetl = 1;
}

