#include "syntree.h"
#include "tokenizer.h"
#include "parse.h"
#include "code_gen.h"
#include "list.h"


oprd_t cogen_expr(FILE *fp, expr_t e, env_t env){
  switch(e->kind){
    case expr_kind_int_literal:
      return cogen_expr_int_literal(fp, e, env);
    case expr_kind_id:
      return cogen_expr_id(fp, e, env);
    case expr_kind_paren:
      return cogen_expr(fp, e, env);
    case expr_kind_app:
      return cogen_expr_app(fp, e, env);
    default:
      synerror(fp, e, env);
  }
}

int cogen_decl(FILE *fp, expr_t e, env_t env){
  oprd_t cur_local;
  for(cur_local = env->local_last; cur_local && cur_local != env->scope_limit; cur_local = cur_local->prev){
    //変数宣言してる時点で大域変数は関係ない
    //同じスコープで同じ変数を使っていたら、それに上書きする仕様
    if(cur_local->name == e->u.s) {
      return 0;
    }
  }
  oprd_t o = (oprd_t)malloc(sizeof(struct operand));
  strcpy(o->name, e->u.s);
  o->reg = env->local_last->reg+8;
  sprintf(o->opr, "%d", o->reg);
  o->prev = env->local_last;
  return 0;
}

oprd_t cogen_expr_int_literal(FILE* fp, expr_t e, env_t env){//即値
  struct operand o;
  o.kind = litr;
  oprd_t imd = &o;
  imd->opr = strdup("$");
  strcpy(imd->opr+1,  e->u.s);
  printf("%s\n", imd->opr);
  return imd;
}

oprd_t cogen_expr_id(FILE *fp, expr_t e, env_t env){
  printf("id\n");
  struct operand cur;
  cur.kind = reg;
  oprd_t cur_var = &cur;
  printf("%s\n", env->local_last->name);
  for(cur_var = env->local_last; cur_var != NULL && cur_var != env->scope_limit; cur_var = cur_var->prev){//local変数にあるか確認
    printf("%s\n", cur_var->name);
    if(!strcmp(cur_var->name, e->u.s)){
      printf("re\n");
      return cur_var;//変数のアドレスを入れたレジスタを返す
    }
  }
  for(cur_var =  env->global_last; cur_var; cur_var = cur_var->prev){
    if(!strcmp(cur_var->name, e->u.s)) return cur_var;
  }//グローバル変数にあるか確認
  synerror(fp, e, env);//なければエラー
}

void synerror(FILE *fp, expr_t e, env_t env){
  printf("syntax error\n");
  printf("%s\n", e->u.s);
  exit(1);
}

oprd_t cogen_expr_paren(FILE *fp, expr_t e, env_t env){
  return cogen_expr(fp, e->u.p, env);
}

