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

.read_again:
	in al, 0x60

	mov rcx, key_buf
	xor rdx, rdx
	mov edx, [key_buf_top]
	add rcx, rdx
	mov [rcx], al

	inc edx
	cmp edx, KEY_BUF_MAX_SIZE
	jne .no_circle
	xor edx, edx
.no_circle:
	mov [key_buf_top], edx

	cmp edx, [key_buf_bot]
	jne .not_full
	inc edx
	cmp edx, KEY_BUF_MAX_SIZE
	jne .no_circle2
	xor edx, edx
.no_circle2:
	mov [key_buf_bot], edx
.not_full:

;	in al, 0x64
;	test al, 1
;	jnz .read_again

	mov al, 0x20
	out 0x20, al

	pop rdi
	pop rdx
	pop rcx
	pop rax
	iretq
