#include "syscall_handler.h"
#include "int_wrapper.h"

// File Descriptor struct. If you need more info, RTDC 8.2
typedef struct fd_t {
    int32_t (*fop_jump_table[NUM_FOPS])();
    int32_t inode;
    int32_t file_type;
    int32_t file_position;
    uint32_t flags;
} fd_t;

// Process Control Block struct. If you need more info, RTDC 6.3.5
typedef struct pcb_t {
    uint8_t args[ARGS_SIZE];
    fd_t file_array[MAX_OPEN_FILES];
    int8_t pid;
    uint32_t parent_esp;
    uint32_t parent_ebp;
    int8_t parent_pid;
} pcb_t;

int process_array[MAX_OPEN_PROCESSES];

int curr_pid;

int first_process = 1;

int32_t no_func(){return -1;}

/* Initializes tables for stdin, stdout, RTC and files */ 
int32_t (*nofunc_table[NUM_FOPS])() = {no_func, no_func, no_func, no_func}; 
int32_t (*std_table[2][NUM_FOPS])() = {
{terminal_read, no_func, terminal_open, terminal_close}, 
{no_func, terminal_write, terminal_open, terminal_close}
};
int32_t (*file_table[NUM_FILE_TYPES][NUM_FOPS])() = {
{rtc_read, rtc_write, rtc_open, rtc_close},
{dir_read, dir_write, dir_open, dir_close},
{file_read, file_write, file_open, file_close}
};
/*
* halt
*   DESCRIPTION:    expand arg from parent program when terminating process
*   INPUTS:         uint8_t status - 8 bit argument from BL
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   expand status to return value of parent program's execute sys call
*
*/
int32_t halt(uint8_t status) {
    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (curr_pid + 1) * K_STACK_SIZE);

    //check for bounds of parent process, if good, go to helper function to change process
    if(curr_pcb->parent_pid < 0)
    {
        printf("Sorry you can't halt the shell");
        return status;
    }
    else
    {
        change_process(curr_pcb->parent_pid);
        tss.esp0 = K_MEM_END - K_STACK_SIZE * curr_pcb->parent_pid - WORD_SIZE;
    }   

    process_array[curr_pid] = 0;

    curr_pid = curr_pcb->parent_pid;

	asm volatile("movl %0, %%esp"::"g"(curr_pcb->parent_esp)); // Asm stuff put the "parent_ksp" into the %ESP
	asm volatile("movl %0, %%ebp"::"g"(curr_pcb->parent_ebp)); // Asm stuff put the "parent_kbp" into the %EBP
	
	asm volatile("movl %0, %%eax"::"g"(status)); // Asm stuff put the "status" into the %EAX
    
    asm volatile("leave");
	asm volatile("ret");
    return 0;    
}


/*
* get_next_pid
*   DESCRIPTION:    gets new process in process array
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   next process in array
*   SIDE EFFECTS:   none
*
*/
int get_next_pid(){
    int i;
    //returns the next in array
    for(i = 0; process_array[i] && i < MAX_OPEN_PROCESSES; i++){}
    return i;
}

