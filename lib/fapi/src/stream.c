//
// stream.c
//
// written by sjrct
//

#include <stream.h>
#include <stdlib.h>
#include <string.h>

#define REALLOC_SIZE 0x200

stream_t stream_create(long src, long dst, int port, int is_buf)
{
	stream_t t;
	
	t.src = src;
	t.dst = dst;
	t.port = port;
	t.size = 0;

	if (dst == 0) is_buf = 1;
	if (is_buf) t.buf = malloc(REALLOC_SIZE);
	else t.buf = NULL;
	
	return t;
}

size_t stream_write(stream_t * stm, char * buf, size_t sz)
{
	if (stm->src != 0) return STREAM_ERROR;

	if (stm->buf == NULL) {
		if (stm->size % REALLOC_SIZE + sz > REALLOC_SIZE) {
			stm->buf = realloc(stm->buf, stm->size + REALLOC_SIZE);
		}
	
		memcpy(stm->buf + stm->size, buf, sz);
		stm->size += sz;
	} else {
		stm->buf = buf;
		stm->size = sz;
		stream_flush(stm);
		stm->buf = NULL;
		stm->size = 0;
	}
	
	return sz;
}

size_t stream_read(stream_t * stm, char * buf, size_t sz)
{
	size_t asz;

	if (stm->dst != 0) return STREAM_ERROR;
	
	asz = sz < stm->size ? sz : stm->size;

	memcpy(stm->buf + stm->size, buf, asz);
	stm->size -= sz;
	
	return asz;
}

void stream_flush(stream_t * s)
{
	// TODO
}

void stream_destroy(stream_t * s)
{
	free(s->buf);
	s->buf = NULL;
}
