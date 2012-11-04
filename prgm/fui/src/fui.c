//
// fui.c
//
// written by sjrct
//

#include <frosk.h>
#include <expanse.h>
#include <stdlib.h>

void sendint(pid_t pid, int port, int n)
{
	send(pid, port, &n, sizeof(int));
}

int main()
{
	expanse_handle handle;
	api_expanse exp;
	pid_t esys;
	
	while (!(esys = get_esys()));

	sendint(esys, ES_COMM_PORT, ES_CREATE_EXPANSE);
	
	exp.width = exp.height = 100;
	exp.attributes = 0;
	send(esys, ES_COMM_PORT, &exp, sizeof(api_expanse));

	while (!receive(esys, ES_COMM_PORT, &handle, sizeof(handle)));
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
