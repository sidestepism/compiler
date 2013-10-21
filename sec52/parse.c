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

expr_t parse_unary_expr(tokenizer_t t){
    // struct token tok = cur_tok(t);
    eat_it(t, TOK_INT_LITERAL);
    return mk_expr_int_literal(t->filename, t->line, "int");
}

/**
 * parse expression
 */
expr_t parse_expr(tokenizer_t t){
    struct token tok = cur_tok(t);
    switch(tok.kind){
        case TOK_INT_LITERAL:
            return parse_unary_expr(t);
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

fun_def_t parse_fun_def(tokenizer_t t)
{
    // 型宣言を食べる (int 以外に増やすなら変える)
    eat_it(t, TOK_INT);
    struct token tok = cur_tok(t);
    char *f = tok.name;
    eat_it(t, TOK_LPAREN);
    var_decl_list_t params = mk_var_decl_list;
    eat_it(t, TOK_RPAREN);
    eat_it(t, TOK_LBRACE);
    expr_t body = parse_expr(t);
    eat_it(t, TOK_RBRACE)
    return mk_fun_def(t->filename, t->line, f, params, body);
}

program_t parse_program(tokenizer_t t)
{
    fun_def_list_t list = mk_fun_def_list();
    
}



var_decl_t parse_decl(tokenizer_t t)
{
    eat_it(t, TOK_INT);
    struct token tok = cur_tok(t);
    char *v = tok.name;
    eat_it(t, TOK_ID);
    return mk_var_decl(t->filename, t->line, v);
}
