//
// start.c
//
// written by sjrct
//

#include <stdlib.h>
#include <ctype.h>
#include <frosk.h>

static void run_prgm(const char *);
ulong read_file(const char *, uchar **);

int main()
{
	uchar * buf, * start;

	if (!read_file("/sys/prgm/start/run_prgms.lst", &buf)) {
		driver_call(3, 3, "run_prgms.lst not found.");
		while (1);
	}

	while (isspace(*buf)) buf++;
	start = buf;
	
	while (*buf != '\0') {
		if (*buf == '#') {
			while (*buf != '\n') buf++;
			while (isspace(*buf)) buf++;
			start = buf;
			continue;
		}
		
		if (isspace(*buf)) {
			*buf ='\0';
			run_prgm((char*)start);
			
			do {
				buf++;
			} while (isspace(*buf));
			
			while (isspace(*buf)) buf++;
			start = buf;
		}
		
		buf++;
	}
	
	while(!get_wsys());
	send(get_wsys(), 0, &buf, 4);
	while(1);

	return 0;
}

ulong read_file(const char * fn, uchar ** buf)
{
	ulong bl, sz;
	handle_t h;

	h = fs_aquire(fn, 0, 1);
	if (h == 0) return 0;
	
	sz = fs_get_size(h);
	bl = sz / 0x1000;
	if (sz % 0x1000) bl++;
	
	*buf = malloc(bl * 0x1000 + 1);
	fs_read(h, *buf, bl, 0);

	fs_release(h);
	
	return sz;
}

void run_prgm(const char * fn)
{
	ulong sz;
	uchar * buf;

	sz = read_file(fn, &buf);
	if (!sz) return;

	// TODO adjust parent and priority
	exec_fbe(buf, sz, NULL, 0, 0x80);
}
