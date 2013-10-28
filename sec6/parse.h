#pragma once
#include "tokenizer.h"
#define WILDCARD -1


void eat_it(tokenizer_t t, token_kind_t tok_kind);
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

expr_t parse_eq_expr(tokenizer_t t);
expr_t parse_relat_expr(tokenizer_t t);
expr_t parse_add_expr(tokenizer_t t);
expr_t parse_mult_expr(tokenizer_t t);
expr_t parse_intlit_expr(tokenizer_t t);
expr_t parse_id_expr(tokenizer_t t);
expr_t parse_paren_expr(tokenizer_t t);
expr_t parse_op_expr(tokenizer_t t);
expr_t parse_unary_expr(tokenizer_t t);
expr_t parse_expr(tokenizer_t t);

var_decl_t parse_decl(tokenizer_t t);
var_decl_t parse_param(tokenizer_t t);
fun_def_t parse_fun_def(tokenizer_t t);
program_t parse_program(tokenizer_t t);

expr_list_t parse_arg_expr(tokenizer_t t);

void expr_list_add(expr_list_t l, expr_t x);
