//
// assert.h
//
// written by sjrct
//

#include <stdio.h>
#include <stdlib.h>

#undef assert
#ifdef NDEBUG
  #define assert(T) ((void)0)
#else
  #define assert(T) if (!(T)) { \
    fprintf(stderr, "Assertion \"T\" failed in file %s line %d.\n", __FILE__, __LINE__); \
    abort(); \
  }
#endif
