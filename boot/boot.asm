
; boot.asm
;
; written by sjrct
;

%include "bootdef.asm"

[org BOOT1_LOC]
[bits 16]

	jmp 0:start
start:
	; setup stack (under bootloader for now)
	mov sp, BOOT1_LOC
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov es, ax

	; TODO: get the offset for the partition

	; load the second stage bootloader
	mov cx, 4
read_again:
	mov ah, 0x42
	mov si, dap
	int 0x13
	jnc read_good

	loop read_again
	jmp read_error
read_good:

	jmp boot2_start

; an error has occured!
read_error:
	; TODO
	cli
	hlt

;;;;;; 1st data section ;;;;;

; data address packet for 2nd stage boot loader
dap:
	db 0x10
	db 0
	dw 7
	dd BOOT2_LOC
	dq 1

; mark as boot sector
times 0x1fe - ($ - $$) db 0
dw 0xaa55

%include "boot/boot2.asm"	
