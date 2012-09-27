//
// parse.c
//
// written by sjrct
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"
#include "error.h"

#define BUFSIZE 1024

static FILE * in;

void ignore_whitespace(void)
{
	int c;
	while (isspace(c = getc(in))) {
		if (c == '\n') current_line++;
	}
	ungetc(c, in);
}

void expect(int want)
{
	int got;

	ignore_whitespace();
	got = getc(in);

	if (got != want) {
		error("Expected '%c' but got '%c'", want, got);
	}
}

int next_token(char * buffer)
{
	int c, i = 0, j = 0;

	ignore_whitespace();

	while (i < BUFSIZE - 1) {
		c = getc(in);
		if (c == '\n') current_line++;
		if (c == ']' || c == ',' || c == '=') break;
		buffer[i++] = c;
		if (!isspace(c)) j = i;
	}

	buffer[j] = '\0';
	
	if (i == BUFSIZE - 1) {
		while (c != ']' && c != ',' && c != '=') {
			c = getc(in);
			if (c == '\n') current_line++;
		}
	}

	return c;
}

parse_struct * parse_next(FILE * f, perm defr, perm defw, perm defe)
{
	int type_char, d;
	char buffer[BUFSIZE];
	parse_struct * ps;
	parse_struct * sub_ps;

	in = f;
	if (current_line == -1) current_line = 1;

	ignore_whitespace();
	type_char = getc(in);
	if (type_char == EOF) return NULL;

	expect(':');
	expect('[');

	ps = malloc(sizeof(parse_struct));
	ps->next = NULL;
	ps->write_perm = defw;
	ps->read_perm = defr;
	ps->exec_perm = defe;
	ps->owner = 0;

	switch (type_char)
	{
	case 'f':
		ps->type = PST_FILE;
		break;
	case 'd':
		ps->type = PST_DIR;
		break;
	case 'i':
		ps->type = PST_INDIRECT;
		break;
	default:
		error("Unknown node type '%c'", type_char);
	}

	do {
		d = next_token(buffer);

		if (d != '=') error("Expected '=' after '%s'", buffer);

		if (!strcmp(buffer, "name")) {
			d = next_token(buffer);
			ps->name = malloc(strlen(buffer) + 1);
			strcpy(ps->name, buffer);
		}
		else if (!strcmp(buffer, "exec")) {
			d = next_token(buffer);
			
			if (!strcmp(buffer, "none")) ps->exec_perm = PERM_ALL;
			else if (!strcmp(buffer, "owner")) ps->exec_perm = PERM_OWNER;
			else if (!strcmp(buffer, "all")) ps->exec_perm = PERM_ALL;
		}
		else if (!strcmp(buffer, "read")) {
			d = next_token(buffer);
			
			if (!strcmp(buffer, "none")) ps->read_perm = PERM_ALL;
			else if (!strcmp(buffer, "owner")) ps->read_perm = PERM_OWNER;
			else if (!strcmp(buffer, "all")) ps->read_perm = PERM_ALL;
		}
		else if (!strcmp(buffer, "write")) {
			d = next_token(buffer);
			
			if (!strcmp(buffer, "none")) ps->write_perm = PERM_ALL;
			else if (!strcmp(buffer, "owner")) ps->write_perm = PERM_OWNER;
			else if (!strcmp(buffer, "all")) ps->write_perm = PERM_ALL;
		}
		else if (!strcmp(buffer, "owner")) {
			d = next_token(buffer);
			ps->owner = atoi(buffer);
		}
		else if (!strcmp(buffer, "actual")) {
			if (type_char != 'f') {
				error("'actual' only valid for files");
			}

			d = next_token(buffer);
			ps->u.actual = malloc(strlen(buffer) + 1);
			strcpy(ps->u.actual, buffer);
		}
		else if (!strcmp(buffer, "dest")) {
			if (type_char != 'i') {
				error("'dest' only valid for indirects");
			}

			d = next_token(buffer);
			ps->u.dest = malloc(strlen(buffer) + 1);
			strcpy(ps->u.dest, buffer);
		}
		else if (!strcmp(buffer, "contents")) {
			if (type_char != 'd') {
				error("'contents' only valid for directories");
			}

			expect('{');

			ignore_whitespace();
			d = getc(in);
			if (d != '}') {
				ungetc(d, in);
				ps->u.contents = sub_ps = parse_next(in, ps->read_perm, ps->write_perm, ps->exec_perm);

				while (sub_ps != NULL) {
					ignore_whitespace();
					d = getc(in);
					if (d == '}') break;
					ungetc(d, in);

					sub_ps->next = parse_next(in, ps->read_perm, ps->write_perm, ps->exec_perm);
					sub_ps = sub_ps->next;
				}
			} else {
				ps->u.contents = NULL;
			}

			ignore_whitespace();
			d = getc(in);
		} else {
			error("Unrecognized field '%s'", buffer);
		}
	} while (d != ']');

	return ps;
}

void free_parse_struct(parse_struct * ps)
{
	if (ps == NULL) return;
	if (ps->type == PST_DIR) free_parse_struct(ps->u.contents);
	if (ps->next != NULL) free_parse_struct(ps->next);
	free(ps);
}
