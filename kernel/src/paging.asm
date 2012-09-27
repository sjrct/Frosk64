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
	jmp .return
	
.not_present:
	and rcx, 4
	or rcx, 3
	push rcx

	mov rdi, 1
	mov rsi, 0
	call alloc_pages

	pop rcx
	mov rdi, rax
	mov rsi, cr2
	mov rdx, 1
	call page_in

.return:
	popfq
	POP_CALLER_REGS
	pop rbp
	add rsp, 8
	iretq
