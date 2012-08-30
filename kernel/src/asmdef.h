;
; asmdef.h
;
; written by sjrct
;

%define IO_WAIT out 0x80, al

%macro PUSH_CALLER_REGS 0
	push rax
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
%endmacro

%macro POP_CALLER_REGS 0
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rax
%endmacro
