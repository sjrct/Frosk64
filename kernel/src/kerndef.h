;//
;//  kerndef.h
;//
;//  written by sjrct
;// 
;//  should work as NASM or C include
;//

; /*
%ifndef KERNDEF_H
%define KERNDEF_H

; Assembly only
%include "asmdef.h"
%define define(X,Y) X: equ Y

%if 0
; */
#ifndef KERNDEF_H
#define KERNDEF_H

// C Only
#include "cdef.h"
#define define(X,Y) enum { X = Y };

; /*
%endif
; */


; ////////// Actual 'definitions' here //////////
define( MEM_MAP_LOC,  0x500 )
define( FST0_LOC,     0x1000 )
define( PML4T_LOC,    0x2000 )
define( KPDPT_LOC,    0x3000 )
define( KPDT0_LOC,    0x4000 )
define( KPT0_0_LOC,   0x5000 )
define( TPDPT_LOC,    0x6000 )
define( VM_INFO_LOC,  0x7000 )
define( PARTIT_LOC,   0x7E00 )
define( DISK_INF_LOC, 0x7E04 )
define( TPDT0_LOC,    0x9000 )
define( TPT0_0_LOC,   0xA000 )
define( TSS_SEG_LOC,  0xB000 )
define( KERNEL_LOC,   0xF000 )
define( STACK_LOC,    0x80000 )
define( KSPACE_LOC,   0xFFFF800000000000 )
define( KOBJ_PG0_LOC, 0xFFFF80000000C000 )

define( USPACE_BOT,   0x100000 )
define( USPACE_TOP,   0x200000 )

define( NON_CANON_SIZE, 0xFFFF000000000000 )


define( KERNEL_CS, 0x08 )
define( KERNEL_DS, 0x10 )
define( USER_CS,   0x18 )
define( USER_DS,   0x20 )
define( TSS_SEL,   0x28 )

define( CODE_DATA_WS, 0 )
define( STACK_WS,     1 )

define( SYSCALL_INT, 0x40 )
define( IRQ0, 0x80 )

define( KEY_BUF_MAX_SIZE, 0x200 )

;// 1024x768x256
define( GRAPHICS_MODE, 0x4118 )

; /*
%if 0
; */
#endif
; /*
%endif
%endif
; */
