//
// fui.c
//
// written by sjrct
//

#include <frosk.h>
#include <rusk.h>
#include <stdlib.h>

void sendint(pid_t pid, int port, int n)
{
	send(pid, port, &n, sizeof(int));
}

int main()
{
	window_handle handle;
	api_window win;
	pid_t wsys;
	
	while (!(wsys = get_wsys()));

	sendint(wsys, RUSK_COMM_PORT, RUSK_CREATE_WINDOW);
	
	win.width = win.height = 400;
	win.attributes = 0;
	send(wsys, RUSK_COMM_PORT, &win, sizeof(api_window));

	while (!receive(wsys, RUSK_COMM_PORT, &handle, sizeof(handle)));
/*
	int x, y;
	static unsigned char buf[100][100][3];
//	ulong n = 0, pid;

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
*/
	return 0;
}
