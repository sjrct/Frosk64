//
// sheep.c
//
// written by sjrct
//

#include <sheep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "error.h"

static char * cat_args(int argc, char ** argv)
{
	int i, l;
	char * s;
	
	for (i = l = 0; i < argc; i++) {
		l += strlen(argv[i]) + 1;
	}
	s = malloc(l);
	
	for (l = i = 0; i < argc; i++) {
		strcpy(s + l, argv[i]);
		l += strlen(argv[i]);
		s[l++] = ' ';
	}
	s[l - 1] = '\0';
	
	return s;
}

static void add_sack(args * ar, const sheep_var * v, int s)
{
	int i;

	s += v->u.sck->size;
	ar->argv = realloc(ar->argv, s * sizeof(char*));

	for (i = 0; i < v->u.sck->size; i++) {
		if (SHEEP_IS_STR(v->u.sck->vars[i])) {
			ar->argv[ar->argc] = malloc(strlen(v->u.sck->vars[i]->u.str) + 1);
			strcpy(ar->argv[ar->argc], v->u.sck->vars[i]->u.str);
			ar->argc++;
		} else {
			add_sack(ar, v->u.sck->vars[i], s);
		}
	}
}

static int address_sack(args ar, const sheep_var ** pv, int i, err_st * es)
{
	int j, k;
	args s;
	const sheep_var * v = *pv;

	while (i + 2 < ar.argc && v != NULL && !strcmp(ar.argv[i + 1], "[") && SHEEP_IS_SCK(v)) {
		i += 2;
		for (j = 0; i + j < ar.argc && strcmp(ar.argv[i + j], "]"); j++);
	
		s.argc = j;
		s.argv = malloc(sizeof(char*) * j);
		for (k = 0; k < j; k++) {
			s.argv[k] = malloc(strlen(ar.argv[i + j]) + 1);
			strcpy(s.argv[k], ar.argv[i + j]);
		}
	
		evaluate(&s, 0, es);
		if (s.argc > 0) {
			v = sheep_get_elm(v, s.argv[0]);
		} else {
			repor_warn(es, "Empty brackets are ignored.\n");
		}
		
		free_args(s);
		i += j;
	}
	
	*pv = v;
	
	return i;
}

static char * address_sack2(args ar, const sheep_var ** pv, err_st * es)
{
	int i = 0, j, k;
	args s;
	char * r = NULL;
	const sheep_var * p, * v;
	
	v = p = *pv;

	while (i + 2 < ar.argc && v != NULL && !strcmp(ar.argv[i + 1], "[") && SHEEP_IS_SCK(v)) {
		i += 2;
		for (j = 0; i + j < ar.argc && strcmp(ar.argv[i + j], "]"); j++);
	
		s.argc = j;
		s.argv = malloc(sizeof(char*) * j);
		for (k = 0; k < j; k++) {
			s.argv[k] = malloc(strlen(ar.argv[i + j]) + 1);
			strcpy(s.argv[k], ar.argv[i + j]);
		}
	
		evaluate(&s, 0, es);
		if (s.argc > 0) {
			p = v;
			r = malloc(strlen(s.argv[0]) + 1);
			strcpy(r, s.argv[0]);
			v = sheep_get_elm(v, r);
		} else {
			repor_warn(es, "Empty brackets are ignored.\n");
		}
		
		free_args(s);
		i += j;
	}
	
	*pv = p;
	
	return ;
}

