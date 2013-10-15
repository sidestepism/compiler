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
            // @ TODO
            return parse_stmt_empty(t);
        break;
        case TOK_BREAK:
            // @ TODO
            return parse_stmt_empty(t);
        break;
        case TOK_RETURN:
            // @ TODO
            return parse_stmt_empty(t);
        break;
        case TOK_IF:
            // @ TODO
            return parse_stmt_empty(t);
        break;
        case TOK_WHILE:
            return parse_stmt_while(t);
        break;

        case TOK_LBRACE:
            return parse_stmt_compound(t);
        break;

        default:
            // これは expression では?
            // @ TODO
            return parse_stmt_empty(t);
        break;
    }
    return NULL;
}



stmt_t parse_stmt_compound(tokenizer_t t){
    stmt_list_t list = mk_stmt_list();
    var_decl_list_t decls = mk_var_decl_list();

    eat_it(t, TOK_LPAREN);

    while(1){
        if(cur_tok(t).kind != TOK_RPAREN){
            stmt_list_add(list, parse_stmt(t));
        }else{
            // } でリスト終了
            break;
        }
    }

    eat_it(t, TOK_RPAREN);

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


stmt_t parse_stmt_empty(tokenizer_t t)
{
    eat_it(t, TOK_SEMICOLON);
    return mk_stmt_empty(t->filename, t->line);
}


