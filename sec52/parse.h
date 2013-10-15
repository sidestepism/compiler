#pragma once
#include "tokenizer.h"

void eat_it(tokenizer_t t, token_kind_t tok_kind);
expr_t parse_unary_expr(tokenizer_t t);
expr_t parse_expr(tokenizer_t t);
stmt_t parse_stmt(tokenizer_t t);
stmt_t parse_stmt_while(tokenizer_t t);
stmt_t parse_stmt_if(tokenizer_t t);
stmt_t parse_stmt_empty(tokenizer_t t);
stmt_t parse_stmt_compound(tokenizer_t);