static void evaluate(args * ar, int ig, err_st * es)
{
	int i, j, k, is_var;
	const sheep_var * v;
	args s, n, o = *ar;

	n.argc = 0;
	n.argv = malloc(o.argc * sizeof(char*));
	
	for (i = 0; i < o.argc; i++) {
		switch (o.argv[i][0]) {
		case '$':
			is_var = 1;
			break;
		case '@':
			is_var = 2;
			break;
		default:
			is_var = 0;
		}
		
		if (!is_var || i < ig) {
			if (!strcmp(o.argv[i], "(") && i >= ig) {
				// TODO
			} else {
				n.argv[n.argc] = malloc(strlen(o.argv[i]) + 1);
				strcpy(n.argv[n.argc], o.argv[i]);
				n.argc++;
			}
		} else {
			v = sheep_get_var(o.argv[i] + 1);

			if (v != NULL) {
				if (SHEEP_IS_STR(v)) {
					if (is_var == 1) {
						n.argv[n.argc] = malloc(strlen(v->u.str) + 1);
						strcpy(n.argv[n.argc], v->u.str);
						n.argc++;
					} else {
						// TODO do what?
					}
				} else {
					if (is_var == 2) {
						i = address_sack(o, v, i, es);
/*						while (i + 2 < o.argc && !strcmp(o.argv[i + 1], "[") && SHEEP_IS_SCK(v)) {
							i += 2;
							for (j = 0; i + j < o.argc && strcmp(o.argv[i + j], "]"); j++);
							
							s.argc = j;
							s.argv = malloc(sizeof(char*) * j);
							for (k = 0; k < j; k++) {
								s.argv[k] = malloc(strlen(o.argv[i + j]) + 1);
								strcpy(s.argv[k], o.argv[i + j]);
							}
							
							evaluate(&s, 0, es);
							if (s.argc > 0) {
								v = sheep_get_elm(v, s.argv[0]);
							} else {
								repor_warn(es, "Empty brackets are ignored.\n");
							}
							i += j;
						}
*/						
						if (SHEEP_IS_SCK(v)) {
							add_sack(&n, v, o.argc);
						} else {
							// TODO do what?
						}
					} else {
						i = address_sack(o, v, i, es);
						
						if (SHEEP_IS_SCK(v)) {
							// TODO
						} else {
							n.argv[n.argc] = malloc(strlen(v->u.str) + 1);
							strcpy(n.argv[n.argc], v->u.str);
							n.argc++;
						}
					}
				}
			}
		}
	}
	
	*ar = n;
	free_args(o);
}

static int parse_eval(const char * buf, err_st * es)
{
	args ar;
	sheep_var * v, * v2;
	char * s, * en;
	int i, r = 1, ig;
	
	while (*buf != '\0') {
		ar = next_statement(&buf, es);
		
		if (ar.argc > 0) {
			if (ar.argc > 1 && !strcmp(ar.argv[1], "=")) {
				if (ar.argv[0][0] == '$') {
					evaluate(&ar, 2, es);
					s = cat_args(ar.argc - 2, ar.argv + 2);
					v = sheep_new_str(ar.argv[0] + 1, s);

					sheep_set_var(v);
					
					sheep_free_var(v);
					free(s);
				}
				else if (ar.argv[0][0] == '@') {
					evaluate(&ar, 2, es);
					v = sheep_new_arr(ar.argv[0] + 1, ar.argc - 2, (const char**)ar.argv + 2);
					sheep_set_var(v);
					sheep_free_var(v);
				}
				else {
					repor_err(es, "Cannot assign value to literal '%s'.\n", ar.argv[0]);
					r = 0;
				}
			} else {
				evaluate(&ar, 0, es);
				// TODO execute command
				for (i = 0; i < ar.argc; i++) {
					printf("argv[%d] = '%s'\n", i, ar.argv[i]);
				}
			}
		}
	
		free_args(ar);
	}
	
	return r;
}

int sheep_exec_file(const char * fn, FILE * errf)
{
	char * buf;
	FILE * f;
	err_st es;
	size_t sz;
	int r;
	
	es.f = errf;
	es.fn = fn;
	es.ln = 0;
	
	f = fopen(fn, "rb");
	if (f == NULL) {
		fprintf(errf, "Error: Cannot open file '%s'\n", fn);
		return 0;
	}
	
	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	buf = malloc(sz + 1);
	fread(buf, 1, sz, f);
	buf[sz] = '\0';
	
	remove_comments(buf);
	r = parse_eval(buf, &es);
	
	fclose(f);
	free(buf);

	return r;
}

int sheep_exec_line(const char * buf, FILE * errf)
{
	err_st es;
	int r;
	
	es.f = errf;
	es.fn = NULL;
	es.ln = 0;

	r = parse_eval(buf, &es);
	
	return r;
}
