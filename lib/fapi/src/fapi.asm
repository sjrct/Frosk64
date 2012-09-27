;
; fapi.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

%macro SYSCALL 2
global _syscall%1_
_syscall%1_:
	mov rax, %2
	int 0x40
	ret
%endmacro

SYSCALL 0, rdi
SYSCALL 1, rsi
SYSCALL 2, rdx
SYSCALL 3, rcx
SYSCALL 4, r8
SYSCALL 5, r9
