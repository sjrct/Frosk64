//
// fs.c
//
// written by sjrct
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "fs.h"
#include "error.h"

#define NT_FREE     0
#define NT_DIR      1
#define NT_FILE     2
#define NT_DATA     3
#define NT_STRING   4
#define NT_INDIRECT 5

#define PERM_EXEC_NONE   0x00
#define PERM_EXEC_OWNER  0x01
#define PERM_EXEC_ALL    0x02
#define PERM_READ_NONE   0x00
#define PERM_READ_OWNER  0x04
#define PERM_READ_ALL    0x08
#define PERM_WRITE_NONE  0x00
#define PERM_WRITE_OWNER 0x10
#define PERM_WRITE_ALL   0x20

#define STR_CHARS_PER_NODE 18

typedef struct  node {
	unsigned char type;
	unsigned int node_ptr;

	union {
		struct {
			unsigned char perm;
			int owner;
			struct node * name;
			struct node * first_data;
			struct node * next;
			struct node * parent;
			char * actual;
		} file;

		struct {
			unsigned char perm;
			int owner;
			struct node * name;
			struct node * first_file;
			struct node * next;
			struct node * parent;
		} dir;

		struct {
			unsigned block_count;
			unsigned long long lba;
			struct node * next;
			unsigned bytes_in_last_block;
		} data;

		struct {
			char size;
			char str[STR_CHARS_PER_NODE];
			struct node * next;
		} string;

		struct {
			unsigned char perm;
			int owner;
			struct node * name;
			struct node * dest;
			struct node * next;
			struct node * parent;
		} indirect;
	} u;
} node;

typedef struct meta_block_list {
	unsigned long long lba;
	struct meta_block_list * next;
} meta_block_list;

struct {
	node * root;
	node * predir;
	node * unused;
	unsigned long long cur_tbl_lba;
	unsigned cur_high_addr;
	int cur_tbl_entry;
	meta_block_list * mbl;
} file_sys = {
	NULL,
	NULL,
	NULL,
	0,
	-0x100,
	0x100,
	NULL
};


static unsigned long long alloc_blocks(unsigned count)
{
	static unsigned long long cur = 1;
	unsigned long long n = cur;
	cur += (unsigned long long)count;
	return n;
}

static void longseek(FILE * f, unsigned long long loc)
{
	fseek(f, 0, SEEK_SET);
	while (loc > INT_MAX) {
		fseek(f, INT_MAX, SEEK_CUR);
		loc -= INT_MAX;
	}
	fseek(f, loc, SEEK_CUR);
}

static node * new_node()
{
	node * n = malloc(sizeof(node));

	if (file_sys.cur_tbl_entry >= 0x100) {
		file_sys.cur_tbl_entry = 1;
		file_sys.cur_tbl_lba = alloc_blocks(2);
		file_sys.cur_high_addr += 0x100;

		if (file_sys.mbl == NULL) {
			file_sys.mbl = malloc(sizeof(meta_block_list));
		} else {
			file_sys.mbl->next = malloc(sizeof(meta_block_list));
			file_sys.mbl = file_sys.mbl->next;
		}

		file_sys.mbl->lba = file_sys.cur_tbl_lba;
	}

	n->node_ptr = file_sys.cur_high_addr | file_sys.cur_tbl_entry;
	file_sys.cur_tbl_entry++;

	return n;
}

static node * alloc_blocks_for_file(const char * fn)
{
	int bytes;
	FILE * f;
	node * n = new_node();

	f = fopen(fn, "r");
	if (f == NULL) return NULL;

	fseek(f, 0, SEEK_END);
	bytes = ftell(f);
	fclose(f);

	if (bytes % 0x1000 == 0) {
		n->u.data.block_count = bytes / 0x1000;
		n->u.data.bytes_in_last_block = 0x1000;
	} else {
		n->u.data.block_count = bytes / 0x1000 + 1;
		n->u.data.bytes_in_last_block = bytes % 0x1000;
	}

	if (n->u.data.block_count > 0xffffff) {
		// FIXME
		error("Maximium block size exceeded by file '%s'\n", fn);
	}

	n->type = NT_DATA;
	n->u.data.next = NULL;
	n->u.data.lba = alloc_blocks(n->u.data.block_count);

	return n;
}

static node * make_string(char * str)
{
	node * n = new_node();
	int l = strlen(str);

	n->type = NT_STRING;

	if (l > STR_CHARS_PER_NODE) {
		n->u.string.size = STR_CHARS_PER_NODE;
		memcpy(n->u.string.str, str, STR_CHARS_PER_NODE);
		n->u.string.next = make_string(str + STR_CHARS_PER_NODE);
	} else {
		n->u.string.size = l;
		memcpy(n->u.string.str, str, l);
		n->u.string.next = NULL;
	}

	return n;
}

