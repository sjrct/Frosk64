//
// bufferutils.c
//
// written by naitsirhc, sjrct
//

#include <bufferutils.h>
#include <stdlib.h>
#include <frosk.h>

pixel_buffer create_buffer(int width, int height, pixel fill)
{
	int x, y;
	pixel_buffer pb;
	
	pb.width = width;
	pb.height = height;
	pb.buffer = malloc(sizeof(pixel*) * width);
	
	for (x = 0; x < width; x++) {
		pb.buffer[x] = malloc(sizeof(pixel) * height);
		for (y = 0; y < height; y++) {
			pb.buffer[x][y] = fill;
		}
	}
	
	return pb;
}

void resize_buffer(pixel_buffer * pb, int new_width, int new_height) {
	int i;
	
	if(new_width == pb->width && new_height == pb->height) {
		return;
	}
	
	if (new_width < pb->width) {
		for (i = 0; i < new_width; i++) {
			pb->buffer[i] = realloc(pb->buffer[i], new_height * sizeof(pixel));
		}
		
		for (; i < pb->width; i++) {
			free(pb->buffer[i]);
		}
	} else{
		for (i = 0; i < pb->width; i++) {
			pb->buffer[i] = realloc(pb->buffer[i], new_height * sizeof(pixel));
		}
		
		if (new_width > pb->width) {
			pb->buffer = realloc(pb->buffer, new_width * sizeof(pixel*));
			for (; i < new_width; i++) {
				pb->buffer[i] = malloc(new_height * sizeof(pixel));
			}
		}
	}

	pb->width = new_width;
	pb->height = new_height;
}

void free_buffer(pixel_buffer buff) {
	int i;	
	for(i = 0; i < buff.width; i++) {
		free(buff.buffer[i]);
	}
	
	free(buff.buffer);
	buff.buffer = NULL;
}

pixel_buffer copy_buffer(pixel_buffer opb) {
	int x, y;
	pixel_buffer pb;
	
	pb.width  = opb.width;
	pb.height = opb.height;

	pb.buffer = malloc(sizeof(pixel*) * pb.width);

	for (x = 0; x < pb.width; x++) {
		pb.buffer[x] = malloc(sizeof(pixel) * pb.height);

		for (y = 0; y < pb.height; y++) {
			pb.buffer[x][y] = opb.buffer[x][y];
		}
	}

	return pb;
}

char * linear_buffer(pixel_buffer pb)
{
	int x, y;
	char * b = malloc(pb.width * pb.height * 3);
	
	for (y = 0; y < pb.height; y++) {
		for (x = 0; x < pb.width; x++) {
			*((pixel*)(b + x * 3 + y * pb.width * 3)) = pb.buffer[x][y];
		}
	}
	
	return  b;
}

void send_buffer(pid_t pid, int port, pixel_buffer buffer) {
	int i;
	int j;
	send(pid, port, &buffer.width, sizeof(int));
	send(pid, port, &buffer.height, sizeof(int));
	
	for(i = 0; i < buffer.width; i++) {
		send(pid, port, buffer.buffer[i], sizeof(pixel) * buffer.height);
	}
}

pixel_buffer receive_buffer(pid_t pid, int port) {
	pixel_buffer buffer;
	int i;
	pixel p = { 0, 0, 0 };
	while(!receive(pid, port, &buffer.width, sizeof(int)));
	while(!receive(pid, port, &buffer.height, sizeof(int)));
	
	buffer = create_buffer(buffer.width, buffer.height, p);
	
	for(i = 0; i < buffer.width; i++) {
		while(!receive(pid, port, buffer.buffer[i], sizeof(pixel) * buffer.height));
	}
	return buffer;
}
