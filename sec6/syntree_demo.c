/* 
 * syntree_main.c
 */

/* 構文木をちょっと作って表示するだけの
   プログラム */

#include "syntree.h"
#include "parse.h"
#include "tokenizer.h"
#include "code_gen.h"

int main(int argc, char ** argv)
{
  // // char_buf のテスト
  // int i;
  // char_buf_t* buf1 = mk_char_buf();
  // for(i = 0; i < 10000; i++){
  //   push_charbuf(buf1, 'a' + i % 10);
  // }
  // printf("%s\n", get_body_charbuf(buf1));
  // printf("%d\n", get_length_charbuf(buf1));
  // clear_charbuf(buf1);
  // free_charbuf(buf1);

  FILE* out;
  if(argc == 1){
    printf("usage: %s [input file] (output file)\n", argv[0]);
    // usage
  }else if(argc == 2){
    out = stdout;
  }else if(argc == 3){
    out = fopen(argv[2], "wb");
  }

  struct env e;
  env_t ep = &e;
  oprd_list_t local = mk_oprd_list();
  struct operand o;
  strcpy(o.name, "x");
  o.reg = 8;
  o.opr = strdup("8");
  oprd_t op = &o;
  o.prev = NULL;
  oprd_list_add(local, op);
  //e.scope_limit = op;
  e.local_last = op;
  e.global_last = op;
  
  tokenizer_t t = mk_tokenizer(argv[1]);
  expr_t W,x;
  W = parse_expr(t);
  pr_expr(out, W);
  printf("\n");  
  printf("%d", W->kind);
  x = expr_list_get(W->u.a.args, 0);
  printf("%d", x->kind);
  pr_expr(out, x);
  printf("\n");

  cogen_expr(out, W, ep); 

  // /* while (x < y) { S } */
  // stmt_t W = mk_stmt_while(f, l, 
		// 	   mk_expr_bin_op(f, l, op_kind_lt, mk_expr_id(f, l, "x"), mk_expr_id(f, l, "y")),
		// 	   mk_stmt_compound(f, l, mk_var_decl_list(), S));
  // pr_stmt(stdout, W);
  return 0;
}
