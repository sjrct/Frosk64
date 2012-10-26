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
	mov rax, USPACE_TOP - 4
	mov qword [TSS_SEG_LOC + 4], rax
	mov dword [TSS_SEG_LOC + 0x64], 0x68
	mov ax, TSS_SEL
	ltr ax
	ret
