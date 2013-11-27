#include "syntree.h"
#include "tokenizer.h"
#include "parse.h"
#include "code_gen.h"
#include "list.h"
#include "env.h"
#include "assert.h"


void cogen_program(FILE *fp, program_t ds){
  int n = fun_def_list_sz(ds->fun_defs);
  int i;
    fprintf(fp, ".include \"ia-32z.s\"\n");
  for(i = 0; i < n; i++){
    fun_def_t d = fun_def_list_get(ds->fun_defs, i);
    cogen_fun_def(fp, d);
    // @TODO
  }
}

void cogen_fun_def_header(FILE *fp, fun_def_t d){
  fprintf(fp, "# [label] cogen_fun_def_header\n");
    fprintf(fp, ".global %s\n", d->f);
    fprintf(fp, " .type %s, @function\n", d->f);

    fprintf(fp, "%s: \n", d->f);

}

void cogen_prologue(FILE *fp, fun_def_t d){
  fprintf(fp, "# [label] cogen_prologue\n");
    fprintf(fp, "pushl %%ebp\n");
    fprintf(fp, "movl %%esp,%%ebp\n");

    // スタックを広げる
    fprintf(fp, "subl $%d,%%esp\n", d->env->param_ptr - d->env->decl_ptr + 4);

    fprintf(fp, "pushl %%esi\n");
    fprintf(fp, "pushl %%ebx\n");
}

void cogen_epilogue(FILE *fp, fun_def_t d){
    fprintf(fp, "popl %%esi\n");
    fprintf(fp, "popl %%ebx\n");
  fprintf(fp, "# [label] cogen_epilogue\n");
}

void cogen_fun_def_trailer(FILE *fp, fun_def_t d){
  fprintf(fp, "# [label] cogen_fun_def_trailer\n");
  // fprintf(fp, "movl %%ebp, %%esp\n");
  // fprintf(fp, "popl %%ebp\n");
  fprintf(fp, "leave\n");
  fprintf(fp, "ret\n");

}


void cogen_fun_def(FILE *fp, fun_def_t d){
  fprintf(fp, "# [label] cogen_fun_def (name: %s)\n", d->f);
  cogen_fun_def_header(fp, d);
  cogen_prologue(fp, d);

  cogen_stmt(fp, d->body);

  cogen_epilogue(fp, d);
  cogen_fun_def_trailer(fp, d);
}


