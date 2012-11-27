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
extern send
extern poll
extern receive
extern reg_wsys
extern get_wsys
extern unreg_wsys
extern get_current_proc
extern get_mouse_pos
extern kb_read
extern write_serial
extern vesa_fill

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
	dq send
	dq poll
	dq receive ; 10
	dq reg_wsys
	dq get_wsys
	dq unreg_wsys
	dq get_current_proc
	dq get_mouse_pos ; 15
	dq kb_read
	dq write_serial
	dq vesa_fill

syscalls_count: equ ($ - syscalls_ls) / 8
