;
; boot2.asm
;
; wrritten by sjrct
;

[bits 16]

boot2_start:
	cli
	cld
	
	mov [DRIVE_LOC], dl

;	mov ax, 0x2401
;	int 0x15

	; enable a20 line
	call a20_wait_2
	mov al, 0xAD
	out 0x64, al

	call a20_wait_2
	mov al, 0xD0
	out 0x64, al

	call a20_wait_1
	in al, 0x60
	mov ah, al

	call a20_wait_2
	mov al, 0xD1
	out 0x64, al

	call a20_wait_2
	mov al, ah
	or al, 2
	out 0x60, al

	call a20_wait_2
	mov al, 0xAE
	out 0x64, al

	; detect memory
	push edx
	mov di, MEM_MAP_LOC + 8
	mov edx, 0x534d4150 ;~~~MAGUS!~~~
	xor ebx, ebx
mem_map_loop:
	mov eax, 0xe820
	mov ecx, 24
	int 0x15
	jc .break
	test ebx, ebx
	jz .break
	
	mov eax, [di + 8]
	test eax, eax
	jne .not_empty
	mov eax, [di + 12]
	test eax, eax
	je mem_map_loop
.not_empty:
	
	add di, 24
	jmp mem_map_loop
.break:
	cmp di, MEM_MAP_LOC + 8
	je error16
	mov [MEM_MAP_LOC], di
	pop edx

	; read primary fs block
	mov si, dap2
	call do_read

	; check that is correct file system
	mov ax, [FST0_LOC]
	cmp ax, 0xF301
	je is_good_fs
	jmp error16
is_good_fs:

	; locate kernel file in predirectory
	; TODO kernel not found code
	mov bx, [FST0_LOC + 8]
	call resolve_fst0_ptr	
	add bx, 12
search_for_kernel:
	mov bx, [bx]
	call resolve_fst0_ptr
	mov si, bx
	mov bx, [bx + 8]
	call resolve_fst0_ptr
	mov cl, [bx + 1]
	cmp cl, kernel_name_length
	jne not_kernel

	add bx, 2
	mov di, kernel_name
	and cx, 0xff
check_string_loop:
	mov al, [bx]
	cmp al, [di]
	jne not_kernel
	inc bx
	inc di
	loop check_string_loop
	jmp found_kernel

not_kernel:
	mov bx, si
	add bx, 0x10
	jmp search_for_kernel
found_kernel:
	mov bx, si
	mov bx, [bx + 0xC]
	call resolve_fst0_ptr

	; load the kernel file
	; TODO: fix data loss from bitshifts
	mov ax, [bx + 4]
	shl ax, 3
	mov [dap2 + 2], ax
	mov dword [dap2 + 4], KERNEL_LOC
	mov eax, [bx + 8]
	shl eax, 3
	mov [dap2 + 8], eax
	shl eax, 3
	mov eax, [bx + 12]
	mov [dap2 + 12], eax
	mov si, dap2
	call do_read

	; switch to protected mode
	cli
	lgdt [gdt32]

	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp KERNEL_CS:protected_code_start

; wait functions for enabling the a20 line
a20_wait_1:
	in al, 0x64
	test al, 1
	jz a20_wait_2
	ret

a20_wait_2:
	in al, 0x64
	test al, 2
	jnz a20_wait_2
	ret

; raw/ledgit (in/out) pointer should be in bx
resolve_fst0_ptr:
	and bx, 0xff
	mov ax, bx
	shl ax, 3
	shl bx, 4
	add bx, ax
	add bx, FST0_LOC
	ret

error16:
	mov ax, 0xb800
	mov es, ax
	xor di, di
	mov si, error16_msg
	mov cx, error16_msg_len
	rep movsb
.loop:
	hlt
	jmp .loop

; the dap's address should be in si
do_read:
	mov bx, si
	mov cx, 4
.read_loop:
	mov ah, 0x42
	int 0x13
	jnc .read_good

	mov si, bx
	loop .read_loop
	jmp error16
.read_good:
	ret


;;;;; 32-bit code section ;;;;;

[bits 32]

protected_code_start:
	; setup seg registers
	mov ax, KERNEL_DS
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	; setup paging
	xor eax, eax
	mov edi, PML4T_LOC
	mov ecx, 0x1000
	rep stosd
	
	mov dword [PML4T_LOC + 0x100 * 8], KPDPT_LOC | 3
	mov dword [KPDPT_LOC], KPDT0_LOC | 3
	mov dword [KPDT0_LOC], KPT0_0_LOC | 3
	mov dword [KPDPT_LOC + 4], 0
	mov dword [KPDT0_LOC + 4], 0

	mov dword [PML4T_LOC], TPDPT_LOC | 3
	mov dword [TPDPT_LOC], TPDT0_LOC | 3
	mov dword [TPDT0_LOC], TPT0_0_LOC | 3
	mov dword [PML4T_LOC + 4], 0
	mov dword [TPDPT_LOC + 4], 0
	mov dword [TPDT0_LOC + 4], 0

	mov ecx, 0x1FF
pg_loop:
	dec ecx
	mov eax, ecx
	shl eax, 12
	or eax, 3
	mov [KPT0_0_LOC + ecx * 8], eax
	mov [TPT0_0_LOC + ecx * 8], eax
	mov dword [KPT0_0_LOC + ecx * 8 + 4], 0
	mov dword [TPT0_0_LOC + ecx * 8 + 4], 0
	test ecx, ecx
	jne pg_loop

	; enable long mode and paging
	mov eax, cr4
	or al, 0x20
	mov cr4, eax
	
	mov eax, PML4T_LOC
	mov cr3, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x100
	wrmsr
	
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	
	lgdt [gdt64]

	jmp KERNEL_CS:long_code_start


;;;;; 64-bit code section ;;;;;

[bits 64]

long_code_start:
	cli
	mov rsp, STACK_LOC + KSPACE_LOC
	jmp KERNEL_LOC + KSPACE_LOC


;;;;; data section ;;;;;

; kernel name should be less than 18 characters long
kernel_name:
	db 'kernel.bin'
kernel_name_length: equ $ - kernel_name

error16_msg:
	db 'E', 4, 'r', 4, 'r', 4, 'o', 4, 'r', 4
error16_msg_len: equ $ - error16_msg

; original contains data for fst0
dap2:
	db 0x10
	db 0
	dw 8
	dd FST0_LOC
	dq 8

; global descriptor table for 32-bit mode
gdt32:
	dw .end - .null_seg
	dd .null_seg
.null_seg:
	dw 0x0000           ; low-word of length
	dw 0x0              ; low-word of base address
	db 0x0              ; mid-byte of base address
	db 0x00             ; flags
	db 0x0              ; access/length
	db 0x0              ; high byte of base address
.code_seg:
	dw 0xffff
	dw 0x0
	db 0x0
	db 0x9a
	db 0xcf
	db 0x0
.data_seg:
	dw 0xffff
	dw 0x0
	db 0x0
	db 0x92
	db 0xcf
	db 0x0
.end:

; global descriptor table for 64 bit mode - still temporary
gdt64:
	dw .end - .null_seg
	dd .null_seg
.null_seg:
	dq 0x0000000000000000
.code_seg:
	dq 0x0020980000000000
.data_seg:
	dq 0x0000920000000000
.end:

times 0x1000 - ($ - $$) db 0
