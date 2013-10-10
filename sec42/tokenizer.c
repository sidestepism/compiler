#include "tokenizer.h"

#define case_x(k) case k: printf("%d:%s\n", t->line, #k); break


void syntax_error(tokenizer_t t){
  int c = t->c;
  FILE *fp = t->fp;
  int num = t->num;
  while(c != '\n'){
    c = next_char(t);
  }
  printf("syntax error :%d:%d\n", t->line, num);
  int i;
  for(i = 0; i < num-1; i++){printf(" ");}
  printf("^\n");
  exit(1);
}


int next_char(tokenizer_t t){
  t->num ++;
  return fgetc(t->fp);
}

tokenizer_t tokenize(tokenizer_t t){
  int c = t->c;
  FILE *fp = t->fp;
  if (t->num > 50) syntax_error(t);

  while (c == ' ' || c == '\n'){ // 連続する改行とスペースを飛ばす
    if(c == '\n'){
      // 改行の場合
      t->line++;
      t->num = 0;
    }else{
      // 空白の場合
    }
    c = next_char(t);
  }

  if (!isdigit(c)){ //数字じゃない時
    t->c = next_char(t);
    switch (c){
      case  '+':
        t->tok.kind = TOK_PLUS;
        break;
      case '(':
        t->tok.kind = TOK_LPAREN;
        break;
      case ')':
        t->tok.kind = TOK_RPAREN;
        break;
      case '{':
        t->tok.kind = TOK_LBRACE;
        break;
      case '}':
        t->tok.kind = TOK_RBRACE;
        break;
      case '*':
        t->tok.kind = TOK_MUL;
        break;
      case '-':
        t->tok.kind = TOK_MINUS;
        break;
      case '/':
        t->tok.kind = TOK_DIV;
        break;
      case '%':
        t->tok.kind = TOK_REM;
        break;
      case '!':
        if(t->c == '='){
          t->tok.kind = TOK_NEQ;
          t->c = next_char(t);
        }else{
          t->tok.kind = TOK_BANG;
        }
        break;
      case '<':
        if(t->c == '='){
          t->tok.kind = TOK_LE;
          t->c = next_char(t);
        }else{
          t->tok.kind = TOK_LT;
        }
        break;
      case '>':
        if(t->c == '='){
          t->tok.kind = TOK_GE;
          t->c = next_char(t);
        }else{
          t->tok.kind = TOK_GT;
        }
        break;
      case '=':
        if(t->c == '='){
          t->tok.kind = TOK_EQ;
          t->c = next_char(t);
        }else{
          t->tok.kind = TOK_ASSIGN;
        }
        break;
      case ';':
        t->tok.kind = TOK_SEMICOLON;
        break;
      case ',':
        t->tok.kind = TOK_COMMA;
        break;
      case EOF:
        t->tok.kind = TOK_EOF;
        break;
      default:
        // ID
        ;
        char name[1024] = {};
        int i = 1;
        if(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
          name[0] = c;
          while(t->c == '_' || (t->c >= 'a' && t->c <= 'z') || (t->c >= 'A' && t->c <= 'Z') || (t->c >= '0' && t->c <= '9')){
            if(i == 1023){
              // too long id (length must be <= 50)
              syntax_error(t);
            }
            name[i++] = t->c;
            t->c = next_char(t);
            // printf("eaten(%c)\n", t->c);
          }

          // 予約後
          if(strcmp("break", name) == 0){
            t->tok.kind = TOK_BREAK;
          }else if(strcmp("continue", name) == 0){
            t->tok.kind = TOK_CONTINUE;
          }else if(strcmp("else", name) == 0){
            t->tok.kind = TOK_ELSE;
          }else if(strcmp("if", name) == 0){
            t->tok.kind = TOK_IF;
          }else if(strcmp("int", name) == 0){
            t->tok.kind = TOK_INT;
          }else if(strcmp("return", name) == 0){
            t->tok.kind = TOK_RETURN;
          }else if(strcmp("while", name) == 0){
            t->tok.kind = TOK_WHILE;
          }else{
            t->tok.kind = TOK_ID;
            strcpy(t->tok.name, name);            
          }
        }else{
          syntax_error(t);
        }
        break;
    }
  }else{
    int i = 0,
        val = 0;
    t->c = c;
    do{
      val = val * 10 + (t->c - '0');
      if(i++ == 10){
        // too long int_literal (length must be <= 10)
        syntax_error(t);
      }
      t->c = next_char(t);
    }while(t->c >= '0' && t->c <= '9');
    t->tok.kind = TOK_INT_LITERAL;
    t->tok.ival = val;

  }
  return t; 
}

tokenizer_t mk_tokenizer(char *filename){
  tokenizer_t t = (tokenizer_t)malloc(sizeof(struct tokenizer));
  FILE *fp = fopen(filename, "rb");
  if(fp == NULL){printf("can't open file\n");exit(1);}
  t->c = next_char(t);
  t->fp = fp;
  t->line = 1;
  t->num = 0;
  return tokenize(t);
}

token cur_tok(tokenizer_t t){
  return t->tok;
}

token next_tok(tokenizer_t t){
  t = tokenize(t);
  return t->tok;
}

// int eat_int(tokenizer_t t){
//   token tok = cur_tok(t);
//   if(tok.kind != TOK_INT) syntax_error(t);
//   next_tok(t);
//   return tok.ival;
// }

// void eat_plus(tokenizer_t t){
//   token tok = cur_tok(t);
//   if (tok.kind != TOK_PLUS) syntax_error(t);
//   next_tok(t);
// }

void output_token(tokenizer_t t){
  if(t->tok.kind == TOK_ID){
    printf("%d:TOK_ID (%s)\n", t->line, t->tok.name);
  }else if(t->tok.kind == TOK_INT_LITERAL){
    printf("%d:TOK_INT_LITERAL (%d)\n", t->line, t->tok.ival);
  }else{
    // switch
    // #define case_x(k) case k: printf("%s\n", #k); break
    switch(t->tok.kind){
      case_x(TOK_INT);
      case_x(TOK_BREAK);
      case_x(TOK_CONTINUE);
      case_x(TOK_ELSE);
      case_x(TOK_IF);
      case_x(TOK_RETURN);
      case_x(TOK_WHILE);
      case_x(TOK_INT_LITERAL);
      case_x(TOK_ID);
      case_x(TOK_EOF);
      case_x(TOK_LPAREN);
      case_x(TOK_RPAREN);
      case_x(TOK_LBRACE);
      case_x(TOK_RBRACE);
      case_x(TOK_MUL);
      case_x(TOK_PLUS);
      case_x(TOK_MINUS);
      case_x(TOK_DIV);
      case_x(TOK_REM);
      case_x(TOK_BANG);
      case_x(TOK_LT);
      case_x(TOK_GT);
      case_x(TOK_LE);
      case_x(TOK_GE);
      case_x(TOK_EQ);
      case_x(TOK_NEQ);
      case_x(TOK_SEMICOLON);
      case_x(TOK_ASSIGN);
      case_x(TOK_COMMA);
   }    
  }
}

