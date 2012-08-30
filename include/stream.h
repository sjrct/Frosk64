//
// stream.h
//
// written by sjrct
//

#ifndef _STREAM_H_
#define _STREAM_H_

#include <stddef.h>

struct stream_data_t;

typedef struct stream_t {
	enum {
		STREAM_STDOUT,
		STREAM_STDIN,
		STREAM_STDERR,
		STREAM_FILE,
		STREAM_RAW
	} type;

	union stream_data_u data;
} stream_t;

#define STREAM_READ  1
#define STREAM_WRITE 2

#define STREAM_ERROR_NONE                0
#define STREAM_ERROR_NOT_WRITABLE        1
#define STREAM_ERROR_NOT_READABLE        2
#define STREAM_ERROR_FILE_DOES_NOT_EXIST 3
#define STREAM_ERROR_RAW_SIZE_INVALID    4

int file_stream_create(stream_t *, const char *, unsigned);
int raw_stream_create(stream_t *, size_t, unsigned);
int stream_write(stream_t *, char *, size_t);
int stream_read(stream_t *, char *, size_t);

extern stream_t * stream_stdin;
extern stream_t * stream_stdout;
extern stream_t * stream_stderr;

#endif
