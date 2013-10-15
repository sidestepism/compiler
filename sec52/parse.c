#include "syntree.h"
#include "tokenizer.h"

#include "parse.h"


void eat_it(tokenizer_t t, token_kind_t tok){

}

void parse_expr(tokenizer_t t){

}

void parse_stmt(tokenizer_t t){

}

void parse_stmt_while(tokenizer_t t)
{
    eat_it(t, TOK_WHILE);
    eat_it(t, TOK_LPAREN);
    parse_expr(t);
    eat_it(t, TOK_RPAREN);
    parse_stmt(t);
}
