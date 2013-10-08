#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
  tok_int,
  tok_nl,
  tok_plus,
  tok_eof,
  tok_other
} token_kind_t;

typedef struct token{
  token_kind_t kind;
  int ival;
} token;

typedef struct tokenizer{
  token tok;
  FILE *fp;
  int c;
  int line;
  int num;
  char error[50];
} *tokenizer_t;

void syntax_error(tokenizer_t t){
  int c = t->c;
  FILE *fp = t->fp;
  int num = t->num;
  while(c != '\n'){
    t->error[t->num] = c;
    c = fgetc(fp);
    t->num++;
  }
  printf("syntax error :%d:%d\n%s\n", t->line, num, t->error);
  int i;
  for(i = 0; i < num-1; i++){printf(" ");}
  printf("^\n");
  exit(1);
}

tokenizer_t tokenize(tokenizer_t t){
  int i = 0, c = t->c;
  char a[10];
  FILE *fp = t->fp;
  if (t->num > 50) syntax_error(t);
  while (c == ' '){
    t->error[t->num] = c;
    t->num++;
    c = fgetc(fp);
  }
  if (!isdigit(c)){
    //printf("%c, %d\n", c, t->num-1);
    t->error[t->num] = c;
    //printf("%s\n", t->error);
    t->num++;
    switch (c){
      case  '+':
        t->tok.kind = tok_plus;
        break;
      case '\n':
        t->tok.kind = tok_nl;
        t->line++;
        memset(t->error, 0, 50);
        break;
      case EOF:
        t->tok.kind = tok_eof;
        break;
      default:
        t->tok.kind = tok_other;
        break;
    }
    t->c = fgetc(fp);
    t->fp = fp;
    return t;
  }
  while(isdigit(c)){
    //printf("%c, %d\n", c, t->num-1);
    t->error[t->num] = c;
    //printf("%s\n", t->error);
    t->num++;
    if (i== 9) syntax_error(t);
    a[i++] = (char)c;
    c = fgetc(fp);
  }
  t->c = c;
  t->fp = fp;
  t->tok.kind = tok_int;
  a[i] = 0;
  t->tok.ival = atoi(a);
  return t;
}

tokenizer_t mk_tokenizer(char *filename){
  tokenizer_t t = (tokenizer_t)malloc(sizeof(struct tokenizer));
  FILE *fp = fopen(filename, "rb");
  if(fp == NULL){printf("can't open file\n");exit(1);}
  t->c = fgetc(fp);
  t->fp = fp;
  t->line = 1;
  t->num = 0;
  memset(t->error, 0, 50);
  return tokenize(t);
}

token cur_tok(tokenizer_t t){
  return t->tok;
}

token next_tok(tokenizer_t t){
  t = tokenize(t);
  return t->tok;
}

int eat_int(tokenizer_t t){
  token tok = cur_tok(t);
  if(tok.kind != tok_int) syntax_error(t);
  next_tok(t);
  return tok.ival;
}

void eat_plus(tokenizer_t t){
  token tok = cur_tok(t);
  if (tok.kind != tok_plus) syntax_error(t);
  next_tok(t);
}

void reset_num(tokenizer_t t){
  t->num = 0;
}

int main(int argc, char ** argv){
  tokenizer_t t = mk_tokenizer(argv[1]);
  while(cur_tok(t).kind != tok_eof){
    if(cur_tok(t).kind == tok_nl){
      reset_num(t);
      next_tok(t);
    }
    else{
      int x = eat_int(t);
      while(cur_tok(t).kind != tok_nl){
        eat_plus(t);
        int y = eat_int(t);
        x = x + y;
      }
      reset_num(t);
      next_tok(t);
      printf("sum = %d\n", x);
    }
  }
  return 0;
}
