;
; mouse.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

extern mousex, mousey


%macro WAIT_WRITE 0
%%loop:
	in al, 0x64
	test al, 0x2
	jz %%loop
%endmacro


%macro WAIT_READ 0
%%loop:
	in al, 0x64
	test al, 0x1
	jnz %%loop
%endmacro


mouse_func:
	mov al, 0xD4
	out 0x64, al

.loop:
	WAIT_READ
	in al, 0x50
	cmp al, 0xFA
	jne .loop
	
	WAIT_WRITE
	mov al, cl
	out 0x60, al
	ret
	

global init_mouse_asm
init_mouse_asm:
	mov cl, 0xF4
	call mouse_func
	ret


global mouse_move
mouse_move:
	xor ecx, ecx
	xor eax, eax
	
	in al, 0x60
	mov dl, al
	in al, 0x60
	mov cl, al
	in al, 0x60
	
	test dl, 0xc0
	jnz .return

	test dl, 0x10
	jz .skipx
	or ecx, 0xffffff00
.skipx:
	
	test dl, 0x20
	jz .skipy
	or eax, 0xffffff00
.skipy:
	
	add [mousex], ecx
	add [mousey], eax
	
.return:
	mov al, 0x20
	out 0x20, al

	iretq
