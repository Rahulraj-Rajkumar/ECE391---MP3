#include "file_system.h"
#include "lib.h"

boot_block_t* boot_block;

#define NUM_BYTES 50

int32_t init_file_system (uint32_t file_system_begin, uint32_t file_system_end){
    boot_block_t* possible_boot_block = (boot_block_t*) file_system_begin;

    if(possible_boot_block->num_dentries > NUM_DENTRIES) return FAILURE;

    //if(possible_boot_block + possible_boot_block->num_dentries + possible_boot_block->num_data_blocks + 1 != (boot_block_t*)file_system_end) return FAILURE;

    boot_block = possible_boot_block;
    return SUCCESS;
}

int32_t read_dentry_by_name (const int8_t* fname, dentry_t* dentry){
    int i;

    if (!dentry) return FAILURE;

    uint32_t num_bits = (strlen((int8_t*)fname) > NAME_SIZE) ? NAME_SIZE : strlen((int8_t*)fname);
    for (i = 0; i < NUM_DENTRIES; i++){
        if(!strncmp((int8_t*)fname, boot_block->dentries[i].file_name, num_bits)){
            *dentry = boot_block->dentries[i];
            return SUCCESS;
        }
    }
    return FAILURE;
}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if (!dentry) return FAILURE;

    if(index >= NUM_DENTRIES) return FAILURE;

    *dentry = boot_block->dentries[index];
    return SUCCESS;
}

int32_t read_data (uint32_t inode_index, uint32_t offset, uint8_t* buf, uint32_t length){
    inode_t* inode;
    data_block_t* data_block;
    uint32_t num_bytes;

    if(inode_index >= boot_block->num_inodes) return FAILURE;
    else inode = (inode_t*)boot_block+(inode_index+1);
    
    if(offset > inode->length) return FAILURE;
    else data_block = (data_block_t*)boot_block + (boot_block->num_inodes+1 + inode->data_block[offset/BLOCK_SIZE]);
    
    if(!buf) return FAILURE;
    
    num_bytes = (offset+length > inode->length) ? (inode->length - offset) : (length);

    int i;
    for(i=offset; i < offset+num_bytes; i++){
        if(i%BLOCK_SIZE == 0) data_block = (data_block_t*)boot_block + (boot_block->num_inodes+1 + inode->data_block[i/BLOCK_SIZE]);
        buf[i-offset] = data_block->data[i%BLOCK_SIZE];
    }
    return num_bytes;
}

void print_file(dentry_t* file){
    printf("File Name: ");
    printf(file->file_name);
    printf("\n");
    printf("File Type: ");
    switch (file->file_type)
    {
    case 0 :
        printf("RTC\n");
        break;
    case 1 :
        printf("Directory\n");
        break;
    case 2 :
        printf("File\n");
        break;
    default:
        return;
    }
    printf("inode Num: %d\n", file->inode_num);
    inode_t* inode = (inode_t*)boot_block+(file->inode_num+1);
    printf("inode Length in B: %d\n", inode->length);
    printf("File Contents:\n");
    uint8_t buf[NUM_BYTES];
    uint32_t i = 0;
    int j, num_bytes;
    while((num_bytes = file_read(file->inode_num, buf, NUM_BYTES, &i)))
        for(j = 0; j < num_bytes; j++) putc(buf[j]);
    printf("\n");
}

int32_t file_open (const uint8_t* filename, int32_t * fd){
    dentry_t file;
    if(-1 == read_dentry_by_name((int8_t*)filename, &file)) return FAILURE;
    *fd = file.inode_num;
    return SUCCESS;
}

int32_t file_close (int32_t fd){
    fd = 0;
    return SUCCESS;
}

int32_t file_write (int32_t fd, const void* buf, int32_t nbytes){
    return FAILURE;
}

int32_t file_read (int32_t fd, uint8_t* buf, int32_t nbytes, uint32_t * offset){
    uint32_t read;
    read = read_data(fd,*offset,buf,nbytes);
    if(read > 0) *offset += read;
    return read;
}

int32_t dir_open (const uint8_t* filename, int32_t * fd){
    dentry_t file;
    if(-1 == read_dentry_by_name((int8_t*)filename, &file)) return FAILURE;
    *fd = file.inode_num;
    return SUCCESS;
}

int32_t dir_close (int32_t fd){
    return SUCCESS;
}

int32_t dir_write (int32_t fd, const void* buf, int32_t nbytes){
    return FAILURE;
}

int32_t dir_read (int32_t fd, uint8_t* buf, int32_t nbytes, uint32_t * offset){
    dentry_t dir;
    if(-1 == read_dentry_by_index(*offset, &dir)) return FAILURE;
    if(nbytes > strlen(dir.file_name)) nbytes = strlen(dir.file_name);
    strncpy((int8_t*)buf, dir.file_name, nbytes); 
    *offset += (nbytes > 0);
    return nbytes;
}
