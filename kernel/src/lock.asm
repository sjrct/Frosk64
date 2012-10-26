;
; lock.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

global getlock
getlock:
	call trylock
	test rax, rax
	jz getlock
	ret


global trylock
trylock:
	xor rax, rax
	lock bts [rdi], rsi
	jc .return
	inc rax
.return:
	ret


global unlock
unlock:
	xor rax, rax
	lock btc [rdi], rsi
	jnc .return
	inc rax
.return:
	ret
