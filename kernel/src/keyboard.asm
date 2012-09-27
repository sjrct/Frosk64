;
; keyboard.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

%include "kerndef.h"

extern key_buf
extern key_buf_bot
extern key_buf_top

global key_int
key_int:
	push rax
	push rcx
	push rdx
	push rdi

	in al, 0x60

	mov edx, [key_buf_bot]
	inc edx
	cmp edx, KEY_BUF_MAX_SIZE
	jne .no_circle
	xor edx, edx
.no_circle:

	mov rcx, [key_buf]
	xor rdi, rdi
	mov edi, [key_buf_bot]
	add rcx, rdi
	mov [rcx], al
	mov [key_buf_bot], edx

	cmp edx, [key_buf_top]
	jne .not_full
	mov eax, [key_buf_top]
	cmp eax, KEY_BUF_MAX_SIZE
	jne .no_circle2
	xor eax, eax
.no_circle2:
	mov [key_buf_top], eax
.not_full:

	mov al, 0x20
	out 0x20, al

	pop rdi
	pop rdx
	pop rcx
	pop rax
	iretq
