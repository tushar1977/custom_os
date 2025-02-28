#ifndef __VFS_H__
#define __VFS_H__
#include <stdio.h>
#include <sys/types.h>

#define INITIAL_CAPACITY 200
#define MAX_FILENAME 256
#define MAX_FILES 100
#define MAX_DATABLOCKS 50

enum vtype { VNON, VREG, VDIR, VBLK, VCHR };
typedef struct {
  char name[MAX_FILENAME]; // File name
  char *data;              // File content
  size_t size;             // File size
  int permissions;         // File permissions (e.g., 0644)
  enum vtype type;         // File type (regular, directory, etc.)
} File;

typedef struct {
  char name[MAX_FILENAME]; // Directory name
  File *files;             // List of files in the directory
  int file_count;          // Number of files
  int capacity;            // Current capacity of the directory
} Directory;

typedef struct {
  Directory *root;   // Root directory
  int total_files;   // Total files in the VFS
  size_t total_size; // Total size of all files
} VFS;

typedef struct {
  size_t total_size;  // Total size of the file system
  size_t block_size;  // Size of each block
  size_t inode_count; // Total number of inodes
  size_t free_blocks; // Number of free blocks
  size_t root_inode;  // Inode of the root directory
} Superblock;

typedef struct {
  size_t size;              // Size of the file
  time_t creation_time;     // Creation timestamp
  time_t modification_time; // Last modified timestamp
  int permissions;          // File permissions
  enum vtype type;          // File type (regular, directory, etc.)
  size_t blocks[50];        // Pointers to data blocks
} Inode;

extern VFS *vfs;

void init_vfs();
int create_file(const char *filename, const char *data);
int delete_file(const char *filename);
const char *read_file(const char *filename);

void insert_data(char *name, char *data);
void display_file_content(char *name);
void display_files();
#endif // !__VFS_H__
