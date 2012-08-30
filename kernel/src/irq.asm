;
; irq.asm
;
; written by sjrct
;

%include "kerndef.h"

%define PIC1 0x20
%define PIC2 0xA0

[bits 64]
[default rel]

[section .text]

global init_irqs
init_irqs:
	mov al, 0x11
	out PIC1, al
	IO_WAIT
	out PIC2, al
	IO_WAIT

	mov al, IRQ0
	out PIC1 + 1, al
	IO_WAIT
	add al, 8
	out PIC2 + 1, al
	IO_WAIT

	mov al, 4
	out PIC1 + 1, al
	IO_WAIT
	mov al, 2
	out PIC2 + 1, al
	IO_WAIT

	mov al, 1
	out PIC1 + 1, al
	IO_WAIT
	out PIC2 + 1, al

	mov al, 0xff
	out PIC1 + 1, al
	out PIC2 + 1, al

	ret
	

global enable_irq
enable_irq:
	cmp di, 7
	ja .slave	
	mov dx, PIC1 + 1
	jmp .master
.slave:
	mov dx, PIC2 + 1
	sub dx, 8
.master:

	mov ecx, edi
	mov si, 1
	shl esi, cl
	not esi
	
	in al, dx
	and eax, esi
	out dx, al
	
	ret


global disable_irq
disable_irq:
	cmp di, 7
	ja .slave	
	mov dx, PIC1 + 1
	jmp .master
.slave:
	mov dx, PIC2 + 1
	sub dx, 8
.master:

	mov ecx, edi
	mov si, 1
	shl esi, cl

	in al, dx
	or eax, esi
	out dx, al

	ret
