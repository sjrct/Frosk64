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

#define driver_call(X,Y,Z)  _syscall3_(X, Y, (long)(Z), 0)
#define exec_fbe(X,Y,Z,T,V) _syscall5_((long)(X), Y, (long)(Z), T, V, 1)
#define fs_aquire(X, Y, Z)  (handle_t)_syscall3_((long)(X), Y, Z, 2)
#define fs_release(X)       _syscall1_(X, 3)
#define fs_read(X, Y, Z, T) _syscall4_(X, (long)(Y), Z, T, 4)
#define fs_get_size(X)      _syscall1_(X, 5)
#define gr_draw(B,X,Y,W,H)  _syscall5_((long)(B), X, Y, W, H, 6)
#define gr_get_info()       _syscall1_(7)

#endif
