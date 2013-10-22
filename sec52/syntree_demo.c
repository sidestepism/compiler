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

  tokenizer_t t = mk_tokenizer(argv[1]);
  program_t W;
  W = parse_program(t);

  pr_program(out, W);

  // /* while (x < y) { S } */
  // stmt_t W = mk_stmt_while(f, l, 
		// 	   mk_expr_bin_op(f, l, op_kind_lt, mk_expr_id(f, l, "x"), mk_expr_id(f, l, "y")),
		// 	   mk_stmt_compound(f, l, mk_var_decl_list(), S));
  // pr_stmt(stdout, W);
  return 0;
}
