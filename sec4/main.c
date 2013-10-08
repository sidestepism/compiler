// main.c
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

int eat_int(tokenizer_t t){
    struct token tok = cur_tok(t);
    if (tok.kind != tok_int){
        printf("expected int\n");
        syntax_error(t);
    }
    next_tok(t);
    return tok.ival;
}

int eat_plus(tokenizer_t t){
    struct token tok = cur_tok(t);
    if (tok.kind != tok_plus){
        printf("expected plus\n");
        syntax_error(t);
    }
    next_tok(t);
    return 0;
}

int main(int argc, char ** argv){
    tokenizer_t t = mk_tokenizer(argv[1]);
    while(cur_tok(t).kind != tok_eof){
        if (cur_tok(t).kind == tok_newline){
            next_tok(t);
            continue;
        }
        int x = eat_int(t);
        while(cur_tok(t).kind != tok_newline && cur_tok(t).kind != tok_eof){
            eat_plus(t);
            int y = eat_int(t);
            x = x + y;
        }
        printf("answer: %d\n", x);
        next_tok(t);
    }
    return 0;
}