void cogen_stmt(FILE *fp, stmt_t s){
  env_t cur_env;

  cur_env = s->env;
  while(cur_env != NULL){
    if(cur_env->end_label != NULL) fprintf(fp, "# end_label: %s\n", cur_env->end_label);
    cur_env = cur_env->parent;
  }
  fprintf(fp, "############\n");

  // fprintf(fp, "call: scan_syntree_stmt\n");
    char* start_label;
    char* end_label;
    char* then_label;
  if(s->kind == stmt_kind_compound){
    // 複合文なのでリスト処理
    int n = stmt_list_sz(s->u.c.body);
    int i;
    for (i = 0; i < n; i++) {
      // それぞれの文について処理
      stmt_t si = stmt_list_get(s->u.c.body, i);
      cogen_stmt(fp, si);
    }
  }else{
    // ブロックではないので処理
    switch(s->kind){
      case stmt_kind_expr:
            // 式文
            // [todo] どこにも使われない式文は削除可能
            cogen_expr(fp, s->u.e);

            // !important 
            // if expr result is in %eflag
            if(s->u.e->info->reg == reg_eflags){
              char* true_label = get_label();
              fprintf(fp, "movl $1, %%edx\n");
              switch (s->u.e->u.a.o){
                  // bool op -> its result is in %eflags
                  case op_kind_eq:
                      fprintf(fp, "je %s\n", true_label);
                      break;
                  case op_kind_neq:
                      fprintf(fp, "jne %s\n", true_label);
                      break;
                  case op_kind_lt:
                      fprintf(fp, "jl %s\n", true_label);
                      break;
                  case op_kind_gt:
                      fprintf(fp, "jg %s\n", true_label);
                      break;
                  case op_kind_le:
                      fprintf(fp, "jle %s\n", true_label);
                      break;
                  case op_kind_ge:
                      fprintf(fp, "jge %s\n", true_label);
                      break;
                  default:
                      fprintf(fp, "invalid expression");
                      exit(1);
                      break;
              };
              fprintf(fp, "movl $0, %%edx\n");
              fprintf(fp, "%s: \n", true_label);
              // result is saved in %edx
              s->u.e->info->reg = reg_edx;
            }

            break;
      case stmt_kind_return:
            cogen_expr(fp, s->u.e);
            // !important 
            // if expr result is in %eflag
            if(s->u.e->info->reg == reg_eflags){
              char* true_label = get_label();
              fprintf(fp, "movl $1, %%edx\n");
              switch (s->u.e->u.a.o){
                  // bool op -> its result is in %eflags
                  case op_kind_eq:
                      fprintf(fp, "je %s\n", true_label);
                      break;
                  case op_kind_neq:
                      fprintf(fp, "jne %s\n", true_label);
                      break;
                  case op_kind_lt:
                      fprintf(fp, "jl %s\n", true_label);
                      break;
                  case op_kind_gt:
                      fprintf(fp, "jg %s\n", true_label);
                      break;
                  case op_kind_le:
                      fprintf(fp, "jle %s\n", true_label);
                      break;
                  case op_kind_ge:
                      fprintf(fp, "jge %s\n", true_label);
                      break;
                  default:
                      fprintf(fp, "invalid expression");
                      exit(1);
                      break;
              };
              fprintf(fp, "movl $0, %%edx\n");
              fprintf(fp, "%s: \n", true_label);
              // result is saved in %edx
              s->u.e->info->reg = reg_edx;
            }

            // 戻り値は eax
            // [todo] env でうまく処理して削除可能
            fprintf(fp, "movl %s, %%eax\n", cogen_addr(s->u.e->info));
            break;
      case stmt_kind_empty:
//        fprintf(fp, "# empty\n");
      break;
      case stmt_kind_compound:
            // すでに処理しているはず
      break;
      case stmt_kind_continue:
        //  start_label
        cur_env = s->env->parent;
        while(cur_env != NULL){
          if(cur_env->start_label != NULL) break;
          cur_env = cur_env->parent;
        }
        if(cur_env == NULL){
          printf("syntax error: invalid break\n");
          exit(1);
        }
        fprintf(fp, "# continue\n");
        fprintf(fp, "jmp %s\n", cur_env->start_label);        
      break;
      case stmt_kind_break:
        // end_label 
        cur_env = s->env->parent;
        fprintf(fp, "# break search start\n");

        while(cur_env != NULL){
          if(cur_env->end_label != NULL) break;
          cur_env = cur_env->parent;
          fprintf(fp, "# searching .. \n");
        }
        if(cur_env == NULL){
          printf("syntax error: invalid break\n");
          exit(1);
        }
         fprintf(fp, "# break search found!\n");
         fprintf(fp, "# break\n");
        fprintf(fp, "jmp %s\n", cur_env->end_label);        
      break;

        case stmt_kind_if:
            ;
            // jump if -> then_label
            //   else の内容
            //   jmp to end_label
            // then_label
            //   then の内容
            // end_label

            end_label = get_label();
            then_label = get_label();

            // u.i.e で分岐
            // ほんとはちゃんと eflags に結果が入ってるかどうか調べる [todo]
            // s->u.i.e->info->kind == var_kind_reg && s->u.i.e->info->reg == reg_eflags

            fprintf(fp, "# [if start] jumpif? -> then_label \n");
            cogen_expr(fp, s->u.i.e);
            fprintf(fp, "# condition eval end \n");

            if(1){
                switch (s->u.i.e->u.a.o){
                    case op_kind_eq:
                        fprintf(fp, "je %s\n", then_label);
                        break;
                    case op_kind_neq:
                        fprintf(fp, "jne %s\n", then_label);
                          break;
                    case op_kind_lt:
                        fprintf(fp, "jl %s\n", then_label);
                        break;
                    case op_kind_gt:
                        fprintf(fp, "jg %s\n", then_label);
                        break;
                    case op_kind_le:
                        fprintf(fp, "jle %s\n", then_label);
                        break;
                    case op_kind_ge:
                        fprintf(fp, "jge %s\n", then_label);
                        break;
                    default:
                        fprintf(fp, "movl %s,%%eax\n", cogen_addr(s->u.i.e->info));
                        fprintf(fp, "cmpl $0,%%eax\n");
                        fprintf(fp, "jne %s\n", then_label);
                        break;
                };
            }else{
                fprintf(fp, "# [error:] non eflags\n");
                exit(1);
            }


            if(s->u.i.el != NULL){
                fprintf(fp, "# else_stmt \n");
                cogen_stmt(fp, s->u.i.el);
            }

            fprintf(fp, "# jumpto -> endif_label \n");
            fprintf(fp, "jmp %s\n", end_label);

            fprintf(fp, "# then_label \n");
            fprintf(fp, "%s: \n", then_label);

            fprintf(fp, "# then_stmt \n");
            cogen_stmt(fp, s->u.i.th);

            fprintf(fp, "# end_label \n");
            fprintf(fp, "%s: \n", end_label);

            fprintf(fp, "# [if end]\n");
            // scan_syntree_stmt(s->u.i.th, p_env);
            // if(s->u.i.el != NULL)scan_syntree_stmt(s->u.i.el, p_env);
            break;
        case stmt_kind_while:
            assert(s->u.w.body->env->start_label == NULL);
            assert(s->u.w.body->env->end_label == NULL);
            // s->env->start_label = start_label = get_label();
            // s->env->end_label = end_label = get_label();
            s->u.w.body->env->start_label = start_label = get_label();
            s->u.w.body->env->end_label = end_label = get_label();

            fprintf(fp, "# [while start] start_label \n");
            fprintf(fp, "%s: \n", start_label);

            cogen_expr(fp, s->u.i.e);

            fprintf(fp, "# jumpif? -> end_label \n");
            if(1){
                switch (s->u.i.e->u.a.o){
                    // bool op -> its result is in %eflags
                    case op_kind_eq:
                        fprintf(fp, "jne %s\n", end_label);
                        break;
                    case op_kind_neq:
                        fprintf(fp, "je %s\n", end_label);
                        break;
                    case op_kind_lt:
                        fprintf(fp, "jge %s\n", end_label);
                        break;
                    case op_kind_gt:
                        fprintf(fp, "jle %s\n", end_label);
                        break;
                    case op_kind_le:
                        fprintf(fp, "jl %s\n", end_label);
                        break;
                    case op_kind_ge:
                        fprintf(fp, "jg %s\n", end_label);
                        break;
                    default:
                        fprintf(fp, "movl %s,%%eax\n", cogen_addr(s->u.i.e->info));
                        fprintf(fp, "cmpl $0,%%eax\n");
                        fprintf(fp, "je %s\n", end_label);
                        break;
                };
            }else{
                fprintf(fp, "# [error:] non eflags\n");
                exit(1);
            }
 
            cogen_stmt(fp, s->u.w.body);
            
            fprintf(fp, "# jumpto -> start_label (%s)\n", start_label);
            fprintf(fp, "jmp %s\n", start_label);
            
            fprintf(fp, "# [while end] %s\n", end_label);
            fprintf(fp, "%s: \n", end_label);

            break;

    }
  }
}