/*
* execute
*   DESCRIPTION:    load and executues a new program  
*   INPUTS:         uint8_t command
*   OUTPUTS:        none
*   RETURN VALUE:   -1 is fail, 0 is ok
*   SIDE EFFECTS:   none
*
*/
int32_t execute(const uint8_t* command) {
    //initialize variables
    uint8_t fname[NAME_SIZE];
    uint8_t buf[4];
    dentry_t dentry;
    int i, j, pid;
    uint32_t offset = 0;

    // Null check command
    if(!command) return FAILURE;

    // if no available next process ID, return -1
    if((pid = get_next_pid()) >= MAX_OPEN_PROCESSES) return FAILURE;


    // get valid command
    for(i = 0; i < 32 && command[i] != ' '; i++) fname[i] = command[i];

    

    // if file is valid, continue
    if(read_dentry_by_name((int8_t*)fname, &dentry)) return FAILURE;
    if(!file_read(dentry.inode_num, buf, 4, offset)) return FAILURE;
    if(strncmp((const int8_t*)buf, "ELF", 4)) return FAILURE;

    change_process(pid);

    if(load_program(dentry.inode_num, (uint8_t *)USR_START_ADDR)) return FAILURE;
        
    curr_pid = pid;

    process_array[pid] = 1;

    /* for every open process, set PCB */
    pcb_t* pcb = (pcb_t*)(K_MEM_END - KSTACK_SIZE * (pid+1));

    for(j = 0; command[i+j] != '\0'; j++) pcb->args[j] = command[i+j];


    for(i = 0; i < MAX_OPEN_FILES; i++){
        for(j = 0; j < NUM_FOPS; j++) pcb->file_array[i].fop_jump_table[j] = (i < 2) ? std_table[i][j] : nofunc_table[j];
        pcb->file_array[i].inode = 0;
        pcb->file_array[i].file_type = -1;
        pcb->file_array[i].file_position = 0;
        pcb->file_array[i].flags = (i < 2) ? 1 : 0;
    }
    
    pcb->pid = pid;

    /* close out of function safely by handling registers */
    int esp, ebp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    asm("movl %%ebp, %0" : "=r"(ebp) :);
    pcb->parent_esp = esp;
    pcb->parent_ebp = ebp;
    //pcb->ret_addr = *(uint32_t*)(ebp);
    pcb->parent_pid = (first_process) ? -1 : ((pcb_t *)(esp & 0xFFFFE000))->pid;

    first_process = 0;

    
    return_to_user(pid);

    return 0;
}

/*
* read
*   DESCRIPTION:    reads data
*   INPUTS:         int32_t fd - file directory, void* buf - buffer to write to, int32_t nbytes - bytes successful
*   OUTPUTS:        none
*   RETURN VALUE:   nbytes
*   SIDE EFFECTS:   none
*
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    int esp, retval;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb_t* pcb = (pcb_t *)(esp & 0xFFFFE000);

    //if out of bounds fail
    if(fd < 0 || fd >= MAX_OPEN_FILES || !buf) return FAILURE;

    sti();

    //read file from array and file directory, return nbytes read
    retval = pcb->file_array[fd].fop_jump_table[READ_INDEX](pcb->file_array[fd].inode, buf, nbytes, pcb->file_array[fd].file_position);


    pcb->file_array[fd].file_position += (pcb->file_array[fd].file_type == 1) ? (retval > 0) : retval;

    return retval;
}

/*
* write
*   DESCRIPTION:    writes data
*   INPUTS:         nt32_t fd - file directory, void* buf - buffer to write to, int32_t nbytes - bytes successful
*   OUTPUTS:        none
*   RETURN VALUE:   nbytes
*   SIDE EFFECTS:   writes
*
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    int esp, retval;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb_t* pcb = (pcb_t *)(esp & PCB_WRITE_BITMASK);
    
    // check bounds
    if(fd < 0 || fd >= MAX_OPEN_FILES || !buf) return FAILURE;

    retval = pcb->file_array[fd].fop_jump_table[WRITE_INDEX](pcb->file_array[fd].inode, buf, nbytes);

    return retval;
}

/*
* open
*   DESCRIPTION:    provides access to the file system
*   INPUTS:         uint8_t filename - fname
*   OUTPUTS:        dentry found
*   RETURN VALUE:   index
*   SIDE EFFECTS:   set up data
*
*/
int32_t open(const uint8_t* filename) {
    dentry_t dentry;
    int i, j, esp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb_t* pcb = (pcb_t *)(esp & 0xFFFFE000);

    //checks bounds
    if(!filename) return FAILURE;

    read_dentry_by_name((const int8_t*)filename, &dentry);

    for(i = 0; i < 8 && pcb->file_array[i].flags; i++);

    if(i == 8) return FAILURE;

    //finds dentry based on name, allocated unused fd and set up data to handle file type
    for(j = 0; j < NUM_FOPS; j++) pcb->file_array[i].fop_jump_table[j] = file_table[dentry.file_type][j];
    pcb->file_array[i].inode = dentry.inode_num;
    pcb->file_array[i].file_type = dentry.file_type;
    pcb->file_array[i].file_position = 0;
    pcb->file_array[i].flags = 1;

    if(pcb->file_array[i].fop_jump_table[OPEN_INDEX](filename)) return FAILURE;

    return i;
}

