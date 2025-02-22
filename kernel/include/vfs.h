#ifndef __VFS_H__
#define __VFS_H__
#include <stdio.h>
#define MAX_FILENAME 256
#define MAX_FILES 100
typedef struct {
  char name[MAX_FILENAME];
  char *data;
  unsigned int size;
} File;

typedef struct {
  File file[MAX_FILES];
  int file_count;
} VFS;

extern VFS *vfs;
void init_vfs();
void create_file(const char *filename, const char *data);
const char *read_file(const char *filename);

void delete_file(const char *filename);

void insert_data(char *name, char *data);
void display_file_content(char *name);
void display_files();
#endif // !__VFS_H__