void cogen_expr_app(FILE *fp, expr_t e){

//  fprintf(fp, "# [label] cogen_expr_app \n");
  expr_t left, right;
  if(expr_list_sz(e->u.a.args) > 0){
      left = expr_list_get(e->u.a.args, 0);
  }
  if(expr_list_sz(e->u.a.args) > 1){
      right = expr_list_get(e->u.a.args, 1);    
  }
 
   switch(e->u.a.o){
    case op_kind_none:
      break;
    case op_kind_fun:/* 組み込み演算子ではない, 関数 */
      ;
      int i;
      int size = expr_list_sz(e->u.a.args);

      for(i = size-1 ; i >= 0; i--){
        cogen_expr(fp, expr_list_get(e->u.a.args, i));
      }

      for(i = size-1 ; i >= 0; i--){
        fprintf(fp, "movl %s, %%ebx\n", cogen_addr(expr_list_get(e->u.a.args, i)->info));
        fprintf(fp, "movl %%ebx, %d(%%esp)\n", 4*i);
      }

      fprintf(fp, "call %s\n", e->u.a.f);
      fprintf(fp, "movl %%eax, %s\n", cogen_addr(e->info));

      break;
    case op_kind_assign:/* a = b 2項だから命令とvar返す */
      // 右辺を評価
      cogen_expr(fp, right);
      if(right->info->reg == reg_eflags){
          char* true_label = get_label();
            fprintf(fp, "movl $1, %%ecx\n");
            switch (right->u.a.o){
                // bool op -> its result is in %eflags
                case op_kind_eq:
                    fprintf(fp, "je %s\n", true_label);
                    break;
                case op_kind_neq:
                    fprintf(fp, "jne %s\n", true_label);
                    break;
                case op_kind_lt:
                    fprintf(fp, "jl %s\n", true_label);
                    break;
                case op_kind_gt:
                    fprintf(fp, "jg %s\n", true_label);
                    break;
                case op_kind_le:
                    fprintf(fp, "jle %s\n", true_label);
                    break;
                case op_kind_ge:
                    fprintf(fp, "jge %s\n", true_label);
                    break;
                default:
                    fprintf(fp, "invalid expression");
                    exit(1);
                    break;
            };
            fprintf(fp, "movl $0, %%ecx\n");
            fprintf(fp, "%s: \n", true_label);
            right->info->reg = reg_ecx; // @TODO
      }
      // 代入文
      cogen_mov(fp, right, left);
      break;
    case op_kind_eq:
    case op_kind_neq:
    case op_kind_lt:
    case op_kind_gt:
    case op_kind_le:
    case op_kind_ge:
      cogen_expr(fp, right);
      cogen_expr(fp, left);
      cogen_mov_reg(fp, right, reg_eax);
      cogen_mov_reg(fp, left, reg_ebx);
      fprintf(fp, "cmpl %s,%s\n",  cogen_pr_reg(reg_eax), cogen_pr_reg(reg_ebx));
          // 結果は eflags に格納しました
          e->info->kind = var_kind_reg;
          e->info->reg = reg_eflags;
      break;

      // ins.kind = e->u.a.o;//cmpl
      // ins.left = cogen_expr(fp, expr_list_get(e->u.a.args, 0), env);
      // ins.right = cogen_expr(fp, expr_list_get(e->u.a.args, 1), env);
      // pr_ins(&ins);
      break;
    case op_kind_bin_plus:
          // もっと効率化できるぞ      
          cogen_expr(fp, left);
          cogen_expr(fp, right);
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "addl %s,%%eax\n", cogen_addr(right->info));
          fprintf(fp, "movl %%eax,%s\n", cogen_addr(e->info));
          break;
    case op_kind_bin_minus:
          cogen_expr(fp, left);
          cogen_expr(fp, right);
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "subl %s,%%eax\n", cogen_addr(right->info));
          fprintf(fp, "movl %%eax,%s\n", cogen_addr(e->info));
          break;
    case op_kind_mult:
          cogen_expr(fp, right);
          cogen_expr(fp, left);
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "imull %s,%%eax\n", cogen_addr(right->info));
          fprintf(fp, "movl %%eax,%s\n", cogen_addr(e->info));
          break;
    case op_kind_div:
          cogen_expr(fp, right);
          cogen_expr(fp, left);
          fprintf(fp, "movl $0,%%edx\n");          
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "movl %s,%%ebx\n", cogen_addr(right->info));
          fprintf(fp, "idivl %%ebx\n");
          fprintf(fp, "movl %%eax,%s\n", cogen_addr(e->info));
          break;
    case op_kind_rem:
          cogen_expr(fp, right);
          cogen_expr(fp, left);
          fprintf(fp, "movl $0,%%edx\n");          
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "movl %s,%%ebx\n", cogen_addr(right->info));
          fprintf(fp, "idivl %%ebx\n");
          fprintf(fp, "movl %%edx,%s\n", cogen_addr(e->info));
          break;
    break;
      /*5+5, x+5, 5+x*/
      case op_kind_un_minus:
          cogen_expr(fp, left);
          fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
          fprintf(fp, "movl $0,%%ebx\n");
          fprintf(fp, "subl %%eax,%%ebx\n");
          fprintf(fp, "movl %%ebx,%s\n", cogen_addr(e->info));
          break;
      case op_kind_un_plus:
          cogen_expr(fp, left);
          cogen_mov(fp, left, e);
          break;
    case op_kind_logneg:
          cogen_expr(fp, left);
          if(left->info->reg == reg_eflags){
          char* true_label = get_label();
            fprintf(fp, "movl $0, %%edx\n");
            switch (left->u.a.o){
                // bool op -> its result is in %eflags
                case op_kind_eq:
                    fprintf(fp, "je %s\n", true_label);
                    break;
                case op_kind_neq:
                    fprintf(fp, "jne %s\n", true_label);
                    break;
                case op_kind_lt:
                    fprintf(fp, "jl %s\n", true_label);
                    break;
                case op_kind_gt:
                    fprintf(fp, "jg %s\n", true_label);
                    break;
                case op_kind_le:
                    fprintf(fp, "jle %s\n", true_label);
                    break;
                case op_kind_ge:
                    fprintf(fp, "jge %s\n", true_label);
                    break;
                default:
                    fprintf(fp, "invalid expression");
                    exit(1);
                    break;
            };
            fprintf(fp, "movl $1, %%edx\n");
            fprintf(fp, "%s: \n", true_label);
            // result is saved in %edx
            e->info->reg = reg_edx;
          }else{
            char* true_label = get_label();

            cogen_mov_reg(fp, left, reg_eax);
            fprintf(fp, "movl $0, %%edx\n");            
            fprintf(fp, "cmpl $0, %%eax\n");
            fprintf(fp, "jne %s\n", true_label);
            fprintf(fp, "movl $1, %%edx\n");
            fprintf(fp, "%s: \n", true_label);
            fprintf(fp, "movl %%edx, %s\n", cogen_addr(e->info));
          }


          break;
      break;
  }
}