/*
* close
*   DESCRIPTION:    closes fd
*   INPUTS:         int32_t fd - file descriptor
*   OUTPUTS:        none
*   RETURN VALUE:   -1 for fail, 0 for good
*   SIDE EFFECTS:   closes fd
*
*/
int32_t close(int32_t fd) {
    int esp, retval;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb_t* pcb = (pcb_t *)(esp & 0xFFFFE000);

    //check bounds
    if(fd < 0 || fd >= MAX_OPEN_FILES) return FAILURE;

    retval = pcb->file_array[fd].fop_jump_table[CLOSE_INDEX](pcb->file_array[fd].inode);

    return retval;
}

/* TODO
* getargs
*   DESCRIPTION:   Allows for executables to accept arguments (such as cat and grep)
*   INPUTS:         buf - command to be executed
*                   nbytes - size of args
*   OUTPUTS:        0 if success, -1 if not
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t getargs(uint8_t* buf, int32_t nbytes) {

    /* check if parameters are valid */
    if( buf == NULL || nbytes == 0 )
	{
		return -1;
	}
	
    /* get current pcb struct */
    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (curr_pid + 1) * K_STACK_SIZE);
	
    /* check if arguments too long */
	if( strlen((const int8_t*)curr_pcb->args) > nbytes )
	{
		return -1;
	}
	/* update pcb->args with buf data */
	strcpy((int8_t*)buf, (const int8_t*)curr_pcb->args);
	return 0;
}

/* TODO
* vidmap
*   DESCRIPTION:    maps text-mode-video memory into user space
*   INPUTS:         uint9_t screen start- start of screen data
*   OUTPUTS:        none
*   RETURN VALUE:   0 if success, -1 if not 
*   SIDE EFFECTS:   sets up video memory for something like fish program
*
*/
int32_t vidmap(uint8_t** screen_start) {

    /* check if screen start is a valid parameter for sanity check */
    if(screen_start == NULL) return -1;
    if((uint32_t) screen_start < USER_VID_MEM - FOUR_MB) return -1;
    if((uint32_t) screen_start >= USER_VID_MEM) return -1;

/* set screen start to specific location in memory */
    *screen_start = (uint8_t *)(USER_VID_MEM);
    return 0;
}

/* TODO
* set_handler
*   DESCRIPTION:    signal handling
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   do nothing rn
*
*/
int32_t set_handler(int32_t signum, void* handler_address) {
    return -1;
}

/* TODO
* sigreturn
*   DESCRIPTION:    signal handling
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   do nothing rn
*
*/
int32_t sigreturn(void) {
    return -1;
}

/* TODO
* user_execute_helper
*   DESCRIPTION: Makes sure user level code is executed
*                at privelege level 3
*   INPUTS:         process_id: 0 indexed pid of current pcb
*   OUTPUTS:        none
*   RETURN VALUE:   none    
*   SIDE EFFECTS:   Switches execution privilege from 0 to 3
*
*/
void return_to_user(int process_id) {

        // Get value for EIP from byte 24 onwards from the user memory start address
        uint32_t EIP = (*(uint32_t *)(USR_START_ADDR + ADDR_DIST_EIP)   );

        // set TSS Kernel mode stack to proper value
        tss.esp0 = K_MEM_END - (process_id) * K_STACK_SIZE - WORD_SIZE;
        
        // set TSS Kernel stack segment to proper value
        tss.ss0 = KERNEL_DS;

        // Set DS to the proper value then Push SS, ESP, 
        // EFLAGS, CS, and EIP to stack in that order then iret
        asm volatile("                        \n\
            andl    $0x00FF, %%eax            \n\
            movw    %%ax, %%ds                \n\
            pushl   %%eax                     \n\
            pushl   %%edx                     \n\
            pushfl                            \n\
            pushl   %%ecx                     \n\
            pushl   %%ebx                     \n\
            iret                              \n\
            "
            : 
            : "a"(USER_DS), "b"(EIP), "c"(USER_CS), "d"(USR_STACK_ADDR)
            : "cc"
        );
}
