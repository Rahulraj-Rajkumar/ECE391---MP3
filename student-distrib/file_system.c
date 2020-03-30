#include "file_system.h"
#include "lib.h"

boot_block_t* boot_block;

#define NUM_BYTES 50

/* init_file_system
 * Inputs: uint32_t file_system_begin, uint32_t file_system_end
 * Outputs: 0 if work -1, if not
 * Side Effects: sets up boot block
 */
int32_t init_file_system (uint32_t file_system_begin, uint32_t file_system_end){
    //set up possible boot block based on the beginning of the file system
    boot_block_t* possible_boot_block = (boot_block_t*) file_system_begin;

    // if out of bounds fail
    if(possible_boot_block->num_dentries > NUM_DENTRIES) return FAILURE;

    //set boot block
    boot_block = possible_boot_block;
    return SUCCESS;
}

/* read_dentry_by_name
 *
 * Inputs: const int_t* fname (name of the file), dentry_t* dentry (directorny entry)
 * Outputs: 0 for success, -1 for fail
 * Side Effects: writes a dentry block to the input dentry
 */
int32_t read_dentry_by_name (const int8_t* fname, dentry_t* dentry){
    int i;

    //when null, fail
    if (!dentry) return FAILURE;

    //when the length of the file name is too long, num of bts to compare is 32
    uint32_t num_bits = (strlen((int8_t*)fname) > NAME_SIZE) ? NAME_SIZE : strlen((int8_t*)fname);

    //find the dentry based on the name in the dentries array in bootblock
    for (i = 0; i < NUM_DENTRIES; i++){
        if(!strncmp((int8_t*)fname, boot_block->dentries[i].file_name, num_bits)){
            *dentry = boot_block->dentries[i];
            return SUCCESS;
        }
    }
    // when everything works above success, else fail
    return FAILURE;
}

/* read_dentry_by_index
 *
 * Inputs: uint32_t index (index the file), dentry_t* dentry (directorny entry)
 * Outputs: 0 for success, -1 for fail
 * Side Effects: writes a dentry block to the input dentry
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    
    //when null, fail
    if (!dentry) return FAILURE;

    //when index is out of bounds fail
    if(index >= NUM_DENTRIES) return FAILURE;
    
    //find the dentry based on index in dentries array in boot block
    *dentry = boot_block->dentries[index];

    //when work, return success
    return SUCCESS;
}

/* read_data
 *
 * Inputs: uint32_t inode_index (index of inode), uint32_t offset (block offset), uint8_t* buf (shit thats read), uint32_t length(how much to read)
 * Outputs: how many bytes is read
 * Side Effects: writes to buf
 */
int32_t read_data (uint32_t inode_index, uint32_t offset, uint8_t* buf, uint32_t length){
    inode_t* inode;
    data_block_t* data_block;
    uint32_t num_bytes;

    //check if index is in bounds and offset is in bounds
    //make inode and data block with proper bounds if bounds are correct using addition
    if(inode_index >= boot_block->num_inodes) return FAILURE;
    else inode = (inode_t*)boot_block+(inode_index+1);
    
    if(offset > inode->length) return FAILURE;
    else data_block = (data_block_t*)boot_block + (boot_block->num_inodes+1 + inode->data_block[offset/BLOCK_SIZE]);
    
    if(!buf) return FAILURE;
    
    //calculate number of bytes read using offset and length
    num_bytes = (offset+length > inode->length) ? (inode->length - offset) : (length);

    //write to buffer by calculating data in each block by using the offset because the length can be greater than block size, use mod/divison 
    //to get correct data 
    int i;
    for(i=offset; i < offset+num_bytes; i++){
        if(i%BLOCK_SIZE == 0) data_block = (data_block_t*)boot_block + (boot_block->num_inodes+1 + inode->data_block[i/BLOCK_SIZE]);
        buf[i-offset] = data_block->data[i%BLOCK_SIZE];
    }
    return num_bytes;
}


/* print_file
 * Inputs: dentry_t* file
 * Outputs: prints the contents of the file
 * Side Effects: none
 */
