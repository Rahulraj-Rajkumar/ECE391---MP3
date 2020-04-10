#include "syscall_handler.h"
#include "int_wrapper.h"

// File Descriptor struct. If you need more info, RTDC 8.2
typedef struct fd_t {
    // I got no clue about these types so if another type is more convenient go for it
    uint32_t (*fop_jump_table[NUM_FOPS])();
    int32_t inode;
    int32_t file_position;
    uint32_t flags;
} fd_t;

// Process Control Block struct. If you need more info, RTDC 6.3.5
typedef struct pcb_t {
    fd_t file_array[MAX_OPEN_FILES];
    int8_t pid;
    uint32_t parent_ksp;
    // There should be a bunch more shit stored in this struct but the documentation says "signal information"
    // and idk what that means.  Probably u wanna store like the parent pcb pointer and also like if it is parent
    // or child process and other shit idk.
} pcb_t;

int num_processes;

bool process_array[MAX_OPEN_PROCESSES]

uint32_t (*default_fops_tables[MAX_OPEN_PROCESSES][NUM_FOPS])() = {
{no_func, terminal_read, no_func, no_func},
{no_func, no_func, terminal_write, no_func},
{no_func, no_func, no_func, no_func},
{no_func, no_func, no_func, no_func},
{no_func, no_func, no_func, no_func},
{no_func, no_func, no_func, no_func},
{no_func, no_func, no_func, no_func},
{no_func, no_func, no_func, no_func}
};
/*
* halt
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t halt(uint8_t status) {
    return 0;
}

/*
* execute
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t execute(const uint8_t* command) {
    uint8_t fname[NAME_SIZE];
    uint8_t buf[4];
    dentry_t dentry;
    int32_t fd;
    int i, j;
    for(i = 0; i < 32 && command[i] != ' '; i++) fname[i] = command[i];


    if(file_open(fname, &fd)) return FAILURE;
    if(file_read(fd, buf, 4, 0)) return FAILURE;
    if(strncmp(buf, "ELF", 4)) return FAILURE;



    new_process(++num_processes);

    // Vishnu to implement loader here

    pcb_t* pcb = 0x800000 - KSTACK_SIZE * num_processes;

    for(i = 0; i < MAX_OPEN_PROCESSES; i++){
        for(j = 0; j < NUM_FOPS; j++){
            pcb->file_array[i].fop_jump_table[j] = default_fops_tables[i][j];
        }
        pcb->file_array[i].inode = 0;
        pcb->file_array[i].file_position = 0;
        pcb->file_array[i].flags = (i < 2) ? 1 : 0;
    }
    pcb->pid = num_processes;
    int esp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb->parent_ksp = esp;
    
    return 0;
}

/*
* read
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    return 0;
}

/*
* write
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    return 0;
}

/*
* open
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t open(const uint8_t* filename) {
    return 0;
}

/*
* close
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t close(int32_t fd) {
    return 0;
}

/* TODO
* getargs
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t getargs(uint8_t* buf, int32_t nbytes) {
    return 0;
}

/* TODO
* vidmap
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t vidmap(uint8_t** screen_start) {
    return 0;
}

/* TODO
* set_handler
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t set_handler(int32_t signum, void* handler_address) {
    return 0;
}

/* TODO
* sigreturn
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t sigreturn(void) {
    return 0;
}

void no_func(){}

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
void user_execute_helper(int32_t process_id) {

        // Get value for EIP from byte 24 onwards from the user memory start address
        uint32_t EIP = (*(uint32_t *)((char *)USR_START_ADDR + ADDR_DIST_EIP));

        // set TSS Kernel mode stack to proper value
        tss.esp0 = KERNEL_MEM_END - (process_id) * K_STACK_SIZE - WORD_SIZE;
        
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

