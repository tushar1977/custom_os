#ifndef __VFS_H__
#define __VFS_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define INITIAL_CAPACITY 200 // Initial capacity for files in the root directory
#define MAX_FILENAME 256     // Maximum length of a file name
#define MAX_FILES 100        // Maximum number of files (inodes) in the VFS
#define MYFS_MAGIC 0x13131313 // Magic number for the superblock
#define MAX_DATABLOCKS 50     // Maximum number of data blocks
#define BLOCK_SIZE 4096       // Size of each data block (4 KB)
#define MAX_BLOCK_NUMBER 50   // Maximum number of blocks per inode

// File types
enum vtype { VNON, VREG, VDIR, VBLK, VCHR };

// File structure
typedef struct {
  char name[MAX_FILENAME]; // File name
  uint32_t inode_number;   // Inode number associated with the file
} File;

// Directory structure
typedef struct {
  char name[MAX_FILENAME]; // Directory name
  File *files;             // List of files in the directory
  int file_count;          // Number of files in the directory
  int capacity;            // Current capacity of the files array
} Directory;

// Superblock structure
struct myfs_superblock {
  uint32_t magic;       // Magic number to identify the file system
  uint32_t block_size;  // Size of each block
  uint32_t inode_count; // Total number of inodes
  uint32_t free_blocks; // Number of free data blocks
  uint32_t root_inode;  // Inode number of the root directory
};

// Inode structure
struct myfs_inode {
  uint32_t mode;                     // File mode (permissions and type)
  uint32_t size;                     // Size of the file
  uint32_t blocks[MAX_BLOCK_NUMBER]; // List of block numbers used by the file
};

// Data block structure
typedef struct {
  char data[BLOCK_SIZE]; // Data stored in the block
} DataBlock;

// Virtual File System (VFS) structure
typedef struct {
  Directory *root;                           // Root directory
  struct myfs_superblock *superblock;        // Superblock
  struct myfs_inode *inode_table[MAX_FILES]; // Inode table
  DataBlock data_blocks[MAX_DATABLOCKS];     // Data blocks
  int total_files;                           // Total number of files in the VFS
  size_t total_size; // Total size of all files in the VFS
} VFS;

// Global VFS pointer
extern VFS *vfs;

// Function declarations

/**
 * Initializes the Virtual File System (VFS).
 * Allocates memory for the VFS, root directory, superblock, and inode table.
 */
void init_vfs();

/**
 * Cleans up and deallocates memory used by the VFS.
 */
void cleanup_vfs();

/**
 * Creates a new file in the VFS.
 *
 * @param name Name of the file.
 * @param data Data to be stored in the file.
 * @param VTYPE Type of the file (e.g., VREG for regular file).
 * @return 0 on success, -1 on failure.
 */
int create_file(const char *name, const char *data, enum vtype VTYPE);

/**
 * Deletes a file from the VFS.
 *
 * @param filename Name of the file to delete.
 * @return 0 on success, -1 if the file is not found.
 */
int delete_file(const char *filename);

/**
 * Reads the contents of a file.
 *
 * @param filename Name of the file to read.
 * @return Pointer to the file data, or NULL if the file is not found.
 */
const char *read_file(const char *filename);

/**
 * Creates a new directory in the VFS.
 *
 * @param name Name of the directory.
 * @return 0 on success, -1 on failure.
 */
int create_directory(const char *name);

/**
 * Displays all files in the root directory.
 */
void display_all_files();

#endif // !__VFS_H__