static node * add_fs_entry_h(parse_struct * ps, node * parent)
{
	node * n = new_node();

	switch (ps->type) {
	case PST_FILE:
		n->type = NT_FILE;
		n->u.file.owner = ps->owner;
		n->u.file.name = make_string(ps->name);

		if (ps->next != NULL) {
			n->u.file.next = add_fs_entry_h(ps->next, parent);
		} else n->u.file.next = NULL;

		n->u.file.actual = malloc(strlen(ps->u.actual));
		strcpy(n->u.file.actual, ps->u.actual);
		n->u.file.first_data = alloc_blocks_for_file(ps->u.actual);

		n->u.file.parent = parent;
		break;

	case PST_DIR:
		n->type = NT_DIR;
		n->u.dir.name = make_string(ps->name);
		n->u.dir.owner = ps->owner;

		if (ps->type == PST_DIR && ps->u.contents != NULL) {
			n->u.dir.first_file = add_fs_entry_h(ps->u.contents, n);
		} else {
			n->u.dir.first_file = NULL;
		}

		if (ps->next != NULL) {
			n->u.dir.next = add_fs_entry_h(ps->next, parent);
		} else n->u.dir.next = NULL;

		if (parent == NULL) n->u.dir.parent = n;
		else n->u.dir.parent = parent;

		break;

	case PST_INDIRECT:
		n->type = NT_INDIRECT;
		n->u.indirect.name = make_string(ps->name);
		n->u.indirect.dest = make_string(ps->u.dest);
		n->u.indirect.owner = ps->owner;
		if (ps->next != NULL) {
			n->u.indirect.next = add_fs_entry_h(ps->next, parent);
		} else n->u.indirect.next = NULL;
		n->u.indirect.parent = parent;
		break;
	}

	return n;
}

void add_fs_entry(parse_struct * ps)
{
	if (file_sys.unused == NULL) {
		file_sys.unused = new_node();
		file_sys.unused->type = NT_DATA;
		file_sys.unused->u.data.block_count = 0;
		file_sys.unused->u.data.bytes_in_last_block = 0;
		file_sys.unused->u.data.next = NULL;
	}

	if (ps->type != PST_DIR) {
		error("Stray node with name '%s'", ps->name);
	}

	if (!strcmp(ps->name, "/")) {
		file_sys.root = add_fs_entry_h(ps, NULL);
	}
	else if (!strcmp(ps->name, "@")) {
		file_sys.predir = add_fs_entry_h(ps, NULL);
	}
	else {
		error("Stray node with name '%s'", ps->name);
	}
}

static void seek_to_node(FILE * f, unsigned ptr)
{
	unsigned long long to;
	int i = 0;
	meta_block_list * mbl = file_sys.mbl;

	while (i != (ptr & ~0xff)) {
		i += 0x100;
		mbl = mbl->next;
	}

	to = mbl->lba * 0x1000;
	to += (ptr & 0xff) * 24;

	longseek(f, to);
}

static void write_file_data(node * n, FILE * f)
{
	FILE * in;
	node * data;
	char * buffer;

	in = fopen(n->u.file.actual, "r");
	if (in == NULL) return;
	data = n->u.file.first_data;

	while (data != NULL) {
		buffer = malloc(data->u.data.block_count * 0x1000);
		fread(buffer, 1, data->u.data.block_count * 0x1000, in);
		longseek(f, data->u.data.lba * 0x1000);
		fwrite(buffer, 1, data->u.data.block_count * 0x1000, f);
		free(buffer);
		data = data->u.data.next;
	}

	fclose(in);
}

