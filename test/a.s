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
	.global a
	.lcomm a,4
	.data
	.global b
	.lcomm b,4
	.data
	.global c
	.lcomm c,4
	.data
	.global d
	.lcomm d,4
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $20, %rsp
	leaq a(%rip), %r10
	movq $1, %r11
	movl %r11d, (%r10)
	leaq b(%rip), %r10
	movq $2, %r11
	movl %r11d, (%r10)
	leaq c(%rip), %r10
	movq $3, %r11
	movl %r11d, (%r10)
	leaq  -12(%rbp) , %r10
	movq $4, %r11
	movl %r11d, (%r10)
	movq  b(%rip), %r10
	movl  -12(%rbp) , %r11d
	addq %r11, %r10
	movq %r10, %rdi
	movq $0,%rax
	   call print
	movq  %rax, %r10
	movq $22, %r10
	movl %r10d, -16(%rbp)  
	movq $66, %r10
	movl %r10d, -20(%rbp)  
	movl  -16(%rbp) , %r10d
	movl  -20(%rbp) , %r11d
	addq %r11, %r10
	movq %r10, %rdi
	movq $0,%rax
	   call print
	movq  %rax, %r10
	movq $0, %r10
	movq %r10, %rax
	leave
	ret
	addq $20, %rsp
