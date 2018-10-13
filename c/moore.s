	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_arithmetricSeries      ## -- Begin function arithmetricSeries
	.p2align	4, 0x90
_arithmetricSeries:                     ## @arithmetricSeries
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
	subq	$16, %rsp
	movl	%edi, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	LBB0_2
## BB#1:
	movl	$0, -4(%rbp)
	jmp	LBB0_3
LBB0_2:
	movl	-8(%rbp), %eax
	movl	-8(%rbp), %ecx
	subl	$1, %ecx
	movl	%ecx, %edi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	callq	_arithmetricSeries
	movl	-12(%rbp), %ecx         ## 4-byte Reload
	addl	%eax, %ecx
	movl	%ecx, -4(%rbp)
LBB0_3:
	movl	-4(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ## -- Begin function main
LCPI1_0:
	.quad	4696837146684686336     ## double 1.0E+6
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4
LCPI1_1:
	.long	1127219200              ## 0x43300000
	.long	1160773632              ## 0x45300000
	.long	0                       ## 0x0
	.long	0                       ## 0x0
LCPI1_2:
	.quad	4841369599423283200     ## double 4503599627370496
	.quad	4985484787499139072     ## double 1.9342813113834067E+25
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi3:
	.cfi_def_cfa_offset 16
Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi5:
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, -20(%rbp)
	callq	_clock
	movq	%rax, -32(%rbp)
	movl	$0, -52(%rbp)
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$10000, -52(%rbp)       ## imm = 0x2710
	jge	LBB1_7
## BB#2:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-52(%rbp), %edi
	callq	_arithmetricSeries
	movl	%eax, -56(%rbp)
	movl	-52(%rbp), %edi
	callq	_arithmetricSeries
	movl	%eax, -60(%rbp)
	movl	-56(%rbp), %eax
	cmpl	-60(%rbp), %eax
	jle	LBB1_4
## BB#3:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -20(%rbp)
	jmp	LBB1_5
LBB1_4:                                 ##   in Loop: Header=BB1_1 Depth=1
	movl	-56(%rbp), %eax
	addl	-20(%rbp), %eax
	movl	%eax, -20(%rbp)
LBB1_5:                                 ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_6
LBB1_6:                                 ##   in Loop: Header=BB1_1 Depth=1
	movl	-52(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -52(%rbp)
	jmp	LBB1_1
LBB1_7:
	leaq	L_.str(%rip), %rdi
	movsd	LCPI1_0(%rip), %xmm0    ## xmm0 = mem[0],zero
	movq	%rdi, -72(%rbp)         ## 8-byte Spill
	movsd	%xmm0, -80(%rbp)        ## 8-byte Spill
	callq	_clock
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	-32(%rbp), %rdi
	subq	%rdi, %rax
	movq	%rax, %xmm0
	movaps	LCPI1_1(%rip), %xmm1    ## xmm1 = [1127219200,1160773632,0,0]
	punpckldq	%xmm1, %xmm0    ## xmm0 = xmm0[0],xmm1[0],xmm0[1],xmm1[1]
	movapd	LCPI1_2(%rip), %xmm1    ## xmm1 = [4.503600e+15,1.934281e+25]
	subpd	%xmm1, %xmm0
	haddpd	%xmm0, %xmm0
	movsd	-80(%rbp), %xmm1        ## 8-byte Reload
                                        ## xmm1 = mem[0],zero
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -48(%rbp)
	movsd	-48(%rbp), %xmm0        ## xmm0 = mem[0],zero
	movq	-72(%rbp), %rdi         ## 8-byte Reload
	movb	$1, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%eax, -84(%rbp)         ## 4-byte Spill
	movl	%ecx, %eax
	addq	$96, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%f"


.subsections_via_symbols
