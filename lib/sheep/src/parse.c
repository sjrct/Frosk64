//
// parse.c
//
// written by sjrct
//

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

#define REALLOC_SIZE 0x10

static void ignore_spaces(const char ** pbuf, err_st * es)
{
	while (isspace(**pbuf)) {
		if (**pbuf == '\n') es->ln++;
		(*pbuf)++;
	}
}

static int isspec(char c, const char * s)
{
	while (*s != '\0') {
		if (*s == c) return 1;
		s++;
	}
	return 0;
}

inline static int isch1(char c)
{
	return isalpha(c) || isdigit(c) || isspec(c, "$@._");
}

inline static int isch2(char c)
{
	return isspec(c, "=-+*/<>%^&|!?:");
}

args next_statement(const char ** pbuf, err_st * es)
{
	args ar;
	int i;
	const char * start, * end;
	const char * buf;

	ignore_spaces(pbuf, es);
	
	buf = *pbuf;
	ar.argc = 0;
	ar.argv = NULL;
	
	while (*buf != ';' && *buf != '\0') {
		if (ar.argc % REALLOC_SIZE == 0) {
			ar.argv = realloc(ar.argv, (ar.argc + REALLOC_SIZE) * sizeof(char*));
		}
	
		if (isch1(*buf)) {
			start = buf;
			while (isch1(*buf)) buf++;
			end = buf;
		}
		else if (isch2(*buf)) {
			start = buf;
			while (isch2(*buf)) buf++;
			end = buf;
		}
		else if (*buf == '\'') {
			buf++;
			start = buf;
			while (*buf != '\'' && *buf != '\0') buf++;
			end = buf;
			if (*buf != '\0') buf++;
		}
		else if (*buf == '"') {
			buf++;
			start = buf;
			while ((*buf != '"' || buf[-1] == '\\') && *buf != '\0') buf++;
			end = buf;
			if (*buf != '\0') buf++;
			
			i = 0;
			ar.argv[ar.argc] = malloc(end - start + 1);
			while ((*start != '"' || start[-1] == '\\') && *start != '\0') {
				if (*start == '\\') {
					start++;
					switch (*start) {
					case 'n':
						ar.argv[ar.argc][i] = '\n';
						break;
						
					case 't':
						ar.argv[ar.argc][i] = '\t';
						break;
					
					case 'r':
						ar.argv[ar.argc][i] = '\r';
						break;
					
					case '\0':
						start--;
						i--;
						break;
					
					default:
						ar.argv[ar.argc][i] = *start;
					}
				} else {
					ar.argv[ar.argc][i] = *start;
				}
				
				i++;
				start++;
			}
			ar.argc++;
			
			ignore_spaces(&buf, es);
			continue;
		}
		else {
			start = buf;
			if (*buf != '\0') buf++;
			end = buf;
		}
		
		ar.argv[ar.argc] = malloc(end - start + 1);
		memcpy(ar.argv[ar.argc], start, end - start);
		ar.argv[ar.argc][end - start] = '\0';
		ar.argc++;

		ignore_spaces(&buf, es);
	}
	
	if (*buf != '\0') *buf++;
	*pbuf = buf;
	
	return ar;
}

void remove_comments(char * buf)
{
	int i, d;

	while (*buf != '\0') {
		if (*buf == '#') {
			i = d = 0;
			while (buf[d] != '\n' && buf[d] != '\0') d++;

			do {
				buf[i] = buf[d + i];
				i++;
			} while (buf[d + i] != '\0');
		}
		buf++;
	}
}

void free_args(args ar)
{
	int i;
	for (i = 0; i < ar.argc; i++) {
		free(ar.argv[i]);
	}
	free(ar.argv);
}
