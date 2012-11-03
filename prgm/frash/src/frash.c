//
// frash.c
//
// written by sjrct
//

//#include <stdio.h>
//#include <sheep.h>
#include <fapi.h>

#define BUF_SZ 512

//static char * prompt = "> ";

int main(int argc, char ** argv)
{
	driver_call(3, 3, "Hello");

/*	int i, interactive = 1;
	char buf[BUF_SZ];
	
	buf[BUF_SZ - 1] = '\0';

	for (i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-")) {
			if (++i < argc) {
				if (!strcmp(argv[i], "i")) {
					interactive = 2;
				}
			}
		} else {
			sheep_exec_file(argv[i], stderr);
			if (interactive != 2) interactive = 0;
		}
	}
	
	if (interactive) {
		while (1) {
			fputs(prompt, stdout);
			fgets(buf, BUF_SZ - 1, stdin);
			
			if (!strcmp(buf, "exit")) {
				break;
			}
			else if (!strncmp(buf, "cd", 2)) {
				if (isspace(buf[2])) {
					i = 2;
					while (isspace(buf[i])) i++;
					chg_cwd(buf + i);
				} else {
					// TODO
				}
			}
			else {
				sheep_exec_line(buf, stderr);
			}
		}
	}
*/
	return 0;
}
