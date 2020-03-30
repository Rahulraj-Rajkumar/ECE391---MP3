#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "types.h"

#define NAME_SIZE   32
#define BLOCK_SIZE  4096
#define NUM_DENTRIES    63
#define SUCCESS     0
#define FAILURE        -1

typedef struct {
    int8_t file_name[NAME_SIZE];
    uint32_t file_type;
    uint32_t inode_num;
    uint32_t reserved[6];
} dentry_t;

typedef struct{
    uint32_t num_dentries;
    uint32_t num_inodes;
    uint32_t num_data_blocks;
    uint32_t reserved[13];
    dentry_t dentries[NUM_DENTRIES];
} boot_block_t;

typedef struct{
    uint32_t length;
    uint32_t data_block[BLOCK_SIZE/4-1];
} inode_t;

typedef struct{
    uint8_t data[BLOCK_SIZE];
} data_block_t;

int32_t init_file_system (uint32_t file_system_begin, uint32_t file_system_end);
int32_t read_dentry_by_name (const int8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode_index, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t file_open (const uint8_t* filename, int32_t * fd);
int32_t file_close (int32_t fd);
int32_t file_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t file_read (int32_t fd, uint8_t* buf, int32_t nbytes, uint32_t * offset);

int32_t dir_open (const uint8_t* filename, int32_t * fd);
int32_t dir_close (int32_t fd);
int32_t dir_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_read (int32_t fd, uint8_t* buf, int32_t nbytes, uint32_t * offset);


#endif
