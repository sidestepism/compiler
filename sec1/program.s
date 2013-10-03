	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z3aaav
	.align	4, 0x90
__Z3aaav:
Leh_func_begin1:
	pushq	%rbp
Ltmp0:
	movq	%rsp, %rbp
Ltmp1:
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
Leh_func_end1:

	.globl	__Z3bbbv
	.align	4, 0x90
__Z3bbbv:
Leh_func_begin2:
	pushq	%rbp
Ltmp2:
	movq	%rsp, %rbp
Ltmp3:
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
Leh_func_end2:

	.globl	__Z3cccv
	.align	4, 0x90
__Z3cccv:
Leh_func_begin3:
	pushq	%rbp
Ltmp4:
	movq	%rsp, %rbp
Ltmp5:
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
Leh_func_end3:

	.globl	__Z3dddv
	.align	4, 0x90
__Z3dddv:
Leh_func_begin4:
	pushq	%rbp
Ltmp6:
	movq	%rsp, %rbp
Ltmp7:
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
Leh_func_end4:

	.section	__TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame0:
Lsection_eh_frame:
Leh_frame_common:
Lset0 = Leh_frame_common_end-Leh_frame_common_begin
	.long	Lset0
Leh_frame_common_begin:
	.long	0
	.byte	1
	.asciz	 "zR"
	.byte	1
	.byte	120
	.byte	16
	.byte	1
	.byte	16
	.byte	12
	.byte	7
	.byte	8
	.byte	144
	.byte	1
	.align	3
Leh_frame_common_end:
	.globl	__Z3aaav.eh
__Z3aaav.eh:
Lset1 = Leh_frame_end1-Leh_frame_begin1
	.long	Lset1
Leh_frame_begin1:
Lset2 = Leh_frame_begin1-Leh_frame_common
	.long	Lset2
Ltmp8:
	.quad	Leh_func_begin1-Ltmp8
Lset3 = Leh_func_end1-Leh_func_begin1
	.quad	Lset3
	.byte	0
	.byte	4
Lset4 = Ltmp0-Leh_func_begin1
	.long	Lset4
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset5 = Ltmp1-Ltmp0
	.long	Lset5
	.byte	13
	.byte	6
	.align	3
Leh_frame_end1:

	.globl	__Z3bbbv.eh
__Z3bbbv.eh:
Lset6 = Leh_frame_end2-Leh_frame_begin2
	.long	Lset6
Leh_frame_begin2:
Lset7 = Leh_frame_begin2-Leh_frame_common
	.long	Lset7
Ltmp9:
	.quad	Leh_func_begin2-Ltmp9
Lset8 = Leh_func_end2-Leh_func_begin2
	.quad	Lset8
	.byte	0
	.byte	4
Lset9 = Ltmp2-Leh_func_begin2
	.long	Lset9
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset10 = Ltmp3-Ltmp2
	.long	Lset10
	.byte	13
	.byte	6
	.align	3
Leh_frame_end2:

	.globl	__Z3cccv.eh
__Z3cccv.eh:
Lset11 = Leh_frame_end3-Leh_frame_begin3
	.long	Lset11
Leh_frame_begin3:
Lset12 = Leh_frame_begin3-Leh_frame_common
	.long	Lset12
Ltmp10:
	.quad	Leh_func_begin3-Ltmp10
Lset13 = Leh_func_end3-Leh_func_begin3
	.quad	Lset13
	.byte	0
	.byte	4
Lset14 = Ltmp4-Leh_func_begin3
	.long	Lset14
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset15 = Ltmp5-Ltmp4
	.long	Lset15
	.byte	13
	.byte	6
	.align	3
Leh_frame_end3:

	.globl	__Z3dddv.eh
__Z3dddv.eh:
Lset16 = Leh_frame_end4-Leh_frame_begin4
	.long	Lset16
Leh_frame_begin4:
Lset17 = Leh_frame_begin4-Leh_frame_common
	.long	Lset17
Ltmp11:
	.quad	Leh_func_begin4-Ltmp11
Lset18 = Leh_func_end4-Leh_func_begin4
	.quad	Lset18
	.byte	0
	.byte	4
Lset19 = Ltmp6-Leh_func_begin4
	.long	Lset19
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset20 = Ltmp7-Ltmp6
	.long	Lset20
	.byte	13
	.byte	6
	.align	3
Leh_frame_end4:


.subsections_via_symbols
