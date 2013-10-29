.include "ia-32z6.s"
# [label] cogen_fun_def (name: func)
# [label] cogen_fun_def_header
.global func
 .type func, @function
func: 
# [label] cogen_prologue
pushl %ebp
movl %esp,%ebp
subl $16,%esp
# [label] cogen_expr_app 
movl 8(%ebp),%eax
addl $1,%eax
movl %eax,-4(%ebp)
movl -4(%ebp), %eax
# [label] cogen_epilogue
# [label] cogen_fun_def_trailer
ret
