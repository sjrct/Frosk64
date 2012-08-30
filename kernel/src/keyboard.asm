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
extern key_buf_size

global key_int
key_int:
	push rax
	push rdx

	in al, 0x60
	xor rdx, rdx
	mov edx, [key_buf_size]
	cmp edx, KEY_BUF_MAX_SIZE
	je .buffer_full

	add rdx, [key_buf]
	mov [rdx], al
	inc dword [key_buf_size]
.buffer_full:
	; TODO handle the buffer overflowing

	mov al, 0x20
	out 0x20, al

	pop rdx
	pop rax
	iretq
