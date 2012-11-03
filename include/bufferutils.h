//
// bufferutils.h
//
// written by naitsirhc
//

#ifndef _BUFFERUTILS_H_
#define _BUFFERUTILS_H_

#pragma pack(push, 1)
typedef struct {
	unsigned char r ,g ,b;
} pixel;
#pragma pack(pop)

typedef struct {
	pixel ** buffer;
	int width;
	int height;
} pixel_buffer;

pixel_buffer create_buffer(int width, int height, pixel fill);
void resize_buffer(pixel_buffer buffer, int new_width, int new_height);
pixel_buffer copy_buffer(pixel_buffer buffer);
void free_buffer(pixel_buffer buff);
char * linear_buffer(pixel_buffer pb);

#endif
