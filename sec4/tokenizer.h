// tokenizer.h


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
	int line;
	int ch;
} token_t;

typedef struct tokenizer
{
	token tok;
	int c;
	FILE* fp;
} * tokenizer_t;
 
tokenizer_t mk_tokenizer(char * filename);
token cur_tok(tokenizer_t t);
token next_tok(tokenizer_t t);
token syntax_error(token tok);
