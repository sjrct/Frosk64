//
// fui.c
//
// written by sjrct
//

#include <frosk.h>

int main()
{
	int x, y;
	struct {
		unsigned char r, g, b;
	} buf[100][100];
	
	
	for (x = 0; x < 100; x++) {
		for (y = 0; y < 100; y++) {
			buf[x][y].r = x * y % 255;
			buf[x][y].g = (x * 2 + y) % 255;
			buf[x][y].b = (x + y * 2) % 255;
		}
	}
	
	gr_draw(buf, 20, 20, 100, 100);
	gr_draw(buf, 120, 20, 100, 100);

	while (1);
	return 0;
}
