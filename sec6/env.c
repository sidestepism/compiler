// env.c
#include "env.h"
#include "list.h"

#include "util.h"
#include "list.h"


/**
 * env 型を作ってポインタを返す
 */ 
env_t mk_env(env_t parent)
{
	env_t env = (env_t) safe_malloc(sizeof(env_t));
	if(env == NULL){
		printf("error: failed to alloc memory for env\n");
		exit(1);
	}
	env->parent = parent;
	env->info_list = mk_syntree_info_list();

	if(parent != NULL){
		env->decl_ptr = parent->decl_ptr;
		env->param_ptr = parent->param_ptr;
	}
	return env;
}

/**
 * env を生成し、変数の値を格納する場所をマップ
 */
env_t scan_syntree_program(program_t prog)
{
	printf("call: scan_syntree_program\n");
	env_t global_env = mk_env(NULL);
    global_env->info_list = mk_syntree_info_list();

	int n = fun_def_list_sz(prog->fun_defs);
	int i;
	for (i = 0; i < n; i++) {
		fun_def_t d = fun_def_list_get(prog->fun_defs, i);
		scan_syntree_fun_def(d, global_env);
	}
	return global_env;
}

/**
 * env を生成し、変数の値を格納する場所をマップ
 */
env_t scan_syntree_fun_def(fun_def_t fun_def, env_t env_global)
{
	printf("call: scan_syntree_fun_def\n");
	env_t env = mk_env(env_global);
    env->info_list = mk_syntree_info_list();


	// 自分の持っている次に置く仮置きメモリのポインタの値 (ebp からのオフセット)
	env->decl_ptr = -4;
	env->param_ptr = 8;

	scan_syntree_params(fun_def->params, env);
	scan_syntree_stmt(fun_def->body, env);

	fun_def->env = env;
	return env;
}

// 宣言のリストを処理
env_t scan_syntree_decls(var_decl_list_t decls, env_t env)
{
	printf("call: scan_syntree_decls\n");
	int n = var_decl_list_sz(decls);
	int i;
	for (i = 0; i < n; i++) {
		var_decl_t d = var_decl_list_get(decls, i);

		// env に追加
		syntree_info_t v = env_add_syntree_info(env, d->v, var_kind_memory, reg_ebp, env->decl_ptr);
		// 構文要素自身にも書き込み
		d->info = v;

		// decl_ptr を前にすすめる
		// @TODO 使われなくなった (スコープを抜けた) メモリ領域をうまいことリサイクルする
		// たぶんできてるはず
		env->decl_ptr -= 4;
	}
	// 引数をすべて追加できた
	return env;
}

env_t scan_syntree_params(var_decl_list_t decls, env_t env)
{
	printf("call: scan_syntree_params\n");
	// 引数を取る数をチェック
	int n = var_decl_list_sz(decls);
	int i;
	for (i = 0; i < n; i++) {
		var_decl_t d = var_decl_list_get(decls, i);

		// env に追加
		syntree_info_t v = env_add_syntree_info(env, d->v, var_kind_memory, reg_ebp, env->param_ptr);
		// 構文要素自身にも書き込み
		d->info = v;

		// param_ptr を後ろにすすめる
		env->param_ptr += 4;
	}
	// 引数をすべて追加できた
	return env;
}


env_t scan_syntree_stmt(stmt_t s, env_t p_env)
{
	printf("call: scan_syntree_stmt\n");

	if(s->kind == stmt_kind_compound){
		// 条件式
		// ブロックなので env を作る。親はp_env
		env_t env = mk_env(p_env);
        env->info_list = mk_syntree_info_list();

		// 宣言文をを処理する
		scan_syntree_decls(s->u.c.decls, env);

		// 文のリストを処理する
		int n = stmt_list_sz(s->u.c.body);
		int i;
		for (i = 0; i < n; i++) {
			// それぞれの文についてスキャン
			stmt_t si = stmt_list_get(s->u.c.body, i);
			scan_syntree_stmt(si, env);
		}
	}else{
		// ブロックではないのでいまある env を使う
		switch(s->kind){
			case stmt_kind_expr:
			case stmt_kind_return:
				scan_syntree_expr(s->u.e, p_env);
			break;

			case stmt_kind_empty:
			case stmt_kind_compound:
			break;
			case stmt_kind_continue:
			break;
			case stmt_kind_break:
			break;

			case stmt_kind_if:
				scan_syntree_stmt(s->u.i.th, p_env);
				if(s->u.i.el != NULL)scan_syntree_stmt(s->u.i.el, p_env);
			break;
			case stmt_kind_while:
				scan_syntree_expr(s->u.w.e, p_env);
				scan_syntree_stmt(s->u.w.body, p_env);
			break;
		}
	}
	return p_env;
}

