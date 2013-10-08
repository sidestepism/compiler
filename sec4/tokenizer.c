#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

tokenizer_t mk_tokenizer(char * filename)
{
	tokenizer_t t = (tokenizer_t) malloc (sizeof(struct tokenizer));
	FILE * fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("error: failed to open %s\n", filename);
		exit(1);
	}
	t->fp = fp;
	t->c = 0;

//	next_tok(t);
	// read the first token

	return t;
}

token cur_tok(tokenizer_t t){
	return t->tok;
}

token next_tok(tokenizer_t t){
	int c = fgetc(t->fp);
	token_t tok = (token_t) malloc(sizeof(token));
 
	// eat all continuous wspaces
	while(c == ' '){
		int c = fgetc(t->fp);
	}

	if(c == EOF){
		tok.kind = tok_eof;
		t->c ++;
	}else if(c == '\n'){
		tok.kind = tok_newline;
		t->c ++;
	}else if(c >= '0' && c <= '9'){
		int val = 0;
		do{
			val = val * 10 + (c - '0');
			t->c ++;
			int c = fgetc(t->fp);
		}while(c >= '0' && c <= '9')
		tok.kind = tok_int;
		tok.ival = val;
	}else if(c == '+'){
		tok.kind = tok_plus;
		t->c ++;
	}else{
		tok.kind = tok_undef;
		t->c ++;
	}
	return tok;
}

void syntax_error(tokenizer_t tok){
	printf("syntax_error!\n");
}

int eat_int(tokenizer_t t){
	token tok = cur_tok(t);
	if (tok.kind != tok_int){
		syntax_error(t);
	}
	next_tok(t);
	return tok.ival;
}

int eat_plus(tokenizer_t t){
	token tok = cur_tok(t);
	if (tok.kind != tok_plus){
		syntax_error(t);
	}
	next_tok(t);
}

int main(int argc, char ** argv){
	tokenizer_t t = mk_tokenizer(argv[1]);
	while(cur_tok(t).kind != tok_eof){
		int x = eat_int(t);
		while(cur_tok(t).kind != tok_newline){
			eat_plus(t);
			int y = eat_int(t);
			x = x + y;
		}
		next_tok(t);
		printf("%d\n", x);
		return 0;
	}
}