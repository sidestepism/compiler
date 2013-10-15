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
	t->ch = 0;
	t->line = 1;
	t->byte = 0;
	
	t->c = fgetc(t->fp);
	next_tok(t);
	// read the first token

	return t;
}

struct token cur_tok(tokenizer_t t){
	return t->tok;
}



void show_tok_kind(tokenizer_t t){
	switch(t->tok.kind){
		case tok_eof:
			printf("tok_eof\n");
		break;
		case tok_wspace:
			printf("tok_wspace\n");
		break;
		case tok_int:
			printf("tok_int\n");
		break;
		case tok_plus:
			printf("tok_plus\n");
		break;
		case tok_minus:
			printf("tok_minus\n");
		break;
		case tok_newline:
			printf("tok_newline\n");
		break;
		case tok_undef:
			printf("tok_undef\n");
		break;
	}
}

char next_char(tokenizer_t t){
	t->ch ++;
	t->byte ++;
	return fgetc(t->fp);
}

tokenizer_t next_tok(tokenizer_t t){
	// eat all continuous wspaces
	while(t->c == ' '){
		t->c = next_char(t);
	}

	if(t->c == EOF){
		t->tok.kind = tok_eof;
	}else if(t->c == '\n'){
		t->line ++;
		t->ch = 0;
		t->tok.kind = tok_newline;
		t->c = next_char(t);
	}else if(t->c >= '0' && t->c <= '9'){
		int val = 0;
		do{
			val = val * 10 + (t->c - '0');
			t->c = next_char(t);
			// if(c >= '0' && c <= '9') printf("this is digit!\n");
		}while(t->c >= '0' && t->c <= '9');
		t->tok.kind = tok_int;
		t->tok.ival = val;
	}else if(t->c == '+'){
		t->tok.kind = tok_plus;
		t->c = next_char(t);
	}else{
		t->tok.kind = tok_undef;
		t->c = next_char(t);
	}
	// show_tok_kind(t);
	return t;
}


void syntax_error(tokenizer_t t){
	printf("syntax error on line %d char %d \n", t->line, t->ch);
	printf("saw '%c'\n", t->c);
	show_tok_kind(t);

	// ここから エラーの位置を表示
	int err = fseek(t->fp, 0, SEEK_SET);
	int i = t->line;

	int c;
	while(i-- > 1){
		do{
			c = fgetc(t->fp);
		} while(c != '\n' && c != EOF);
	}
	// 問題の行に辿り着いた

	while(1){
		c = fgetc(t->fp);
		if(c == '\n' || c == EOF) break;
		printf("%c", c);
	}
	printf("\n");

	for(i = 0; i < t->ch - 2; i++){
		printf(" ");
	}
	printf("^\n");

	exit(1);
}

