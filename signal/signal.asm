section .data
	message db "Can you try to signal me to run ", 0x0	
	shell db "/bin/sh", 0x0
	end db " :)", 0xa, 0x0

section .text
global _start

hello:
	sub rsp, 0x200
	xor rax, rax
	xor rdi, rdi
	mov rsi, rsp
	mov rdx, 0x400
	syscall

	mov rdx, rax
	mov rax, 0x1
	mov rdi, 0x1
	syscall
	add rsp, 0x200	
	
	ret	

_start:
	mov rax, 1
	mov rdi, 1
	mov rsi, message
	mov rdx, 32
	syscall
	mov rax, 1
	mov rsi, shell
	mov rdx, 7
	syscall
	mov rax, 1
	mov rsi, end
	mov rdx, 4
	syscall

	call hello
	mov rax, 0x3c
	xor rdi, rdi
	syscall
