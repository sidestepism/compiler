#include "tokenizer.h"

int main(int argc, char ** argv){
  tokenizer_t t = mk_tokenizer(argv[1]);
  while(cur_tok(t).kind != TOK_EOF){
    output_token(t);
    next_tok(t);
  }
  return 0;
}
