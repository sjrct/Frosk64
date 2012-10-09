;
; paging.asm
;
; written by sjrct

[bits 64]
[default rel]

[section .text]

%include "kerndef.h"

extern alloc_pages
extern page_in
extern add_pgs_to_ws

extern cur_ws

global page_fault
page_fault:
	push rbp
	mov rbp, rsp
	PUSH_CALLER_REGS
	pushfq
	
	mov rcx, [rbp + 8]
	
	test rcx, 1
	jz .not_present

	; TODO not-privledged error
	mov rax, 0xc0debeef
	jmp $
	jmp .return
	
.not_present:
	and rcx, 4
	or rcx, 3
	push rcx

	mov rdi, 1
	mov rsi, 0
	call alloc_pages
	push rax
	
	mov rdi, [cur_ws]
	mov rsi, cr2
	and rsi, ~0xfff
	mov rdx, rax
	mov rcx, 0x1000
	mov r8, CODE_DATA_WS
	call add_pgs_to_ws

	pop rdi
	pop rcx
	mov rsi, cr2
	mov rdx, 0x1000
	call page_in

.return:
	popfq
	POP_CALLER_REGS
	pop rbp
	add rsp, 8
	iretq
