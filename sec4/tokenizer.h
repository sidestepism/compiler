// tokenizer.h
#pragma once

typedef enum {
	tok_eof,
	tok_wspace,
	tok_int,
	tok_plus,
	tok_minus,
	tok_undef,
	tok_newline
} token_kind_t;

typedef struct token
{
	token_kind_t kind;
	int ival;
} token_t;

typedef struct tokenizer
{
	struct token tok;
	int c;
	int line;
	int ch;
	int byte;
	FILE * fp;
} * tokenizer_t;


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
  TOK_COMMA
} token_kind_t;
 
tokenizer_t mk_tokenizer(char * filename);

struct token cur_tok(tokenizer_t t);

tokenizer_t next_tok(tokenizer_t t);

void syntax_error(tokenizer_t tok);

