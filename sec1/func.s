	.file	"func.c"
	.text
	.globl	f
	.type	f, @function
f:
	pushl %ebp
	movl %esp, %ebp
	subl $4, %esp

	movl 8(%ebp), %edx
	movl 12(%ebp), %eax
	imull %edx, %eax 

	movl 12(%ebp), %edx
	movl 16(%ebp), %ebx
	imull %edx, %ebx
	addl %ebx, %eax

	movl 8(%ebp), %edx
	movl 16(%ebp), %ebx
	imull %edx, %ebx
	addl %ebx, %eax

	leave
	ret
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
