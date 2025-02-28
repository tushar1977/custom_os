#include "../include/vfs.h"
#include "../include/memory.h"
#include "../include/string.h"
#include <stdint.h>
#include <stdio.h>

#define VFS_SUCCESS 0
#define VFS_ERROR -1
#define VFS_FILE_NOT_FOUND -2
#define VFS_FULL -3

VFS *vfs = NULL;
void init_vfs() {
  vfs->root = (Directory *)malloc(sizeof(Directory));
  vfs->root->files = (File *)malloc(INITIAL_CAPACITY * sizeof(File));
  vfs->root->file_count = 0;
  vfs->root->capacity = INITIAL_CAPACITY;
  vfs->total_files = 0;
  vfs->total_size = 0;
}

int create_file(const char *name, const char *data) {
  if (vfs->root->file_count >= vfs->root->capacity) {
    vfs->root->capacity *= 2;
    vfs->root->files =
        (File *)realloc(vfs->root->files, vfs->root->capacity * sizeof(File));
  }

  File *file = &vfs->root->files[vfs->root->file_count];
  memcpy(file->name, name, strlen(name));
  memmove(file->data, data, strlen(data));
  file->size = strlen(data);
  file->permissions = 0644;
  file->type = VREG;

  vfs->root->file_count++;
  vfs->total_files++;
  vfs->total_size += file->size;

  return 0;
}

int delete_file(const char *name) {
  for (int i = 0; i < vfs->root->file_count; i++) {
    if (strcmp(vfs->root->files[i].name, name) == 0) {
      free(vfs->root->files[i].data);
      for (int j = i; j < vfs->root->file_count - 1; j++) {
        vfs->root->files[j] = vfs->root->files[j + 1];
      }
      vfs->root->file_count--;
      vfs->total_files--;
      return 0;
    }
  }
  return -1;
}

const char *read_file(const char *name) {
  for (int i = 0; i < vfs->root->file_count; i++) {
    if (strcmp(vfs->root->files[i].name, name) == 0) {
      return vfs->root->files[i].data;
    }
  }
  return NULL;
}
