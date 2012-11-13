//
// fapi.h
//
// written by sjrct
//

#ifndef _FAPI_H_
#define _FAPI_H_

long _syscall0_(long);
long _syscall1_(long, long);
long _syscall2_(long, long, long);
long _syscall3_(long, long, long, long);
long _syscall4_(long, long, long, long, long);
long _syscall5_(long, long, long, long, long, long);

typedef long handle_t;
typedef long pid_t;

#define driver_call(X,Y,Z)  _syscall3_(X, Y, (long)(Z), 0)
#define exec_fbe(X,Y,Z,T,V) _syscall5_((long)(X), Y, (long)(Z), T, V, 1)
#define fs_aquire(X, Y, Z)  (handle_t)_syscall3_((long)(X), Y, Z, 2)
#define fs_release(X)       _syscall1_(X, 3)
#define fs_read(X, Y, Z, T) _syscall4_(X, (long)(Y), Z, T, 4)
#define fs_get_size(X)      _syscall1_(X, 5)
#define gr_draw(B,X,Y,W,H)  _syscall5_((long)(B), X, Y, W, H, 6)
#define gr_get_info()       _syscall0_(7)
#define send(I,P,B,S)       _syscall4_(I, P, (long)(B), S, 8)
#define poll(P)             _syscall1_(P, 9)
#define receive(I,P,B,S)    _syscall4_(I, P, (long)(B), S, 10)
#define reg_esys()          _syscall0_(11)
#define get_esys()          _syscall0_(12)
#define unreg_esys()        _syscall0_(13)
#define get_current_proc()  _syscall0_(14)
#define get_mouse_pos(X,Y)  _syscall2_((ulong)(X), (ulong)(Y), 15)
#define kb_read(B,S)        _syscall2_((ulong)(B),S,16)

#endif