static void write_fs_h(node * n, FILE * f, int add)
{
	unsigned zero = 0;
	int i = 0;

	seek_to_node(f, n->node_ptr);
	
	fseek(f, add, SEEK_CUR);

	fputc(n->type, f);

	switch (n->type)
	{
	case NT_FILE:
		fputc(n->u.file.perm, f);
		fwrite((char*)(&zero), 1, 2, f);
		fwrite((char*)(&n->u.file.owner), sizeof(int), 1, f);

		if (n->u.file.name == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.file.name->node_ptr), sizeof(int), 1, f);

		if (n->u.file.first_data == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.file.first_data->node_ptr), sizeof(int), 1, f);

		if (n->u.file.next == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.file.next->node_ptr), sizeof(int), 1, f);

		if (n->u.file.parent == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.file.parent->node_ptr), sizeof(int), 1, f);

		if (n->u.file.name != NULL) write_fs_h(n->u.file.name, f, add);
		if (n->u.file.first_data != NULL) write_fs_h(n->u.file.first_data, f, add);
		if (n->u.file.next != NULL) write_fs_h(n->u.file.next, f, add);

		write_file_data(n, f);

		break;
		
	case NT_DIR:
		fputc(n->u.dir.perm, f);
		fwrite((char*)(&zero), 1, 2, f);
		fwrite((char*)(&n->u.dir.owner), sizeof(int), 1, f);

		if (n->u.dir.name == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.dir.name->node_ptr), sizeof(int), 1, f);

		if (n->u.dir.first_file == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.dir.first_file->node_ptr), sizeof(int), 1, f);

		if (n->u.dir.next == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.dir.next->node_ptr), sizeof(int), 1, f);

		if (n->u.dir.parent == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.dir.parent->node_ptr), sizeof(int), 1, f);

		if (n->u.dir.name != NULL) write_fs_h(n->u.dir.name, f, add);
		if (n->u.dir.first_file != NULL) write_fs_h(n->u.dir.first_file, f, add);
		if (n->u.dir.next != NULL) write_fs_h(n->u.dir.next, f, add);
		break;

	case NT_INDIRECT:
		fputc(n->u.indirect.perm, f);
		fwrite((char*)(&zero), 1, 2, f);
		fwrite((char*)(&n->u.indirect.owner), sizeof(int), 1, f);

		if (n->u.indirect.name == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.indirect.name->node_ptr), sizeof(int), 1, f);

		if (n->u.indirect.dest == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.indirect.dest->node_ptr), sizeof(int), 1, f);

		if (n->u.indirect.next == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.indirect.next->node_ptr), sizeof(int), 1, f);

		if (n->u.indirect.parent == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.indirect.parent->node_ptr), sizeof(int), 1, f);

		if (n->u.indirect.name != NULL) write_fs_h(n->u.indirect.name, f, add);
		if (n->u.indirect.dest != NULL) write_fs_h(n->u.indirect.dest, f, add);
		if (n->u.indirect.next != NULL) write_fs_h(n->u.indirect.next, f, add);

		break;
		
	case NT_DATA:
		fwrite((char*)(&zero), 1, 3, f);
		fwrite((char*)(&n->u.data.block_count), sizeof(int), 1, f);
		fwrite((char*)(&n->u.data.lba), sizeof(long long), 1, f);
		
		if (n->u.data.next == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.data.next), sizeof(int), 1, f);
		
		fwrite((char*)(&n->u.data.bytes_in_last_block), sizeof(int), 1, f);

		if (n->u.data.next != NULL) write_fs_h(n->u.data.next, f, add);
		
		break;
	
	case NT_STRING:
		fputc(n->u.string.size, f);
		for (i = 0; i < STR_CHARS_PER_NODE; i++) fputc(n->u.string.str[i], f);
				
		if (n->u.string.next == NULL) fwrite((char*)(&zero), sizeof(int), 1, f);
		else fwrite((char*)(&n->u.string.next), sizeof(int), 1, f);

		if (n->u.data.next != NULL) write_fs_h(n->u.data.next, f, add);
		
		break;
		
	case NT_FREE:
		fseek(f, 23, SEEK_CUR);
		break;
	}

	if (add == 0) {
		write_fs_h(n, f, 0x1000);
	}
}

void write_fs(FILE * f)
{
	int i;
	unsigned mark = 0xf301;
	unsigned long long rlba;
	meta_block_list * mbl = file_sys.mbl;

	while (mbl != NULL) {
		for (i = 0; i != 1; i++) {
			longseek(f, mbl->lba * 0x1000 + i * 0x1000);
			fwrite((char*)(&mark), sizeof(int), 1, f);
			fwrite((char*)(&file_sys.root->node_ptr), sizeof(int), 1, f);
			fwrite((char*)(&file_sys.predir->node_ptr), sizeof(int), 1, f);
			fwrite((char*)(&file_sys.unused->node_ptr), sizeof(int), 1, f);
			if (i == 0) rlba = mbl->lba + 1;
			else rlba = mbl->lba - 1;
			fwrite((char*)(&rlba), sizeof(long long), 1, f);
			mbl = mbl->next;
		}
	}

	write_fs_h(file_sys.predir, f, 0);
	write_fs_h(file_sys.root, f, 0);
	write_fs_h(file_sys.unused, f, 0);

	fseek(f, 0, SEEK_END);
	rlba = ftell(f) % 0x1000;
	if (rlba != 0) {
		rlba = 0x1000 - rlba;
		for (; rlba > 0; rlba--) fputc(0, f);
	}
}
