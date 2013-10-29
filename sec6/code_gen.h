#pragma once
#include "parse.h"
#include "env.h"
#define WILDCARD -1


void cogen_program(FILE *fp, program_t ds);
void cogen_fun_def(FILE *fp, fun_def_t f);
void cogen_stmt(FILE *fp, stmt_t s);
void cogen_expr(FILE *fp, expr_t e);

char* cogen_addr(syntree_info_t info);
char* cogen_pr_reg(reg_t reg);
char* cogen_mov(FILE* fp, expr_t right, expr_t left);
char* cogen_mov_reg(FILE *fp, expr_t e, reg_t reg);

char* get_label();