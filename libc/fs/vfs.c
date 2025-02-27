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
  if (!vfs) {
    printf("Error: VFS not initialized\n");
    return;
  }

  if (vfs->file_count >= MAX_FILES) {
    printf("VFS is full. Cannot create more files.\n");
    return;
  }

  int name_len = strlen(filename);
  int data_len = strlen(data);

  File *file = &vfs->file[vfs->file_count];

  file->data = (char *)malloc(data_len + 1);

  if (!file->data) {
    printf("Memory allocation failed for file: %s\n", filename);
    free(file->data);
    return;
  }

  memcpy(file->name, filename, name_len + 1);
  file->name[MAX_FILENAME - 1] = '\0';

  memcpy(file->data, data, data_len + 1);
  printf("%s\n", filename);
  printf("%s\n", file->name);
  file->size = data_len;
  vfs->file_count++;

  printf("File created %s\n", filename);
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
  if (!vfs) {
    printf("Error: VFS not initialized\n");
    return;
  }

  for (int i = 0; i < vfs->file_count; i++) {
    if (strcmp(vfs->file[i].name, filename) == 0) {
      free(vfs->file[i].name);
      free(vfs->file[i].data);

      for (int j = i; j < vfs->file_count - 1; j++) {
        vfs->file[j] = vfs->file[j + 1];
      }
      vfs->file_count--;
      return;
    }
  }

  printf("File %s not found\n", filename);
}

void display_files() {
  if (!vfs) {
    printf("Error: VFS not initialized\n");
    return;
  }

  printf("Files in VFS (%d/%d):\n", vfs->file_count, MAX_FILES);

  for (int i = 0; i < vfs->file_count; i++) {
    printf("%s\n", vfs->file[i].name);
  }

  printf("----------------------------------------\n");
}

void insert_data(char *name, char *data) {

  if (!vfs) {
    printf("Error: VFS not initialized\n");
    return;
  }

  File *f = check_file(name);
  if (f == NULL) {
    create_file(name, data);
    return;
  }

  free(f->data);
  f->data = (char *)malloc(strlen(data) + 1);

  if (!f->data) {
    printf("Memory allocation failed for data insertion.\n");
    return;
  }
  memcpy(f->data, data, strlen(data) + 1);
}

void display_file_content(char *name) {
  if (vfs == NULL) {
    printf("VFS not initialized\n");
    return;
  }

  File *f = check_file(name);
  if (f != NULL && f->data != NULL) {
    printf("%s\n", f->data);
  } else {
    printf("File not found: %s\n", name);
  }
}
