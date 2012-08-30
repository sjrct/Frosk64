//
// main.c
//
// written by sjrct
//

#include <stdio.h>
#include "fs.h"
#include "parse.h"

int main(int argc, char ** argv)
{
	parse_struct * ps;
	FILE * f;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s [in file] [out file]\n", argv[0]);
		return -1;
	}

	f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "Error: Cannot open '%s' for reading.\n", argv[1]);
		return -1;
	}


	while (1) {
		ps = parse_next(f, PERM_NONE, PERM_NONE, PERM_NONE);
		if (ps == NULL) break;
		add_fs_entry(ps);
		free_parse_struct(ps);
	}

	fclose(f);

	f = fopen(argv[2], "w");
	if (f == NULL) {
		fprintf(stderr, "Error: Cannot open '%s' for writing.\n", argv[2]);
		return -1;
	}

	write_fs(f);

	fclose(f);

	return 0;
}
