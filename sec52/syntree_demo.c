/* 
 * syntree_main.c
 */

/* 構文木をちょっと作って表示するだけの
   プログラム */

#include "syntree.h"
#include "parse.h"
#include "tokenizer.h"


int main(int argc, char ** argv)
{
  // /* 例えば while (i < n) { p = p * i; i = i + 1; } */
  // char * f = "p.c"; int l = 10;
  // /* S0 : p = p * i */
  // stmt_t S0 = mk_stmt_expr(f, l, mk_expr_bin_op(f, l, op_kind_assign,
		// 				mk_expr_id(f, l, "p"), 
		// 				mk_expr_bin_op(f, l, op_kind_mult, 
		// 					       mk_expr_id(f, l, "p"), 
		// 					       mk_expr_id(f, l, "i"))));
  //  S1 : i = i + 1 
  // stmt_t S1 = mk_stmt_expr(f, l, mk_expr_bin_op(f, l, op_kind_assign,
		// 				mk_expr_id(f, l, "i"), 
		// 				mk_expr_bin_op(f, l, op_kind_bin_plus, 
		// 					       mk_expr_id(f, l, "i"), 
		// 					       mk_expr_int_literal(f, l, "1"))));
  // stmt_list_t S = mk_stmt_list();
  // stmt_list_add(S, S0);
  // stmt_list_add(S, S1);

  tokenizer_t t = mk_tokenizer(argv[1]);
  stmt_t W;
  while(cur_tok(t).kind != TOK_EOF){
    W = parse_stmt(t);
  }

  pr_stmt(stdout, W);


  return 0;

  // /* while (x < y) { S } */
  // stmt_t W = mk_stmt_while(f, l, 
		// 	   mk_expr_bin_op(f, l, op_kind_lt, mk_expr_id(f, l, "x"), mk_expr_id(f, l, "y")),
		// 	   mk_stmt_compound(f, l, mk_var_decl_list(), S));
  // pr_stmt(stdout, W);
  return 0;
}
