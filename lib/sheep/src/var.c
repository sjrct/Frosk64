//
// var.c
//
// written by sjrct
//

#include <sheep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REALLOC_SIZE 0x10

static sheep_var * env = NULL;

void sheep_set_var(const sheep_var * v)
{
	if (env == NULL) env = sheep_new_sck("");
	sheep_set_elm(env, v);
}

const sheep_var * sheep_get_var(const char * n)
{
	if (env == NULL) {
		env = sheep_new_sck("");
		return NULL;
	}
	return sheep_get_elm(env, n);
}

static sheep_var * new_var(const char * nm)
{
	sheep_var * v = malloc(sizeof(sheep_var));
	v->name = malloc(strlen(nm) + 1);
	strcpy(v->name, nm);
	return v;
}

sheep_var * sheep_new_str(const char * nm, const char * vl)
{
	sheep_var * v = new_var(nm);

	v->type = SHEEP_STR;
	v->u.str = malloc(strlen(vl) + 1);
	strcpy(v->u.str, vl);

	return v;
}

sheep_var * sheep_new_sck(const char * nm)
{
	sheep_var * v = new_var(nm);

	v->type = SHEEP_SCK;
	v->u.sck = malloc(sizeof(sheep_sck));
	v->u.sck->size = 0;
	v->u.sck->vars = NULL;

	return v;
}

sheep_var * sheep_new_arr(const char * nm, int cnt, const char ** arr)
{
	int i, c2;
	char * s;
	sheep_var * v;
	sheep_var * r = sheep_new_sck(nm);
	
	c2 = cnt;
	i = 0;
	while (c2 != 0) {
		c2 /= 10;
		i++;
	}
	s = malloc(i + 1); 
	
	for (i = 0; i < cnt; i++) {
		sprintf(s, "%d", i);
		v = sheep_new_str(s, arr[i]);
		sheep_set_elm(r, v);
		sheep_free_var(v);
	}
	
	return r;
}

sheep_var * sheep_copy_var(const sheep_var * o)
{
	size_t i;
	sheep_var * v;
	
	if (o == NULL) return NULL;
	
	v = new_var(o->name);
	v->type = o->type;
	
	switch (v->type) {
	case SHEEP_STR:
		v->u.str = malloc(strlen(o->u.str) + 1);
		strcpy(v->u.str, o->u.str);
		break;
	
	case SHEEP_SCK:
		v->u.sck = malloc(sizeof(sheep_sck));
		v->u.sck->size = o->u.sck->size;
		v->u.sck->vars = malloc(sizeof(sheep_var*) * v->u.sck->size);

		for (i = 0; i < v->u.sck->size; i++) {
			v->u.sck->vars[i] = sheep_copy_var(o->u.sck->vars[i]);
		}
		break;
	}	
	
	return v;
}

void sheep_free_var(sheep_var * v)
{
	size_t i;
	
	if (v == NULL) return;

	switch (v->type) {
	case SHEEP_STR:
		free(v->u.str);
		break;
	
	case SHEEP_SCK:
		for (i = 0; i < v->u.sck->size; i++) {
			sheep_free_var(v->u.sck->vars[i]);
		}
		
		free(v->u.sck->vars);
		free(v->u.sck);
		break;
	}
	
	free(v);
}

void sheep_set_elm(sheep_var * sck, const sheep_var * v)
{
	size_t i;

	if (!SHEEP_IS_SCK(sck)) return;

	for (i = 0; i < sck->u.sck->size; i++) {
		if (!strcmp(sck->u.sck->vars[i]->name, v->name)) {
			sheep_free_var(sck->u.sck->vars[i]);
			sck->u.sck->vars[i] = sheep_copy_var(v);
			return;
		}
	}
	
	if (sck->u.sck->size % REALLOC_SIZE == 0) {
		sck->u.sck->vars = realloc(sck->u.sck->vars, sizeof(sheep_var) * (sck->u.sck->size + REALLOC_SIZE));
	}
	
	sck->u.sck->vars[sck->u.sck->size++] = sheep_copy_var(v);
}

const sheep_var * sheep_get_elm(sheep_var * sck, const char * nm)
{
	size_t i;

	if (SHEEP_IS_SCK(sck)) {
		for (i = 0; i < sck->u.sck->size; i++) {
			if (!strcmp(sck->u.sck->vars[i]->name, nm)) {
				return sck->u.sck->vars[i];
			}
		}
	}
	
	return NULL;
}
