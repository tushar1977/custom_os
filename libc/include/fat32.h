#ifndef __FAT_H__
#define __FAT_H__

typedef struct fat_32 {

  unsigned int table_size_32;
  unsigned short extended_flags;
  unsigned short fat_version;
  unsigned int root_cluster;
  unsigned short fat_info;
  unsigned short backup_BS_sector;
  unsigned char reserved_0[12];
  unsigned char drive_number;
  unsigned char reserved_1;
  unsigned char boot_signature;
  unsigned int volume_id;
  unsigned char volume_label[11];
  unsigned char fat_type_label[8];
} __attribute__((packed)) fat;

#endif // __FAT_H__