void print_file(dentry_t* file){

    //print file name and file type
    printf("File Name: ");
    printf(file->file_name);
    printf("\n");
    printf("File Type: ");

    //prints either rtc, directory or file as type depending on what the value of file type is
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

    //prints the inode number and length
    printf("inode Num: %d\n", file->inode_num);
    inode_t* inode = (inode_t*)boot_block+(file->inode_num+1);
    printf("inode Length in B: %d\n", inode->length);

    //prints the file contents by using file_read to fill the buf
    printf("File Contents:\n");
    uint8_t buf[NUM_BYTES];
    uint32_t i = 0;
    int32_t fd = file->inode_num;
    int j, num_bytes;
    while((num_bytes = file_read(&fd, buf, NUM_BYTES, &i)))
        for(j = 0; j < num_bytes; j++) putc(buf[j]);
    printf("\n");
}


/* file_open
 *
 * Inputs: const uint8_t* filename (name of file), int32_t * fd (file descriptor)
 * Outputs: 0 if work, -1 if no
 * Side Effects: write to fd
 */
int32_t file_open (const uint8_t* filename, int32_t * fd){
    dentry_t file;
    //read dentry by filename and set fd
    if(-1 == read_dentry_by_name((int8_t*)filename, &file)) return FAILURE;
    *fd = file.inode_num;
    return SUCCESS;
}

/* file_close
 *
 * Inputs: int32_t * fd (file descriptor)
 * Outputs: 0 if work, -1 if no
 * Side Effects: write to fd
 */
int32_t file_close (int32_t * fd){
    // set fd to 0 to close
    * fd  = 0;
    return SUCCESS;
}

/* file_write
 *
 * Inputs: int32_t * fd, const void* buf, int32_t nbytes
 * Outputs: 0 if work, -1 if no
 * Side Effects: none
 */
int32_t file_write (int32_t * fd, const void* buf, int32_t nbytes){
    //should not write so auto fail
    return FAILURE;
}

/* file_read
 *
 * Inputs: int32_t* fd (file descriptor), uint8_t* buf (buffer), int32_t nbytes (number of bytes to read), uint32_t * offset (offset)
 * Outputs: number of read
 * Side Effects: calls read_data
 */
int32_t file_read (int32_t* fd, uint8_t* buf, int32_t nbytes, uint32_t * offset){
    uint32_t read;
    //data is read until end of file or until end of buffer
    read = read_data(*fd,*offset,buf,nbytes);
    if(read > 0) *offset += read;
    return read;
}

/* dir_open
 *
 * Inputs: const uint8_t* filename (file name), int32_t * fd (file descriptor)
 * Outputs: 0 if work, -1 if nto work
 * Side Effects: writes to fd
 */
int32_t dir_open (const uint8_t* filename, int32_t * fd){
    dentry_t file;
    //sets fd to inode number bsaed on the filename given
    if(-1 == read_dentry_by_name((int8_t*)filename, &file)) return FAILURE;
    *fd = file.inode_num;
    return SUCCESS;
}

/* dir_close
 *
 * Inputs: int32_t * fd (file descriptor)
 * Outputs: 0 if work, -1 if nto work
 * Side Effects: none
 */
int32_t dir_close (int32_t * fd){
    //should just be 0
    return SUCCESS;
}

/* dir_write
 *
 * Inputs: int32_t * fd, const void* buf, int32_t nbytes
 * Outputs: 0 if work, -1 if nto work
 * Side Effects: none
 */
int32_t dir_write (int32_t * fd, const void* buf, int32_t nbytes){
    //should not write so auto fail
    return FAILURE;
}

/* dir_read
 *
 * Inputs: int32_t * fd (file descriptor), uint8_t* buf (buffer), int32_t nbytes (number of bytes to read), uint32_t * offset (offset)
 * Outputs: number of bytes read
 * Side Effects: changes buf
 */
int32_t dir_read (int32_t * fd, uint8_t* buf, int32_t nbytes, uint32_t * offset){
    dentry_t dir;
    //read by index
    if(-1 == read_dentry_by_index(*offset, &dir)) return FAILURE;
    //sets the number of bytes to read to be the length of the filename if greater
    if(nbytes > strlen(dir.file_name)) nbytes = strlen(dir.file_name);
    //copies the filename to buf for what files are in the directory
    strncpy((int8_t*)buf, dir.file_name, nbytes); 
    // if there is more stuff to read, go to next one
    *offset += (nbytes > 0);
    return nbytes;
}
