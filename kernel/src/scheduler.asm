;
; scheduler.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

%include "kerndef.h"

extern swap_ws

extern head_process
extern head_thread


global start_timer
start_timer:
	mov al, 0x30	; interrupt on terminal count mode
	out 0x43, al
	mov al, 1
	out 0x40, al
	xor al, al
	out 0x40, al
	ret


global context_switch
context_switch:
	PUSH_CALLER_REGS
	push rbx

	mov rbx, [head_thread]
	mov rax, rbx
	test rbx, rbx
	jz .return_early

.search_loop:
	cmp byte [rbx + 1], 0 ; is thread ready?
	je .break

	mov rbx, [rbx + 24]
	cmp rax, rbx ; no ready threads found?
	je .return_early
	jmp .search_loop
.break:

	; set thread as running
	mov byte [rbx + 1], 1
	
	; set head_thread to next thread
	mov rax, [rbx + 24]
	mov [head_thread], rax

	; swap in the thread's/process' pages
	mov rdi, [rbx + 8]
	mov rdi, [rdi + 16]
	mov rsi, STACK_WS
	call swap_ws
	
	mov rdi, [rbx + 16]
	mov rdi, [rdi + 8]
	mov rsi, CODE_DATA_WS
	call swap_ws
	
	; save callee-save registers
	push rbp
	push r12
	push r13
	push r14
	push r15

	; push iretq data	
	mov rax, rsp
	push qword KERNEL_DS	; TODO fix for change in priv level
	push rax
	pushfq
	xor rax, rax
	mov ax, cs
	push rax
	mov rax, .return_here
	push rax
	
	; change old threads status to ready and save rsp
	mov rax, [current_thread]
	mov byte [rax + 1], 0
	mov rax, [rax + 8]
	mov [rax + 24], rsp

	; load the new rsp
	mov rax, [rbx + 8]
	mov rsp, [rax + 24]

	; signal that the interrupt is finished
	mov al, 0x20
	out 0x20, al

	; reset the timer
	mov al, 0xff
	out 0x40, al
	out 0x40, al
		
	iretq
.return_here:

	; restore callee-save registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp

.return:
	pop rbx
	POP_CALLER_REGS
	iretq

.return_early:
	mov al, 0x20
	out 0x20, al
	mov al, 0xff
	out 0x40, al
	out 0x40, al
	jmp .return
	

[section .data]

current_thread: dq 0
