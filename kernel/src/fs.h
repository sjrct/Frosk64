//
// fs.h
//
// written by sjrct
//

#ifndef FS_H
#define FS_H

#include <cmn/utypes.h>

enum {
	FS_FREE = 0,
	FS_DIRECTORY,
	FS_FILE,
	FS_DATA,
	FS_STRING,
	FS_INDIRECT,
};

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
#define SECTS_PER_BLOCK 8

#pragma pack(push, 1)
typedef struct fs_node {
	uchar type;
	
	union {
		struct {
			uchar perm;
			char pack[2];
			uint owner;
			uint name_ptr;
			uint first_data_ptr;
			uint next_ptr;
			uint parent_ptr;
		} file;
		
		struct {
			uchar perm;
			char pack[2];
			uint owner;
			uint name_ptr;
			uint first_file_ptr;
			uint next_ptr;
			uint parent_ptr;
		} dir;
		
		struct {
			char pack[3];
			uint blocks;
			ulong lba;
			uint next_ptr;
			uint bytes_in_last_block;
		} data;

		struct {
			uchar size;
			char str[STR_CHARS_PER_NODE];
			uint next_ptr;
		} string;
		
		struct {
			uchar perm;
			char pack[2];
			uint owner;
			uint name_ptr;
			uint dest_ptr;
			uint next_ptr;
			uint parent_ptr;
		} indirect;
	} u;
} fs_node;
#pragma pack(pop)

typedef fs_node * handle_t;

void init_fs(void);

handle_t fs_aquire(const char *, handle_t, int);
int fs_release(handle_t);

ulong fs_read(handle_t, char *, ulong, ulong);
ulong fs_write(handle_t, const char *, ulong, ulong);

ulong fs_get_size(handle_t);
int fs_get_filename(handle_t, char *, ulong);
ulong fs_get_perm(handle_t);
int fs_set_perm(handle_t, ulong);

handle_t fs_create_file(const char *, handle_t, handle_t);
handle_t fs_create_dir(const char *, handle_t, handle_t);
handle_t fs_create_indirect(const char *, const char *, handle_t, handle_t);

int fs_remove(handle_t);

#endif
