// char_buf.h
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntree.h"

#define REGISTER 1
#define MEMORY 0

/* 構文木情報のリスト */
typedef struct syntree_info_list * syntree_info_list_t;

/* 構文木情報のリスト関係の関数 */
syntree_info_list_t mk_syntree_info_list();  /* 空のリストを作るコンストラクタ */
void syntree_info_list_add(syntree_info_list_t l, syntree_info_t x); /* lにxを追加 */
syntree_info_t syntree_info_list_get(syntree_info_list_t l, int i);  /* lのi番目の要素 */
int syntree_info_list_sz(syntree_info_list_t l);      /* lの要素数 */



typedef enum{
  reg_eax,
  reg_ebx,
  reg_ecx,
  reg_edx,
  reg_edi,
  reg_esi,
  reg_esp,
  reg_ebp,
  reg_eip,
  reg_eflags
} reg_t;


typedef enum var_kind{
  var_kind_imm,
  var_kind_reg,
  var_kind_memory,
} var_kind_t;

// 生成された時点では親情報は持っていない
typedef struct env {
    // 上のブロックのenv
    env_t parent;
    syntree_info_list_t info_list;
    int decl_ptr;
    int param_ptr;
} env;

typedef struct syntree_info {
    char* name;
    var_kind_t kind;
    reg_t reg;
    int offset; // var_memory の場合
    int val; // var_imm の場合
} syntree_info;

env_t mk_env();
env_t scan_syntree_program(program_t prog);
env_t scan_syntree_fun_def(fun_def_t fun_def, env_t env_global);
env_t scan_syntree_decls(var_decl_list_t decls, env_t env);
env_t scan_syntree_params(var_decl_list_t decls, env_t env);
env_t scan_syntree_stmt(stmt_t s, env_t p_env);
env_t scan_syntree_expr(expr_t e, env_t env);
void free_env (env_t* buf);

syntree_info_t env_add_syntree_info(env_t env, char* name, var_kind_t kind, reg_t reg, int offset);
syntree_info_t mk_syntree_info_imm(env_t env, int val);
syntree_info_t mk_syntree_info_id(env_t env, char* name);

