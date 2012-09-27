;
; syscalls_ls.asm
;
; written by sjrct
;

extern driver_call
extern exec_fbe
extern fs_aquire
extern fs_release
extern fs_read
extern fs_get_size
extern gr_draw
extern gr_get_info

global syscalls_ls
syscalls_ls:
	dq driver_call ; 0
	dq exec_fbe
	dq fs_aquire
	dq fs_release
	dq fs_read
	dq fs_get_size ; 5
	dq gr_draw
	dq gr_get_info

syscalls_count: equ ($ - syscalls_ls) / 8
