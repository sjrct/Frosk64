//
// stream.h
//
// written by sjrct
//

#ifndef _STREAM_H_
#define _STREAM_H_

#include <stddef.h>

typedef struct stream_t {
	long dst;
	long src;
	char * buf;
	size_t size;
	int port;
} stream_t;

#define STREAM_ERROR (-1)

stream_t stream_create(long, long, int, int);
size_t stream_write(stream_t *, char *, size_t);
size_t stream_read(stream_t *, char *, size_t);
void stream_flush(stream_t *);
void stream_destroy(stream_t *);

#endif
