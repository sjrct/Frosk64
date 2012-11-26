;
; mouse.asm
;
; written by sjrct
;

[bits 64]
[default rel]

%include "kerndef.h"

[section .text]

extern mousex, mousey

; wait until status indicates free buffer space
%macro WAIT_WRITE 0
%%loop:
	in al, 0x64
	test al, 0x2
	jnz %%loop
%endmacro


; wait until status indicates presence of data
%macro WAIT_READ 0
%%loop:
	in al, 0x64
	test al, 0x1
	jz %%loop
%endmacro


; generic mouse function (cl = func id)
mouse_func:
	mov al, 0xD4
	out 0x64, al

	WAIT_WRITE
	mov al, cl
	out 0x60, al

	WAIT_READ
	in al, 0x60
	cmp al, 0xFA
;	jne mouse_func
	
	ret
	

; reads bytes from 60h while stuff is there
clear_input:
	in al, 0x64
	test al, 0x1
	jz .done
	in al, 0x60
	jmp clear_input
.done:
	ret


; assembly portion of mouse initialization
global init_mouse_asm
init_mouse_asm:
	; enable mouse port
	mov al, 0xA8
	out 0x64, al

	; set enable irq12 bit, clear disable mouse clock bit
;	mov al, 0x20
;	out 0x64, al
;	WAIT_READ
;	in al, 0x60
	
;	or al, 0x2
;	and al, ~0x20
;	mov cl, al
	
;	mov al, 0x60
;	out 0x64, al
;	WAIT_WRITE
;	mov al, cl
;	out 0x60, al
	
;	IO_WAIT
;	call clear_input

	; set defaults
	mov cl, 0xF6
	call mouse_func
	
	; enable mouse
	mov cl, 0xF4
	call mouse_func
	
	; set remote mode
	mov cl, 0xF0
	call mouse_func
	ret
	
	
; get mouse position in remote system
global get_mouse_pos
get_mouse_pos:
	mov cl, 0xEB
	call mouse_func
	
	xor ah, ah
	
	WAIT_READ
	in al, 0x60 ; status
	in al, 0x60 ; ignored

	in al, 0x60 ; x data
	add [mousex], ax
	in al, 0x60 ; ignored

	in al, 0x60 ; y data
	add [mousey], ax
	in al, 0x60 ; ignored

	in al, 0x60 ; z data
	in al, 0x60 ; ignored

	mov eax, [mousex]
	mov [rdi], eax
	mov eax, [mousey]
	mov [rsi], eax

	ret


; interrupt handler for IRQ12 (PS2 mouse int)
global mouse_move
mouse_move:
	mov dword [mousex], 0x100
	iretq

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
