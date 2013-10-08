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

 
tokenizer_t mk_tokenizer(char * filename);

struct token cur_tok(tokenizer_t t);

tokenizer_t next_tok(tokenizer_t t);

void syntax_error(tokenizer_t tok);

