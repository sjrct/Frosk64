;
; preamble.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

extern main

global _start
_start:
	mov ax, 0x23	; user ds
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call main
	
	; TODO: system call to end_thread
	jmp $
