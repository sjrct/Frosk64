//
// sout.h
//
// written by sjrct
//

#ifndef SOUT_H
#define SOUT_H

#include "cdef.h"

//
// cls: Clears the screen
//
// Parameters:
//   char color
//
// All color bytes (odd bytes) are set to the specified color.
// All character bytes (even bytes) are set to zero.
// This function is automaticly called with '0x0F' if it has not been called
//  before another operation.
//
void cls(char);

//
// outnl: Outputs a single new line character
//
void outnl();

//
// outc: Outputs a single character to the screen
//
// Parameters:
//   char character
//
// If the character is a newline ('\n') then it wraps to outnl()
//
void outc(char);

//
// outs: Output a string to the screen
//
// Parameters:
//   const char * string
//
// The string must be null-terminated
//
void outs(const char *);

//
// outn: Outputs a unsigned number to the screen
//
// Parameters:
//   ulong value
//   int   base
//
// The base is the number of digits to use when displaying the number, for
//  example, 10 is decimal, 16 is hexdecimal, 3 is ternary, etc. The base 
//  should be greater than 0 and be less than or equal to 36. The characters
//  0-9 are used as digits, then A-Z.
//
void outn(ulong, int);

//
// outl: Outputs a signed number to the screen
//
// Parameters:
//   ulong value
//   int   base
//
// Behaves exactly like outn() but if the number is negative it will output a
//  minus sign ('-') first.
//
void outl(long, int);

#endif
