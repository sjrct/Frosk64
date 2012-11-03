;
; rtc.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

extern ticks

rtc_int:
	inc qword [ticks]

	mov al, 0x20
	mov 0x20, al
	
	mov al, 0xC
	out 0x70, al
	in al, 0x71

	iretq
