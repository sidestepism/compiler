#include "syntree.h"
#include "tokenizer.h"

#include "parse.h"


void eat_it(tokenizer_t t, token_kind_t tok_kind){
    struct token tok = cur_tok(t);
    if (tok.kind != tok_kind){

        printf("expected ");
        output_token_kind(tok_kind);
        printf("unexpected ");
        output_token_kind(tok.kind);
        syntax_error(t, "eat_it error");
    }
    output_token(t);
    next_tok(t);
}

<<<<<<< HEAD
expr_t parse_unary_expr(tokenizer_t t){
    // struct token tok = cur_tok(t);

    char* literal = get_copy_charbuf(t->token_buf);

    eat_it(t, TOK_INT_LITERAL);

    return mk_expr_int_literal(t->filename, t->line, literal);
}
=======
>>>>>>> bell

/**
 * parse expression
 */
expr_t parse_expr(tokenizer_t t)i{
    expr_t expr;
    xpr_list_t list = mk_expr_list();
    struct token tok = cur_tok(t);
    expr_list_add(list, parse_eq_expr(t));
    while(expr = parse_expr(t)){
      expr_list_add(list, expr);
    }
  /*要修正*/
    expr = mk_expr_call(t->filename, t->line, list) //?
    return expr;
}

expr_t parse_eq_expr(tokenizer_t t){
  struct token tok = cur_tok(t);
  expr_list_t list = mk_expr_list();
  expr_list_add(list, parse_relat_expr(t));
  while(1)
  {
    if(tok.kind == TOK_EQ || TOK_NEQ)
    {
      tok.next;
      expr_list_add(list, parse_relat_expr(t));
    }
    else break;
  }
  expr_t expr = mk_expr_;
  return;
}

expr_t parse_relat_expr(tokenizer_t t){
  struct token tok = cur_tok(t);
  expr_list_t list = mk_expr_list();
  expr_list_add(list, parse_add_expr(t));
  while(1)
  {
    if(tok.kind == TOK_LT || TOK_GT || TOK_LE || TOK_GE)
    {
      tok.next;
      expr_list_add(list, parse_add_expr(t));
    }
    else break;
  }
  expr_t expr = ;
  return;
}

expr_t parse_add_expr(tokenizer_t t){
  struct token tok = cur_tok(t);
  expr_list_t list = mk_expr_list(t);
  expr_list_add(list, parse_mult_expr(t));
  while(1)
  {
    if(tok.kind == TOK_PLUS || TOK_MINUS)
    {
      tok.next;
      expr_list_add(list, parse_mult_expr(t));
    }
    else break;
  }
  expr_t expr = ;
  return;
}

expr_t parse_mult_expr(tokenizer_t t){
  expr_list_t list = mk_expr_list(t);
  expr_list_add(list, parse_unary_expr(t));
  while(1)
  {
    if(tok.kind == TOK_MUL || TOK_DIV || TOK_REM)
    {
      tok.next;
      expr_list_add(list, parse_unary_expr(t));
    }
    else break
  }
  expr_t expr = ;
  return;
}

expr_t parse_intlit_expr(tokenizer_t t){
    return mk_expr_int_literal(t->filename, t->line, "int");
}

