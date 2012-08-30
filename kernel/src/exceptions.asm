;
; exceptions.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

extern outs

global div0_fault
div0_fault:
	mov rdi, div0_msg
	call outs
	iretq
	
global gp_fault
gp_fault:
	mov rdi, gpf_msg
	call outs
	iretq
	
global dbl_fault
dbl_fault:
	mov rdi, dbl_msg
	call outs
.lp:	hlt
	jmp .lp

global undef_int
undef_int:
	mov rdi, uint_msg
	call outs
	iretq

[section .rodata]

div0_msg: db `Division by 0 fault\n`, 0
gpf_msg:  db `General protection fault\n`, 0
dbl_msg:  db `Double fault!!\n`, 0
uint_msg: db `Undefined interrupt error\n`, 0