char* cogen_addr(syntree_info_t info)
{
    if (info == NULL) {
        printf("info is NULL\n");
        exit(1);
    }
  char str[32];
  switch(info->kind){
    case var_kind_imm:
      sprintf(str, "$%d", info->val);
      return strdup(str);
      break;
    case var_kind_reg:
      return cogen_pr_reg(info->reg);
      break;
    case var_kind_memory:
          if(info->offset == 0){
            sprintf(str, "(%s)", cogen_pr_reg(info->reg));
          }else{
            sprintf(str, "%d(%s)", info->offset, cogen_pr_reg(info->reg));
          }
          return strdup(str);
      break;
  }
  return NULL;
}

void cogen_mov(FILE* fp, expr_t left, expr_t right){
    if(right->info->kind == var_kind_memory && left->info->kind == var_kind_memory){
    // both of right and left are placed on memory
    // mem->mem
    fprintf(fp, "movl %s,%%eax\n", cogen_addr(left->info));
    fprintf(fp, "movl %%eax,%s\n", cogen_addr(right->info));
  }else{
    // mem->reg, reg->mem, reg->reg
    fprintf(fp, "movl %s,%s\n", cogen_addr(left->info), cogen_addr(right->info));
  }
}

char* cogen_pr_reg(reg_t reg)
{
  switch(reg){
    case reg_eax: return "%eax";
    case reg_ebx: return "%ebx";
    case reg_ecx: return "%ecx";
    case reg_edx: return "%edx";
    case reg_edi: return "%edi";
    case reg_esi: return "%esi";
    case reg_esp: return "%esp";
    case reg_ebp: return "%ebp";
    case reg_eip: return "%eip";
    case reg_eflags: return "%eflags";
  }
  return NULL;
}

void cogen_mov_reg(FILE *fp, expr_t e, reg_t reg){
  // ugry @TODO
  if(e->info->kind == var_kind_imm){
    fprintf(fp, "movl $%d,%s\n", e->info->val, cogen_pr_reg(reg));
  }else{    
    fprintf(fp, "movl %s,%s\n", cogen_addr(e->info), cogen_pr_reg(reg));
  }
}

void cogen_expr(FILE *fp, expr_t e){
  switch(e->kind){
    case expr_kind_int_literal:
      break;
    case expr_kind_id:      
      break;
    case expr_kind_paren:
      cogen_expr(fp, e->u.p);

      break;
    case expr_kind_app:
      cogen_expr_app(fp, e);
      break;
    default:
      fprintf(fp, "syntax error\n");
      break;
  }  
}

int label_c = 0;
char* get_label(){
    char str[12];
    sprintf(str, "label%d", label_c);
    label_c++;
    return strdup(str);
}



