//
// fui.c
//
// written by sjrct
//

#include <frosk.h>

int main()
{
	int x, y;
	static unsigned char buf[100][100][3];

	for (y = 0; y < 100; y++) {
		for (x = 0; x < 100; x++) {
			buf[x][y][0] = x % 255;
			buf[x][y][1] = y % 255;
			buf[x][y][2] = (x + y) % 255;
		}
	}

	gr_draw(buf, 20, 20, 100, 100);

	for (y = 0; y < 100; y++) {
		for (x = 0; x < 100; x++) {
			buf[x][y][0] = x % 255;
			buf[x][y][1] = (100 - y) % 255;
			buf[x][y][2] = (x + (100 - y)) % 255;
		}
	}

	gr_draw(buf, 120, 20, 100, 100);

	while (1);
	return 0;
}
