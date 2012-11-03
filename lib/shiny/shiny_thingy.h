#define SHINY_CONTAINER(X) (X.ptr.container)
#define SHINY_BUFFER(X) (X.ptr.buffer)

typedef struct {
	int x, y;
} shiny_loc;
typedef struct {
	int width, height;
} shiny_size;

typedef struct {
	shiny_size size;
	shiny_loc loc;
	
	enum {
		CONTAINER = 0,
		BUFFER,
	} type;
	
	union {
		buffer_thingy * buffer;
		container_thingy * container;
	} ptr;
	
	window_handle win_handle;
} shiny_thingy;


void draw(shiny_thingy *, shiny_loc);
