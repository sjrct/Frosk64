//
// stdint.h
//
// written by sjrct
//

#ifndef _STDINT_H_
#define _STDINT_H_

#define _HAVE_INT_OF(B,T,N,X,U) \
	typedef signed T int # B # _t; \
	typedef unsigned T uint # B # _t; \
	typedef int # B #_t int_least # B # _t; \
	typedef int # B #_t int_fast # B # _t; \
	typedef uint # B #_t uint_least # B # _t; \
	typedef uint # B #_t uint_fast # B # _t; \
	enum { \
		INT # B # _MIN = N,\
		INT # B # _MAX = X,\
		UINT # B # _MAX = U,\
		INT_LEAST # B # _MIN = N,\
		INT_LEAST # B # _MAX = X,\
		UINT_LEAST # B # _MAX = U,\
		INT_FAST # B # _MIN = N,\
		INT_FAST # B # _MAX = X,\
		UINT_FAST # B # _MAX = U,\
	}

_HAVE_INT_OF(8, char, 0x80, 0x7F, 0xFF);
_HAVE_INT_OF(16, short, 0x8000, 0x7FFF, 0xFFFF);
_HAVE_INT_OF(32, int, 0x80000000, 0x7FFFFFFF, 0xFFFFFFFF);
_HAVE_INT_OF(64, long, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);

typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

typedef int64_t intptr_t;
typedef uint64_t uintptr_t;

#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX

#define SIZE_MAX    UINT64_MAX
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX

#undef _HAVE_INT_OF

#endif
