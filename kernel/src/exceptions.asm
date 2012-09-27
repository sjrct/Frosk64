;
; exceptions.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

extern driver_call

global div0_fault
div0_fault:
	mov rdi, 3
	mov rsi, 3
	mov rdx, div0_msg
	call driver_call
.lp: hlt
	jmp .lp


global gp_fault
gp_fault:
	mov rdi, 3
	mov rsi, 3
	mov rdx, gpf_msg
	call driver_call
.lp: hlt
	jmp .lp


global dbl_fault
dbl_fault:
	mov rdi, 3
	mov rsi, 3
	mov rdx, dbl_msg
	call driver_call
.lp:	hlt
	jmp .lp


global undef_int
undef_int:
	mov rdi, 3
	mov rsi, 3
	mov rdx, uint_msg
	call driver_call
.lp: hlt
	jmp .lp


[section .rodata]

div0_msg: db `Division by 0 fault\n`, 0
gpf_msg:  db `General protection fault\n`, 0
dbl_msg:  db `Double fault!!\n`, 0
uint_msg: db `Undefined interrupt error\n`, 0
