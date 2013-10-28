// char_buf.h
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct char_buf {
	char* body;
	int length;
	int maxlength;
} char_buf_t;

char_buf_t* mk_char_buf();

void push_charbuf (char_buf_t* buf, char c);
char* get_body_charbuf (char_buf_t* buf);
char* get_copy_charbuf (char_buf_t* buf);
int get_length_charbuf (char_buf_t* buf);
void free_charbuf (char_buf_t* buf);
void clear_charbuf (char_buf_t* buf);
