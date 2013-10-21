// char_buf.c
#include "char_buf.h"

/**
 * char_buf 型を作ってポインタを返す
 */ 
char_buf_t * mk_char_buf()
{
	// 初期設定
	int length = 32;

	char_buf_t* buf = (char_buf_t *) malloc(sizeof(char_buf_t));
	buf->body = (char *) malloc(sizeof(char) * length);
	buf->length = 0;
	buf->maxlength = length;
	memset(buf->body, 0, buf->maxlength);

	if(buf == NULL){
		printf("error: failed to alloc memory for char_buf\n");
		exit(1);
	}

	return buf;
}

/**
 * char_buf 型に文字列を追加する
 */ 
void push_charbuf (char_buf_t* buf, char c)
{
	if(buf->length + 1 >= buf->maxlength){
		buf->body = (char *)realloc(buf->body, buf->maxlength * 2 * sizeof(char));
		memset(buf->body + buf->maxlength, 0, buf->maxlength * sizeof(char));
		if(buf->body == NULL){
			printf("error: failed to realloc memory for char_buf->body\n");
			exit(1);
		}
	}
	buf->body[buf->length ++] = c;
}

/**
 * char_buf の body を取得
 */
char* get_body_charbuf (char_buf_t* buf)
{
	return buf->body;
}

/**
 * char_buf の copy を取得
 */
char* get_copy_charbuf (char_buf_t* buf)
{
	char* ary = (char*) malloc(buf->length * sizeof(char*) + 1);
	memcpy(ary, buf->body, buf->length + 1);
	return ary;
}


/**
 * char_buf の長さを取得
 */
 int get_length_charbuf (char_buf_t* buf)
 {
 	return buf->length;
 }


/**
 * char_buf のメモリを解放
 */
void free_charbuf (char_buf_t* buf)
{
	free(buf->body);
	free(buf);
}

/**
 * 中身を消去 (memset 0, length = 0)
 */ 
void clear_charbuf(char_buf_t* buf)
{
	memset(buf->body, 0, buf->maxlength);
	buf->length = 0;
}