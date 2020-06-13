	.LC0:
		.string "Q "
	.LC1:
		.string ". "
	.LC2:
		.string "\n"
	.LC3:
		.string "\n\n"
	.LC10:
		.string "%d\n"
	.text
	.global print
	print:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %rdi, -8(%rbp)
	movq -8(%rbp), %rax
	movq %rax, %rsi
	leaq .LC10(%rip), %rdi
	movq $0,%rax
	call printf@PLT
	nop
	leave
	ret
	.LC11:
		.string "%s"
	.text
	.global printstr
	printstr:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %rdi, -8(%rbp)
	movq -8(%rbp), %rax
	movq %rax, %rsi
	leaq .LC11(%rip), %rdi
	movq $0,%rax
	call printf@PLT
	nop
	leave
	ret
	.global printmsg
	printmsg:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
	subq $8, %rsp
	movq  -8(%rbp) , %r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L1
	call printstr
	jmp .L2
	.L1:
	subq $8,%rsp
	   call printstr
	 addq $8,%rsp
	.L2:
	movq  %rax, %r10
	movq $0,%rax
	leave
	ret
	addq $8, %rsp
	.global print_board
	print_board:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
	subq $16, %rsp
	leaq  -12(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -12(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L4
	jmp .L3
	.L3:
	subq $0, %rsp
	leaq  -16(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -16(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L6
	jmp .L5
	.L5:
	movq  -8(%rbp) , %r10
	movl  -12(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	imulq $4,%r11
	addq %r11, %r10
	movq (%r10), %r10
	movq $0, %r11
	cmp %r11d, %r10d
	setne %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L7
	leaq .LC0(%rip) ,%r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L9
	call printmsg
	jmp .L10
	.L9:
	subq $8,%rsp
	   call printmsg
	 addq $8,%rsp
	.L10:
	movq  %rax, %r10
	jmp .L8
	.L7 :
	leaq .LC1(%rip) ,%r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L11
	call printmsg
	jmp .L12
	.L11:
	subq $8,%rsp
	   call printmsg
	 addq $8,%rsp
	.L12:
	movq  %rax, %r10
	.L8:
	leaq  -16(%rbp) , %r10
	movl  -16(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -16(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L6
	jmp .L5
	.L6:
	leaq .LC2(%rip) ,%r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L13
	call printmsg
	jmp .L14
	.L13:
	subq $8,%rsp
	   call printmsg
	 addq $8,%rsp
	.L14:
	movq  %rax, %r10
	leaq  -12(%rbp) , %r10
	movl  -12(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -12(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L4
	jmp .L3
	.L4:
	leaq .LC3(%rip) ,%r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L15
	call printmsg
	jmp .L16
	.L15:
	subq $8,%rsp
	   call printmsg
	 addq $8,%rsp
	.L16:
	movq  %rax, %r10
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	addq $16, %rsp
	.global conflict
	conflict:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
	movl %esi, -12(%rbp)
	movl %edx, -16(%rbp)
	subq $20, %rsp
	leaq  -20(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -20(%rbp) , %r10d
	movl  -12(%rbp) , %r11d
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L18
	jmp .L17
	.L17:
	subq $4, %rsp
	movq  -8(%rbp) , %r10
	movl  -20(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	imulq $4,%r11
	addq %r11, %r10
	movq (%r10), %r10
	movq $0, %r11
	cmp %r11d, %r10d
	setne %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L19
	movq $1, %r10
	movq %r10, %rax
	leave
	ret
	jmp .L20
	.L19 :
	.L20:
	leaq  -24(%rbp) , %r10
	movl  -12(%rbp) , %r11d
	movl  -20(%rbp) , %r12d
	subq %r12, %r11
	movl %r11d, (%r10)
	movq $0, %r10
	movl  -16(%rbp) , %r11d
	movl  -24(%rbp) , %r12d
	subq %r12, %r11
	movq $1, %r12
	addq %r12, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L21
	subq $0, %rsp
	movq  -8(%rbp) , %r10
	movl  -20(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	movl  -24(%rbp) , %r12d
	subq %r12, %r11
	imulq $4,%r11
	addq %r11, %r10
	movq (%r10), %r10
	movq $0, %r11
	cmp %r11d, %r10d
	setne %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L23
	movq $1, %r10
	movq %r10, %rax
	leave
	ret
	jmp .L24
	.L23 :
	.L24:
	jmp .L22
	.L21 :
	.L22:
	movl  -16(%rbp) , %r10d
	movl  -24(%rbp) , %r11d
	addq %r11, %r10
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L25
	subq $0, %rsp
	movq  -8(%rbp) , %r10
	movl  -20(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	movl  -24(%rbp) , %r12d
	addq %r12, %r11
	imulq $4,%r11
	addq %r11, %r10
	movq (%r10), %r10
	movq $0, %r11
	cmp %r11d, %r10d
	setne %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L27
	movq $1, %r10
	movq %r10, %rax
	leave
	ret
	jmp .L28
	.L27 :
	.L28:
	jmp .L26
	.L25 :
	.L26:
	addq $4, %rsp
	leaq  -20(%rbp) , %r10
	movl  -20(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -20(%rbp) , %r10d
	movl  -12(%rbp) , %r11d
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L18
	jmp .L17
	.L18:
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	addq $20, %rsp
	.global solve
	solve:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
	movl %esi, -12(%rbp)
	subq $16, %rsp
	movl  -12(%rbp) , %r10d
	movq $9, %r11
	cmp %r11d, %r10d
	setg %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L29
	subq $0, %rsp
	movq  -8(%rbp) , %r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L31
	call print_board
	jmp .L32
	.L31:
	subq $8,%rsp
	   call print_board
	 addq $8,%rsp
	.L32:
	movq  %rax, %r10
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	jmp .L30
	.L29 :
	.L30:
	leaq  -16(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -16(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L34
	jmp .L33
	.L33:
	subq $0, %rsp
	movq  -8(%rbp) , %r10
	movq %r10, %rdi
	movl  -12(%rbp) , %r10d
	movq %r10, %rsi
	movl  -16(%rbp) , %r10d
	movq %r10, %rdx
	movq %rsp, %r10
	andq $15, %r10
	jnz .L35
	call conflict
	jmp .L36
	.L35:
	subq $8,%rsp
	   call conflict
	 addq $8,%rsp
	.L36:
	movq  %rax, %r10
	movq $0, %r11
	cmp %r11d, %r10d
	sete %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L37
	subq $0, %rsp
	movq  -8(%rbp) , %r10
	movl  -12(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	imulq $4,%r11
	addq %r11, %r10
	movq $1, %r11
	movl %r11d, (%r10)
	movq  -8(%rbp) , %r10
	movq %r10, %rdi
	movl  -12(%rbp) , %r10d
	movq $1, %r11
	addq %r11, %r10
	movq %r10, %rsi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L39
	call solve
	jmp .L40
	.L39:
	subq $8,%rsp
	   call solve
	 addq $8,%rsp
	.L40:
	movq  %rax, %r10
	movq  -8(%rbp) , %r10
	movl  -12(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -16(%rbp) , %r11d
	imulq $4,%r11
	addq %r11, %r10
	movq $0, %r11
	movl %r11d, (%r10)
	jmp .L38
	.L37 :
	.L38:
	leaq  -16(%rbp) , %r10
	movl  -16(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -16(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L34
	jmp .L33
	.L34:
	movq $2, %r10
	movq %r10, %rax
	leave
	ret
	addq $16, %rsp
	.global main
	main:
	pushq %rbp
	movq %rsp, %rbp
	subq $408, %rsp
	leaq  -408(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -408(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L42
	jmp .L41
	.L41:
	leaq  -404(%rbp) , %r10
	movq $0, %r11
	movl %r11d, (%r10)
	movl  -404(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L44
	jmp .L43
	.L43:
	leaq  -400(%rbp) , %r10
	movl  -408(%rbp) , %r11d
	imulq $40,%r11
	addq %r11, %r10
	movl  -404(%rbp) , %r11d
	imulq $4,%r11
	addq %r11, %r10
	movq $0, %r11
	movl %r11d, (%r10)
	leaq  -404(%rbp) , %r10
	movl  -404(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -404(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L44
	jmp .L43
	.L44:
	leaq  -408(%rbp) , %r10
	movl  -408(%rbp) , %r11d
	addq $1, %r11
	movl %r11d, (%r10)
	movl  -408(%rbp) , %r10d
	movq $10, %r11
	cmp %r11d, %r10d
	setl %r10b
	movzbq %r10b, %r10
	cmp $0,%r10
	jz .L42
	jmp .L41
	.L42:
	leaq  -400(%rbp) , %r10
	movq %r10, %rdi
	movq $0, %r10
	movq %r10, %rsi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L45
	call solve
	jmp .L46
	.L45:
	subq $8,%rsp
	   call solve
	 addq $8,%rsp
	.L46:
	movq  %rax, %r10
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	addq $408, %rsp
