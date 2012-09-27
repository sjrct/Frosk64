;
; lock.asm
;
; written by sjrct
;

[bits 64]
[default rel]

[section .text]

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
