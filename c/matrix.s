	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_arymul                 ## -- Begin function arymul
	.p2align	4, 0x90
_arymul:                                ## @arymul
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi0:
	.cfi_def_cfa_offset 16
Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi2:
	.cfi_def_cfa_register %rbp
	addq	$16, %rdi
	movl	$8, %r8d
	.p2align	4, 0x90
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	(%rsi), %ecx
	movl	12(%rsi), %eax
	imull	-16(%rdi), %ecx
	imull	-12(%rdi), %eax
	addl	%ecx, %eax
	movl	24(%rsi), %ecx
	imull	-8(%rdi), %ecx
	addl	%eax, %ecx
	movl	36(%rsi), %eax
	imull	-4(%rdi), %eax
	addl	%ecx, %eax
	movl	48(%rsi), %ecx
	imull	(%rdi), %ecx
	addl	%eax, %ecx
	movl	%ecx, -8(%rdx,%r8)
	movl	4(%rsi), %eax
	movl	16(%rsi), %ecx
	imull	-16(%rdi), %eax
	imull	-12(%rdi), %ecx
	addl	%eax, %ecx
	movl	28(%rsi), %eax
	imull	-8(%rdi), %eax
	addl	%ecx, %eax
	movl	40(%rsi), %ecx
	imull	-4(%rdi), %ecx
	addl	%eax, %ecx
	movl	52(%rsi), %eax
	imull	(%rdi), %eax
	addl	%ecx, %eax
	movl	%eax, -4(%rdx,%r8)
	movl	8(%rsi), %eax
	movl	20(%rsi), %ecx
	imull	-16(%rdi), %eax
	imull	-12(%rdi), %ecx
	addl	%eax, %ecx
	movl	32(%rsi), %eax
	imull	-8(%rdi), %eax
	addl	%ecx, %eax
	movl	44(%rsi), %ecx
	imull	-4(%rdi), %ecx
	addl	%eax, %ecx
	movl	56(%rsi), %eax
	imull	(%rdi), %eax
	addl	%ecx, %eax
	movl	%eax, (%rdx,%r8)
	addq	$20, %rdi
	addq	$12, %r8
	cmpq	$56, %r8
	jne	LBB0_1
## BB#2:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
