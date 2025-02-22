#include "../include/vfs.h"
#include "../include/memory.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "stdbool.h"
#include "stdio.h"

VFS *vfs = NULL;
void init_vfs() {
  vfs = (VFS *)malloc(sizeof(VFS));
  if (!vfs) {
    printf("Error: Failed to allocate memory for VFS\n");
    return;
  }
  vfs->file_count = 0;
}
File *check_file(char *name) {

  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, name) == 0) {
      return &vfs->file[i];
    }
  }

  return NULL;
}

void create_file(const char *filename, const char *data) {
  if (vfs->file_count >= MAX_FILES) {
    printf("VFS is full. Cannot create more files.\n");
    return;
  }

  int name_len = strlen(filename);
  int data_len = strlen(data);

  File *file = &vfs->file[vfs->file_count];

  file->data = (char *)malloc(data_len + 1);

  memcpy(file->name, filename, name_len + 1);
  file->name[MAX_FILENAME - 1] = '\0';

  memcpy(file->data, data, data_len + 1);

  file->size = data_len;
  vfs->file_count++;
}

const char *read_file(const char *filename) {
  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, filename) == 0) {

      return vfs->file[i].data;
    }
  }

  return "notp";
}

void delete_file(const char *filename) {

  File replace;

  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, filename) == 0) {
      vfs->file[i] = replace;
      vfs->file_count--;
    }
  }
}

void display_files() {

  for (int i = 0; i < vfs->file_count; i++) {
    printf("%s", vfs->file[i].name);
    printf(" ");
  }
}
void insert_data(char *name, char *data) {

  File *f = check_file(name);
  if (f == NULL) {
    printf("File not Found \n");
    return;
  }

  if (f->data != NULL) {
    free(f->data);
  }
  f->data = (char *)malloc(strlen(data) + 1);

  memcpy(f->data, data, strlen(data));
}

void display_file_content(char *name) {

  File *f = check_file(name);
  if (f != NULL) {
    printf("%s\n", f->data);
  }
}
