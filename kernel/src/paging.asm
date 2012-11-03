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
	SWITCH_SEG_SELS
	pushfq

	mov rcx, [rbp + 8]

	test rcx, 1
	jz .not_present

	; TODO not-privledged error
	mov rax, 0xc0debeef
	jmp $
	jmp .return

.not_present:
;	and rcx, 4
;	or rcx, 3
;	push rcx

	; allocate new page
	mov rdi, 1
	mov rsi, 0
	call alloc_pages
	push rax

	; add page to current applicable workspace
	;add_pgs_to_ws(kern_obj * o, ulong vbase, ulong pbase, uint size, ushort fl)
	mov rsi, cr2
	and rsi, ~0xfff
	
	mov r11, USPACE_TOP / 2
	cmp rsi, r11
	jbe .code
	mov rdi, [cur_ws + STACK_WS * 8]
	jmp .stack
.code:
	mov rdi, [cur_ws + CODE_DATA_WS * 8]
.stack:
	
	mov rdx, rax
	mov rcx, 0x1000
	mov r8, 7
	call add_pgs_to_ws

	; page in the new page
	pop rdi
;	pop rcx
	mov rcx, 7
	mov rsi, cr2
	mov rdx, 0x1000
	call page_in

.return:
	popfq
	RESTORE_SEG_SELS
	POP_CALLER_REGS
	pop rbp
	add rsp, 8
	iretq