oprd_t cogen_expr_app(FILE *fp, expr_t e, env_t env){
  //これが命令の元をつくる
  struct inst ins;
  oprd_t ret;
  switch(e->u.a.o){
    case op_kind_none:
      return;
    case op_kind_fun:/* 組み込み演算子ではない, 関数 */
      printf("call %s", e->u.a.f);
      return; //@TODO eaxのoprdを返す
    case op_kind_assign:/* a = b 2項だから命令とvar返す */
      ins.kind = op_kind_assign;
      ins.left = cogen_expr(fp, expr_list_get(e->u.a.args, 0), env); //left
      ins.right = cogen_expr(fp, expr_list_get(e->u.a.args, 1), env);
      pr_ins(&ins);
      return ins.left;
    case op_kind_eq:
    case op_kind_neq:
    case op_kind_lt:
    case op_kind_gt:
    case op_kind_le:
    case op_kind_ge:/* a == b 2項だから命令とopr返す*/
      ins.kind = e->u.a.o;//cmpl
      ins.left = cogen_expr(fp, expr_list_get(e->u.a.args, 0), env);
      ins.right = cogen_expr(fp, expr_list_get(e->u.a.args, 1), env);
      pr_ins(&ins);
      return;
    case op_kind_bin_plus:
    case op_kind_bin_minus:
    case op_kind_mult:
    case op_kind_div:
    case op_kind_rem:/* a + b 2項だから命令とopr返す*/
      ins.kind = e->u.a.o;
      FILE *out;
      pr_expr(out, expr_list_get(e->u.a.args, 0));
      printf("\n");
      ins.left = cogen_expr(fp, expr_list_get(e->u.a.args, 0), env);
      printf("%s cogen kind %d\n", ins.left->opr, ins.left->kind);
      printf(" %s\n", ins.left->opr);
      pr_expr(out, expr_list_get(e->u.a.args, 0));
      printf("\n");
      ins.right = cogen_expr(fp, expr_list_get(e->u.a.args, 1), env);
      printf("%s cogen kind %d\n", ins.left->opr, ins.left->kind);
      printf("%s cogen kind %d\n", ins.right->opr, ins.right->kind);
      pr_ins(&ins);
      if(ins.left->kind == fix) return ins.left;
      else{
        struct operand o;
        o.kind = fix;
        o.opr = strdup("fix");
        oprd_t reg = &o;
        return reg;
      }
      /*5+5, x+5, 5+x*/
    case op_kind_un_minus:
      /*1項だからoprだけ返す*/
      printf("movl $0 fix\n");
      printf("subl %s fix\n", ins.right->opr);
      ret->kind = fix;
      strcpy(ret->name, "fix");
      return ret;
      /*case op_kind_un_plus:*/
    case op_kind_logneg:
      ret = cogen_expr(fp, expr_list_get(e->u.a.args, 0), env);
      printf("cmpl $0 %s\n", ret->opr);
      printf("je\n");
      return;
  }
}

/* 5 * -f(1,3,4) */
oprd_list_t mk_oprd_list()
{
    return (oprd_list_t)mk_list();
}

/* 式のリストに式を追加する */
void oprd_list_add(oprd_list_t l, oprd_t x)
{
    list_add((list_t)l, (void *)x);
}

/* 式のリストのi番目の要素を返す */
oprd_t oprd_list_get(oprd_list_t l, int i)
{
    return (oprd_t)list_get((list_t)l, i);
}

/* 式のリストの長さを返す */
int oprd_list_sz(oprd_list_t l)
{
    return list_sz((list_t)l);
}


void pr_ins(ins_t i){
  switch(i->kind){
    //case op_kind_none:
    case op_kind_fun:
      //forで引数
      printf("call %s\n", i->right->opr);
      break;
    case op_kind_assign:
      printf("movl %s %s\n", i->right->opr, i->left->opr);
      break;
    case op_kind_eq:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("je");
      break;
    case op_kind_neq:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("jne");
      break;
    case op_kind_lt:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("jl");
      break;
    case op_kind_gt:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("jg");
      break;
    case op_kind_le:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("jle");
      break;
    case op_kind_ge:
      printf("cmpl %s %s\n", i->right->opr, i->left->opr);
      printf("jge");
      break;
    case op_kind_bin_plus:
      printf("print時点でのkind %d\n", i->left->kind);
      if(i->left->kind == fix) printf("addl %s fix\n", i->right->opr);
      else {
        printf("movl %s fix\n", i->left->opr);
        printf("addl %s fix\n", i->right->opr);
      }
      break;
    case op_kind_bin_minus:
      if(i->right->kind == fix) {
        printf("movl %s fix", i->left->opr);
        printf("subl fix fix\n");
      }
      else {
        printf("movl %s fix\n", i->left->opr);
        printf("subl %s fix\n", i->right->opr);
      }
      break;
    case op_kind_mult:
      if(i->right->kind == fix) printf("mull %s fix\n", i->right->opr);
      else {
        printf("movl %s fix\n", i->left->opr);
        printf("mull %s fix\n", i->right->opr);
      }
      break;
    case op_kind_div:
      printf("idivl %s\n", i->right->opr);
      break;
    case op_kind_rem:
      printf("idivl %s\n", i->right->opr);
      break;
    /*case op_kind_un_plus:
    case op_kind_un_minus:
      printf("subl $0 %s\n", i->right->opr);
    case op_kind_un_logneg:
      printf("cmpl $0 %s\n", i->right->opr);
      printf("jl");*/
  }
}
