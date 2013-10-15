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
stmt_t parse_stmt_continue(tokenizer_t);
stmt_t parse_stmt_break(tokenizer_t);
stmt_t parse_stmt_return(tokenizer_t);
stmt_t parse_stmt_expr(tokenizer_t);
var_decl_t pars_decl(tokenizer_t t);
