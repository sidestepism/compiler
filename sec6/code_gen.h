#pragma once
#include "parse.h"
#define WILDCARD -1

typedef struct oprd_list *oprd_list_t;
typedef struct operand *oprd_t;
typedef struct env * env_t;
typedef struct operand * oprd_t;

typedef enum{
  litr,
  reg,
  fix,
} oprd_kind;

struct operand{
  char* opr; //レジスタ名
  int reg; //レジスタオフセット
  oprd_kind kind;
  char name[1000];
  oprd_t prev;
};

struct env{
  oprd_list_t local;
  oprd_t scope_limit;
  oprd_t local_last;
  oprd_t global_last;
};

typedef struct inst{
  op_kind_t kind;
  oprd_t left;
  oprd_t right;
} *ins_t;

void pr_ind(ins_t i);
oprd_t cogen_expr(FILE *fp, expr_t e, env_t env);
int cogen_decl(FILE *fp, expr_t e, env_t env);
oprd_t cogen_expr_int_literal(FILE* fp, expr_t e, env_t env);
oprd_t cogen_expr_id(FILE *fp, expr_t e, env_t env);
void synerror(FILE *fp, expr_t e, env_t env);
oprd_t cogen_expr_paren(FILE *fp, expr_t e, env_t env);
oprd_t cogen_expr_app(FILE *fp, expr_t e, env_t env);


oprd_list_t mk_oprd_list();
void oprd_list_add(oprd_list_t l, oprd_t x);
oprd_t oprd_list_get(oprd_list_t l, int i);
int oprd_list_sz(oprd_list_t l);




