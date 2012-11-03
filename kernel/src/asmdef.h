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

%macro SWITCH_SEG_SELS 0
	mov ax, ds
	push ax
	
	mov ax, es
	push ax
	
	mov ax, fs
	push ax
	
	mov ax, gs
	push ax
	
	mov ax, KERNEL_DS
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
%endmacro

%macro RESTORE_SEG_SELS 0
	pop ax
	mov gs, ax

	pop ax
	mov fs, ax

	pop ax
	mov es, ax

	pop ax
	mov ds, ax
%endmacro
