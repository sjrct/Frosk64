;
; paging.asm
;
; written by sjrct

[bits 64]
[default rel]

[section .text]

%include "kerndef.h"

extern new_virt_page

global page_fault
page_fault:
	push rbp
	mov rbp, rsp
	PUSH_CALLER_REGS
	pushfq
	
	mov rsi, [rbp + 8]
	
	test rsi, 1
	jz .not_present
	
	; TODO not-privledged error
	jmp .return
	
.not_present:
	and rsi, 4
	or rsi, 3
	mov rdi, cr2
	call new_virt_page

.return:
	popfq
	POP_CALLER_REGS
	pop rbp
	add rsp, 8
	iretq
