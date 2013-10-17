// tokenizer.h
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "char_buf.h"


typedef enum{
  TOK_INT,
  TOK_BREAK,
  TOK_CONTINUE,
  TOK_ELSE,
  TOK_IF,
  TOK_RETURN,
  TOK_WHILE,
  TOK_INT_LITERAL,
  TOK_ID,
  TOK_EOF,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_MUL,
  TOK_PLUS,
  TOK_MINUS,
  TOK_DIV,
  TOK_REM,
  TOK_BANG,
  TOK_LT,
  TOK_GT,
  TOK_LE,
  TOK_GE,
  TOK_EQ,
  TOK_NEQ,
  TOK_SEMICOLON,
  TOK_ASSIGN,
  TOK_COMMA,
} token_kind_t;

typedef struct token{
  token_kind_t kind;
  int ival;
  char name[1024];
  char_buf_t* literal;
} token;

typedef struct tokenizer{
  token tok;
  FILE *fp;
  int c;
  int line;
  int num;
  char* filename;
  char_buf_t* line_buf;
  char_buf_t* token_buf;
} *tokenizer_t;


void syntax_error(tokenizer_t t, char *msg);
tokenizer_t tokenize(tokenizer_t t);
tokenizer_t mk_tokenizer(char *filename);
token cur_tok(tokenizer_t t);
token next_tok(tokenizer_t t);
void output_token(tokenizer_t t);
void output_token_kind(token_kind_t kind);
int next_char(tokenizer_t t);
void line_break(tokenizer_t);