env_t scan_syntree_expr(expr_t e, env_t env)
{
	printf("call: scan_syntree_expr\n");

	switch(e->kind){
		case expr_kind_int_literal:
			e->info = mk_syntree_info_imm(env, atoi(e->u.s));
		break;
		case expr_kind_id:
			e->info = search_syntree_info_id(env, e->u.s);
            if(e->info == NULL){
                printf("[error] id %s not defined¥n", e->u.s);
                exit(1);
            }
		break;
		case expr_kind_paren:
			// ただのカッコ
			env = scan_syntree_expr(e->u.p, env);
		break;
		case expr_kind_app:
			;
			syntree_info_t v = env_add_syntree_info(env, "partial", var_kind_memory, reg_ebp, env->decl_ptr);
			// この結果を置いておくところを確保 (decl_ptr)
			e->info = v;
			env->decl_ptr -= 4;
			
			// expr_list をすべて走査
			int n = expr_list_sz(e->u.a.args);
			int i;
			for (i = 0; i < n; i++) {
				// それぞれの文についてスキャン
				expr_t ei = expr_list_get(e->u.a.args, i);
				scan_syntree_expr(ei, env);
			}
		break;
	}
	return env;
}

// ------------------------------

/* 文のリスト関係 */
syntree_info_list_t mk_syntree_info_list()
{
  return (syntree_info_list_t)mk_list();
}

void syntree_info_list_add(syntree_info_list_t l, syntree_info_t x)
{
  list_add((list_t)l, (void *)x);
}

syntree_info_t syntree_info_list_get(syntree_info_list_t l, int i)
{
  return (syntree_info_t)list_get((list_t)l, i);
}

int syntree_info_list_sz(syntree_info_list_t l)
{
  return list_sz((list_t)l);
}

/**
 * syntree_info_t のメモリを確保してポインタを返す
 */
syntree_info_t mk_syntree_info()
{
	syntree_info_t info = (syntree_info_t) safe_malloc(sizeof(syntree_info_t) + 32);

	if(info == NULL){
		printf("error: failed to alloc memory for syntree_info\n");
		exit(1);
	}

	return info;
}


syntree_info_t env_add_syntree_info(env_t env, char* name, var_kind_t kind, reg_t reg, int offset)
{
	printf("call: env_add_syntree_info (name: %s, kind: %d, offset: %d)\n", name, kind, offset);
	syntree_info_t info = mk_syntree_info();
	info->name = name;
	info->kind = kind; // var_kind_memory, var_kind_register
	info->reg = reg;
	info->offset = offset;
	syntree_info_list_add(env->info_list, info);
//    pr_syntree_info_table(env->info_list);
	return info;
}

syntree_info_t mk_syntree_info_imm(env_t env, int val){
	syntree_info_t info = mk_syntree_info();
	info->name = NULL;
	info->kind = var_kind_imm;
	info->val = val;
    info->offset = 0;
	return info;
}
syntree_info_t search_syntree_info_id(env_t env, char* name){
	printf("call: search_syntree_info_id (name: %s)\n", name);

	// id でサーチして結果を返す
	int i = 0; 
	int n = syntree_info_list_sz(env->info_list);

	for (i = 0; i < n; i++) {
		// それぞれの文についてスキャン
		syntree_info_t si = syntree_info_list_get(env->info_list, i);
		if (si->name == NULL) continue;
		if (strcmp(si->name, name) == 0){
			printf("[found]: search_syntree_info_id (name: %s/%s, offset: %d)\n", si->name, name, si->offset);
			return si;
		}
	}

	if(env->parent == NULL){
		printf("[not found]: search_syntree_info_id (name: %s)\n", name);
		return NULL;
	}else return search_syntree_info_id(env->parent, name);
}

void pr_syntree_info_table(syntree_info_list_t l)
{
	printf("call: pr_syntree_info_table\n");
	int n = syntree_info_list_sz(l);
    int i;

	for (i = 0; i < n; i++) {
        syntree_info_t info = syntree_info_list_get(l, i);
        printf("name: %s, offset: %d\n", info->name, info->offset);

    }
}

/**
 * env のメモリを解放
 */
void free_env (env_t* buf)
{
	free(buf);
}
