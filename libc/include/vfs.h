#ifndef __VFS_H__
#define __VFS_H__
#include <stdint.h>
#include <stdio.h>
#define MAX_FILENAME 256
#define MAX_PATH 4096
#define MAX_FILES 100
typedef struct {
  char name[MAX_FILENAME];
  char data[MAX_PATH];
  unsigned int size;
} File;

typedef struct {
  File file[MAX_FILES];
  int file_count;
} VFS;

void init_vfs(VFS *vfs);
void create_file(VFS *vfs, const char *filename, const char *data);

void delete_file(VFS *vfs, const char *filename);
const char *read_file(VFS *vfs, const char *filename);

void display_files(VFS *vfs);
#endif // !__VFS_H__
