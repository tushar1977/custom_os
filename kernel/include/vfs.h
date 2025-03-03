#ifndef __VFS_H__
#define __VFS_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 200
#define MAX_FILENAME 256
#define MAX_FILES 100
#define MYFS_MAGIC 0x13131313
#define MAX_DATABLOCKS 50
#define BLOCK_SIZE 4096
#define MAX_BLOCK_NUMBER 50

enum vtype { VNON, VREG, VDIR, VBLK, VCHR };

typedef struct {
  char name[MAX_FILENAME];
  uint32_t inode_number;
} File;

typedef struct {
  char name[MAX_FILENAME];
  File *files;
  int file_count;
  int capacity;
} Directory;

struct myfs_superblock {
  uint32_t magic;
  uint32_t block_size;
  uint32_t inode_count;
  uint32_t free_blocks;
  uint32_t root_inode;
};

struct myfs_inode {
  uint32_t mode;
  uint32_t size;
  uint32_t blocks[MAX_BLOCK_NUMBER];
};

typedef struct {
  char data[BLOCK_SIZE];
} DataBlock;

typedef struct {
  Directory *root;
  struct myfs_superblock superblock;
  struct myfs_inode inode_table[MAX_FILES];
  DataBlock data_blocks[MAX_DATABLOCKS];
  int total_files;
  size_t total_size;
} VFS;
// | Superblock | Inode Table | Data Blocks |

extern VFS *vfs;

void init_vfs();

int create_file(const char *name, const char *data, enum vtype VTYPE);
int delete_file(const char *filename);
const char *read_file(const char *filename);
int create_directory(const char *name);
void display_all_files();
#endif // !__VFS_H__
