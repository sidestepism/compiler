.include "ia-32z6.s"
# [label] cogen_fun_def (name: fun)
# [label] cogen_fun_def_header
.global fun
 .type fun, @function
fun: 
# [label] cogen_prologue
pushl %ebp
movl %esp,%ebp
subl $24,%esp
# [label] cogen_expr_app 
movl 8(%ebp),%eax
addl -8(%ebp),%eax
movl %eax,-4(%ebp)
movl -4(%ebp), %eax
# [label] cogen_epilogue
# [label] cogen_fun_def_trailer
ret
# [label] cogen_fun_def (name: main)
# [label] cogen_fun_def_header
.global main
 .type main, @function
main: 
# [label] cogen_prologue
pushl %ebp
movl %esp,%ebp
subl $12,%esp
# [label] cogen_expr_app 
call fun
movl $1 8(%esp)
movl $2 4(%esp)
movl $3 0(%esp)
movl -4(%ebp), %eax
# [label] cogen_epilogue
# [label] cogen_fun_def_trailer
ret
