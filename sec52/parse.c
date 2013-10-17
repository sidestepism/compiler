#include "syntree.h"
#include "tokenizer.h"

#include "parse.h"

void eat_it(tokenizer_t t, token_kind_t tok_kind){
    struct token tok = cur_tok(t);
    if (tok_kind != TOK_ANY && tok.kind != tok_kind){
        printf("expected ");
        output_token_kind(tok_kind);
        printf("unexpected ");
        output_token_kind(tok.kind);
        syntax_error(t, "eat_it error");
    }
    output_token(t);
    next_tok(t);
}



/**
 * parse expression
 */
expr_t parse_expr(tokenizer_t t){
    expr_t expr_left, expr_right;
    expr_left = parse_eq_expr(t);

    struct token tok = cur_tok(t);
    if(tok.kind == TOK_ASSIGN){
        eat_it(t, TOK_ASSIGN);
        expr_right = parse_expr(t);
        expr_t assign_expr = mk_expr_bin_op(t->filename, t->line, op_kind_assign, expr_left, expr_right);
        return assign_expr;
    }else{
        // これは式文
        return expr_left;
    }
}

expr_t parse_eq_expr(tokenizer_t t){
    expr_t expr_left, expr_right;
    expr_left = parse_relat_expr(t);

    struct token tok = cur_tok(t);
    while(tok.kind == TOK_EQ || tok.kind == TOK_NEQ){
        eat_it(t, TOK_ANY); // なんでも食べる
        expr_right = parse_relat_expr(t);
        expr_left = mk_expr_bin_op(t->filename, t->line, 
          (tok.kind == TOK_EQ) ? op_kind_eq : op_kind_neq   // == , != のどちらかしかない
          , expr_left, expr_right);  // left にたたんでいく
        tok = cur_tok(t);
    }

  return expr_left;
}

expr_t parse_relat_expr(tokenizer_t t){
    expr_t expr_left, expr_right;
    expr_left = parse_add_expr(t);

    struct token tok = cur_tok(t);

    // relational-operator
    while(tok.kind == TOK_LT || tok.kind == TOK_GT || tok.kind == TOK_LE || tok.kind == TOK_GE){
        eat_it(t, TOK_ANY); // なんでも食べる
        expr_right = parse_add_expr(t);

        op_kind_t op_kind;
        if(tok.kind == TOK_LT) op_kind = op_kind_lt;
        if(tok.kind == TOK_GT) op_kind = op_kind_gt;
        if(tok.kind == TOK_LE) op_kind = op_kind_le;
        if(tok.kind == TOK_GE) op_kind = op_kind_ge;

        expr_left = mk_expr_bin_op(t->filename, t->line, 
          op_kind, expr_left, expr_right);  // left にたたんでいく
        tok = cur_tok(t);
    }
    return expr_left;


  // struct token tok = cur_tok(t);
  // expr_list_t list = mk_expr_list();
  // expr_list_add(list, parse_add_expr(t));
  // while(1)
  // {
  //   if(tok.kind == TOK_LT || tok.kind == TOK_GT || tok.kind == TOK_LE || tok.kind == TOK_GE)
  //   {
  //     eat_it(t, TOK_ANY); // なんでも食べる
  //     expr_list_add(list, parse_add_expr(t));
  //   }
  //   else break;
  // }
  // expr_t expr = ;
}

expr_t parse_add_expr(tokenizer_t t){
    expr_t expr_left, expr_right;
    expr_left = parse_mult_expr(t);

    struct token tok = cur_tok(t);
    while(tok.kind == TOK_PLUS || tok.kind == TOK_MINUS){
        eat_it(t, TOK_ANY); // なんでも食べる
        expr_right = parse_expr(t);
        expr_left = mk_expr_bin_op(t->filename, t->line, 
          (tok.kind == TOK_PLUS) ? op_kind_bin_plus : op_kind_bin_minus   // +, - のどちらかしかない
          , expr_left, expr_right);  // left にたたんでいく
        tok = cur_tok(t);
    }

  return expr_left;

  // struct token tok = cur_tok(t);
  // expr_list_t list = mk_expr_list(t);
  // expr_list_add(list, parse_mult_expr(t));
  // while(1)
  // {
  //   if(tok.kind == TOK_PLUS || TOK_MINUS)
  //   {
  //     tok.next;
  //     expr_list_add(list, parse_mult_expr(t));
  //   }
  //   else break;
  // }
  // expr_t expr = ;
  // return;
}

expr_t parse_mult_expr(tokenizer_t t){
    expr_t expr_left, expr_right;
    expr_left = parse_unary_expr(t);

    struct token tok = cur_tok(t);

    // relational-operator
    while(tok.kind == TOK_MUL || tok.kind == TOK_DIV || tok.kind == TOK_REM){
        eat_it(t, TOK_ANY); // なんでも食べる
        expr_right = parse_unary_expr(t);

        op_kind_t op_kind;
        if(tok.kind == TOK_MUL) op_kind = op_kind_mult;
        if(tok.kind == TOK_DIV) op_kind = op_kind_div;
        if(tok.kind == TOK_REM) op_kind = op_kind_rem;

        expr_left = mk_expr_bin_op(t->filename, t->line, 
          op_kind, expr_left, expr_right);  // left にたたんでいく
        tok = cur_tok(t);
    }
    return expr_left;  
  // expr_list_t list = mk_expr_list(t);
  // expr_list_add(list, parse_unary_expr(t));
  // while(1)
  // {
  //   if(tok.kind == TOK_MUL || TOK_DIV || TOK_REM)
  //   {
  //     tok.next;
  //     expr_list_add(list, parse_unary_expr(t));
  //   }
  //   else break
  // }
  // expr_t expr = ;
  // return;
}

expr_t parse_intlit_expr(tokenizer_t t){
    eat_it(t, TOK_INT_LITERAL);
    return mk_expr_int_literal(t->filename, t->line, "int");
}

expr_t parse_id_expr(tokenizer_t t){
  eat_it(t, TOK_ID);
  struct token tok = cur_tok(t);
  char *name = tok.name;

  if(tok.kind == TOK_LPAREN)
  { // 関数呼び出し
    eat_it(t, TOK_LPAREN);    
    expr_list_t list = parse_arg_expr(t);
    eat_it(t, TOK_RPAREN);
    // いまんとこ 引数がないやつならいける
    return mk_expr_call(t->filename, t->line, name, list);
  }
  else
  { // 変数名
    return mk_expr_id(t->filename, t->line, name);
  }
}

expr_list_t parse_arg_expr(tokenizer_t t){
  // todo
  expr_list_t list = mk_expr_list(t);
  return list;
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
  // struct token tok = cur_tok(t);

  // if(tok.kind == TOK_BANG)
  // {
    eat_it(t, TOK_BANG);
    expr_t expr = parse_unary_expr(t);
    
    return mk_expr_un_op(t->filename, t->line, op_kind_logneg, expr);
  // }
}

expr_t parse_unary_expr(tokenizer_t t)
{
  struct token tok = cur_tok(t);
  switch(tok.kind){
    case TOK_INT_LITERAL:
      return parse_intlit_expr(t);
      break;
    case TOK_ID:
      return parse_id_expr(t);
      break;
    case TOK_LPAREN:
      return parse_paren_expr(t);
      break;
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_BANG:
      return parse_op_expr(t);
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