expr_t parse_id_expr(tokenizer_t t){
  expr_list_t list = mk_expr_list(t);
  char *name = t->name;
  eat_it(t, TOK_ID);
  while(1)
  {
    if(eat_it(t, LPAREN))
    {
      parse_arg_expr(t);
      eat_it(t, RPAREN);
    }
    else
    {
      return mk_expr_id(t->filename, t->line, name);
  }
  return mk_expr_call(t->filename, t->line, name, list);
}

expr_t parse_paren_expr(tokenizer_t t)
{
  eat_it(t, TOK_LPAREN);
  expr_t expr = parse_expr(t);
  eat_it(t, TOK_RPAREN);
  return mk_expr_paren(t->filename, t->line, expr);
}

expr_t parse_op_expr(tokenizer_t t)
{
  if(eat_it(t, TOK_BANG))
  {
    expr_t expr = parse_unary_expr(t);
    
    return mk_expr_un_op(t->filename, t->line, expr);
  return mk_expr_

expr_t parse_unary_expr(tokenizer_t t)
{
  struct token tok = cur_tok(t);
  switch(tok.kind){
    case TOK_INT_LITERAL:
      return parse_initlit_expr(t);
      break;
    case TOK_ID;
      return parse_id_expr(t);
      break;
    case TOK_LPAREN;
      return parse_paren_expr(t);
      break;
    case TOK_PLUS || TOK_MINUS || TOK_BANG
      return parse_op_expr(tokenizer_t t);
      break;
    default:
      syntax_error(t, "parse_expr error");
      break;
  }
  return NULL;
}

stmt_t parse_stmt(tokenizer_t t){
    struct token tok = cur_tok(t);
    switch(tok.kind){
        case TOK_SEMICOLON:
            return parse_stmt_empty(t);
        break;

        case TOK_CONTINUE:
            return parse_stmt_continue(t);
        break;
        case TOK_BREAK:
            return parse_stmt_break(t);
        break;
        case TOK_RETURN:
            return parse_stmt_return(t);
        break;
        case TOK_IF:
            return parse_stmt_if(t);
        break;
        case TOK_WHILE:
            return parse_stmt_while(t);
        break;

        case TOK_LBRACE:
            return parse_stmt_compound(t);
        break;

        default:
            return parse_stmt_expr(t);
        break;
    }
    return NULL;
}




stmt_t parse_stmt_compound(tokenizer_t t){
    stmt_list_t list = mk_stmt_list();
    var_decl_list_t decls = mk_var_decl_list();

    eat_it(t, TOK_LBRACE);

    while(1){
        if(cur_tok(t).kind != TOK_RBRACE){
            //宣言
            stmt_list_add(list, parse_stmt(t));
        }else{
            // } でリスト終了
            break;
        }
    }

    eat_it(t, TOK_RBRACE);

    stmt_t comp = mk_stmt_compound(t->filename, t->line, decls, list);

    return comp;

}

stmt_t parse_stmt_while(tokenizer_t t)
{
    /** while(1) */
    eat_it(t, TOK_WHILE);
    eat_it(t, TOK_LPAREN);
    expr_t e = parse_expr(t);
    eat_it(t, TOK_RPAREN);

    /** { ; ; ;  } */
    stmt_t body = parse_stmt(t);

    return mk_stmt_while(t->filename, t->line, e, body);
}


stmt_t parse_stmt_if(tokenizer_t t)
{
    eat_it(t, TOK_IF);
    eat_it(t, TOK_LPAREN);
    expr_t e = parse_expr(t);
    eat_it(t, TOK_RPAREN);
    stmt_t body = parse_stmt(t);

    return mk_stmt_while(t->filename, t->line, e, body);
}


stmt_t parse_stmt_empty(tokenizer_t t)
{
    eat_it(t, TOK_SEMICOLON);
    return mk_stmt_empty(t->filename, t->line);
}

stmt_t parse_stmt_continue(tokenizer_t t)
{
    eat_it(t, TOK_CONTINUE);
    return mk_stmt_continue(t->filename, t->line);
}

stmt_t parse_stmt_break(tokenizer_t t)
{
    eat_it(t, TOK_BREAK);
    return mk_stmt_break(t->filename, t->line);
}

stmt_t parse_stmt_return(tokenizer_t t)
{
    eat_it(t, TOK_RETURN);
    expr_t e = parse_expr(t);
    return mk_stmt_return(t->filename, t->line, e);
}

stmt_t parse_stmt_expr(tokenizer_t t)
{
    expr_t e = parse_expr(t);
    return mk_stmt_expr(t->filename, t->line, e);
}

var_decl_t parse_decl(tokenizer_t t)
{
    eat_it(t, TOK_INT);
    struct token tok = cur_tok(t);
    char *v = tok.name;
    eat_it(t, TOK_ID);
    return mk_var_decl(t->filename, t->line, v);
}
