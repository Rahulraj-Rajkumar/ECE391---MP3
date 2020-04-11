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
    fd_t file_array[MAX_OPEN_PROCESSES];
    int8_t pid;
    uint32_t parent_esp;
    uint32_t parent_ebp;
    int8_t parent_pid;
    // There should be a bunch more shit stored in this struct but the documentation says "signal information"
    // and idk what that means.  Probably u wanna store like the parent pcb pointer and also like if it is parent
    // or child process and other shit idk.
} pcb_t;

int process_array[MAX_OPEN_PROCESSES];

int curr_pid;

int first_process = 1;

uint32_t (*nofunc_table[NUM_FOPS])() = {no_func, no_func, no_func, no_func}; 
uint32_t (*std_table[2][NUM_FOPS])() = {
{terminal_read, no_func, terminal_open, terminal_close}, 
{no_func, terminal_write, terminal_open, terminal_close}
};
uint32_t (*file_table[NUM_FILE_TYPES][NUM_FOPS])() = {
{rtc_read, rtc_write, rtc_open, rtc_close},
{file_read, file_write, file_open, file_close},
{dir_read, dir_write, dir_open, dir_close}
};
/*
* halt
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t halt(uint8_t status) {
    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - curr_pid * K_STACK_SIZE);


    if((curr_pcb->parent_esp & PCB_BITMASK) == NULL)
    {
        tss.esp0 = K_MEM_END;
    }
    else
    {
        add_newpage(curr_pid, K_MEM_END + curr_pcb->parent_pid * FOUR_MB);
        tss.esp0 = K_MEM_END - K_STACK_SIZE * curr_pcb->parent_pid - WORD_SIZE;
    }   

    process_array[curr_pid] = 0;

    asm volatile("              \n\
        movl    %0, %%esp       \n\
        movl    %1, %%ebp       \n\
        movb    %2, %%eax       \n\
        jmp     HALT_RET_LABEL  \n\
        "
        :
        : "r" (curr_pcb->parent_esp), "r" (curr_pcb->parent_ebp), "r" (status)
        : "cc"
    );

    asm volatile(
		"HALT_RET_LABEL: \n\
		leave \n\
		ret \n\
		"
	);
    return 0;    
}

/*
* execute
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t execute(const uint8_t* command) {

    uint8_t fname[NAME_SIZE];
    uint8_t buf[4];
    dentry_t dentry;
    int32_t fd;
    int i, j, pid;
    int offset = 0;
    if((pid = get_next_pid()) > MAX_OPEN_PROCESSES) return FAILURE;

    for(i = 0; i < 32 && command[i] != ' '; i++) fname[i] = command[i];

    if(file_open(fname, &fd)) return FAILURE;
    if(file_read(&fd, buf, 4, &offset)) return FAILURE;
    if(strncmp(buf, "ELF", 4)) return FAILURE;



    new_process(pid);

    if(load_program(fd, (uint8_t *)USR_START_ADDR)) return FAILURE;
        
    curr_pid = pid;

    pcb_t* pcb = K_MEM_END - KSTACK_SIZE * pid;

    for(i = 0; i < MAX_OPEN_PROCESSES; i++){
        for(j = 0; j < NUM_FOPS; j++) pcb->file_array[i].fop_jump_table[j] = (i < 2) ? std_table[i] : nofunc_table;
        pcb->file_array[i].inode = 0;
        pcb->file_array[i].file_position = 0;
        pcb->file_array[i].flags = (i < 2) ? 1 : 0;
    }
    pcb->pid = pid;
    

    int esp, ebp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    asm("movl %%ebp, %0" : "=r"(ebp) :);
    pcb->parent_esp = esp;
    pcb->parent_ebp = ebp;
    pcb->parent_pid = (first_process) ? 0 : ((pcb_t *)(esp & 0xFFFFE000))->pid;

    first_process = 0;
    
    return_to_user(pid);

    return 0;
}

/*
* read
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes) {
    return 0;
}

/*
* write
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
    return 0;
}

/*
* open
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t open(const uint8_t* filename) {
    return 0;
}

/*
* close
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t close(int32_t fd) {
    return 0;
}

/* TODO
* getargs
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t getargs(uint8_t* buf, int32_t nbytes) {
    return 0;
}

/* TODO
* vidmap
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t vidmap(uint8_t** screen_start) {
    return 0;
}

/* TODO
* set_handler
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
*
*/
int32_t set_handler(int32_t signum, void* handler_address) {
    return 0;
}

/* TODO
* sigreturn
*   DESCRIPTION:  
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   none
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
void return_to_user(int32_t process_id) {

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

int get_next_pid(){
    int i;
    for(i = 0; process_array[i] && i < 8; i++){}
    return i;
}