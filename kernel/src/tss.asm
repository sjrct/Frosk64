;
; tss.asm
;
; written by sjrct
;

[bits 64]

%include "kerndef.h"

[section .text]

global init_tss
init_tss:
	mov qword [TSS_SEG_LOC + 4], STACK_LOC
	mov dword [TSS_SEG_LOC + 0x64], 0x68
	mov r8, [TSS_SEG_LOC + 0x48]
	mov r9, [TSS_SEG_LOC + 0x50]
	mov r10, [TSS_SEG_LOC + 0x58]
	mov ax, 0x30
	ltr ax
	jmp $
	ret
