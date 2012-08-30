//
// io.h
//
// written by sjrct
//

#ifndef IO_H
#define IO_H

#include "cdef.h"
#include "kernobj.h"

//
// open_file: opens a file
//
// Parameters:
//   const char * filename
//
// Returns: A kernel object containing the file data
//
kern_obj * open_file(const char *);

//
// close_file: Closes a file
//
// Parameters:
//   kern_obj * file
//
void close_file(kern_obj *);

//
// write_file: Writes to a file
//
// Parameters:
//   char * source
//   uint element_size
//   uint element_count
//   kern_obj * file
//
// Returns: Elements written
//
uint write_file(char *, uint, uint, kern_obj *);

//
// read_file: Read from a file
//
// Parameters:
//   char * destination
//   uint element_size
//   uint element_count
//   kern_obj * file
//
// Returns: Elements read
//
uint read_file(char *, uint, uint, kern_obj *);

#endif
