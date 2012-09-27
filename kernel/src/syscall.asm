;
; syscall.asm
;
; written by sjrct
;

[bits 64]
[default rel]

%include "kerndef.h"
%include "syscalls_ls.asm"

extern register_int

[section .text]

global init_syscalls
init_syscalls:
	mov rdi, SYSCALL_INT
	mov rsi, syscall
	mov rdx, KERNEL_CS
	mov rcx, 0x8F
	call register_int
	ret


global syscall
syscall:
	cmp rax, syscalls_count
	jae .return
	
	lea r11, [syscalls_ls]
	mov rax, [r11 + rax * 8]
	call rax
.return:
	iretq
