#include "../include/vfs.h"
#include "../include/stdio.h"
#include "../include/string.h"

void init_vfs(VFS *vfs) { vfs->file_count = 0; }

void create_file(VFS *vfs, const char *filename, const char *data) {

  File *file = &vfs->file[vfs->file_count];
  int name_len = strlen(filename);
  int data_len = strlen(data);
  memcpy(file->name, filename, name_len + 1);
  memcpy(file->data, data, data_len + 1);
  file->size = data_len;
  vfs->file_count++;
}

const char *read_file(VFS *vfs, const char *filename) {
  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, filename) == 0) {

      return vfs->file[i].data;
    }
  }

  return "notp";
}

void delete_file(VFS *vfs, const char *filename) {

  File replace;

  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, filename) == 0) {
      vfs->file[i] = replace;
      vfs->file_count--;
    }
  }
}

void display_files(VFS *vfs) {

  for (int i = 0; i < vfs->file_count; i++) {
    printf("%s", vfs->file[i].name);
    printf(" ");
  }
}
