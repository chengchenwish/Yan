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
	.data
	.global pp
	.lcomm pp,1
	.data
	.global y
	.lcomm y,2
	.data
	.global ppp
	.lcomm ppp,8
	.data
	.global f
	.lcomm f,8
	.data
	.global ff
	.lcomm ff,4
	.data
	.global dd
	.lcomm dd,48
	.data
	.global a
	.lcomm a,4
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	leaq  -8(%rbp) , %r10
	movq $99, %r11
	movl %r11d, (%r10)
	movq $881, %r10
	movq $90, %r11
	cmp %r11d, %r10d
	setg %r10b
	movzbq %r10b, %r10
	cmp $0, %r10
	jz .L1
	movq $90, %r10
	movq %r10, %rdi
	movq %rsp, %r10
	andq $15, %r10
	jnz .L3
	call print
	jmp .L4
.L3:
	subq $8,%rsp
	   call print
	 addq $8,%rsp
.L4:
	movq  %rax, %r10
	jmp .L2
.L1:
.L2:
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	addq $8, %rsp
