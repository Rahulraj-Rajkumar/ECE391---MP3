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
    int8_t leaf;
    uint32_t parent_esp;
    uint32_t parent_ebp;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    int8_t parent_pid;
    int8_t terminal;
} pcb_t;

// indices represent numbered PID's, values stored is a bool flag showing status of process (0 is off, 1 is active)
int process_array[MAX_OPEN_PROCESSES];

// indices represent the number of terminal, and values stored is the PID of the active process in that terminal 
int terminal_array[NUM_TERMINALS] = {-1, -1, -1};

// current active terminal that is being run by the scheduler
int curr_terminal;

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
    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (terminal_array[curr_terminal] + 1) * K_STACK_SIZE);

    // Check for bounds of parent process, if good, go to helper function to change process.
    // if there is no parent process, restart the shell
    if(curr_pcb->parent_pid < 0)
    {
        printf("Sorry you can't halt the base shell\n");

        // Executes a "new" shell.  PCB is already set so we do not have to initialize it.

        dentry_t dentry;
        uint8_t fname[6] = "shell\0";
        uint8_t buf[4];
        uint32_t offset = 0;

        if(read_dentry_by_name((int8_t*)fname, &dentry)) return FAILURE;
        if(!file_read(dentry.inode_num, buf, 4, offset)) return FAILURE;
        if(strncmp((const int8_t*)buf, "ELF", 4)) return FAILURE;

        return_to_user(terminal_array[curr_terminal]);
    }
    else
    {
        // Close all open files
        int i;
        for(i = 0; i < 8 && curr_pcb->file_array[i].flags; i++)
        {
            close(i);
        }
        change_process(curr_pcb->parent_pid);
        tss.esp0 = K_MEM_END - K_STACK_SIZE * curr_pcb->parent_pid - WORD_SIZE;
    }   

    process_array[terminal_array[curr_terminal]] = 0;

    terminal_array[curr_terminal] = curr_pcb->parent_pid;

    ((pcb_t *)(K_MEM_END - (curr_pcb->parent_pid + 1) * K_STACK_SIZE))->leaf = 1;

	asm volatile("movl %0, %%esp"::"g"(curr_pcb->parent_esp)); // Asm stuff put the "parent_ksp" into the %ESP

    asm volatile("pushl $0"::"g"(status)); // We have to push the status on top of the stack, we will not have access to local vars when we change ebp

	asm volatile("movl %0, %%ebp"::"g"(curr_pcb->parent_ebp)); // Asm stuff put the "parent_kbp" into the %EBP
	
	asm volatile("popl %eax"); // Pop the status into eax because it is the return value
    
    asm volatile("leave ");
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
* execute_shells
*   DESCRIPTION: executes the base shells in the three terminals
*   INPUTS: buf - command to execute
*   OUTPUTS: none
*   RETURN VALUE: 0 success, -1 fail
*   SIDE EFFECTS: starts up three shells on three terminals
*
*/

int32_t execute_shells() {
    //initialize variables
    // uint8_t command[SHELL_LENGTH] = "shell ";
    uint8_t fname[SHELL_LENGTH] = "shell\0";
    uint8_t buf[SHELL_BYTES];
    dentry_t dentry;
    int i, j, k;
    uint32_t offset = 0;
    uint32_t EIP = 0;
    pcb_t* pcb;

    // get shell command and get info for it
    // for(i = 0; i < MAX_NAME_LENGTH && command[i] != ' '; i++) fname[i] = command[i];
     // if file is valid, continue
    if(read_dentry_by_name((int8_t*)fname, &dentry)) return FAILURE;
    if(!file_read(dentry.inode_num, buf, SHELL_BYTES, offset)) return FAILURE;

    /* execute shells by iterating through terminals and setting fields accordingly*/
    for (i = SECOND_TERMINAL; i >= 0; i--) {
        // set global variables
        terminal_array[i] = i;
        process_array[i] = 1;

        change_process(i);

        // load the program
        if(load_program(dentry.inode_num, (uint8_t *)USR_START_ADDR)) return FAILURE;

        // Get value for EIP from byte 24 onwards from the user memory start address
        EIP = (*(uint32_t *)(USR_START_ADDR + ADDR_DIST_EIP));

        /* for every shell, set PCB fields */
        pcb = (pcb_t*)(K_MEM_END - KSTACK_SIZE * (i+1));

        for(k = 0; k < MAX_OPEN_FILES; k++){
            for(j = 0; j < NUM_FOPS; j++) pcb->file_array[k].fop_jump_table[j] = (k < STD_FILES) ? std_table[k][j] : nofunc_table[j];
            pcb->file_array[k].inode = 0;
            pcb->file_array[k].file_type = -1;
            pcb->file_array[k].file_position = 0;
            pcb->file_array[k].flags = (i < STD_FILES) ? 1 : 0;
        }

        pcb->pid = i;

        pcb->leaf = 1;

        pcb->terminal = i;

        pcb->parent_pid = -1;
        
        pcb->terminal = i;

        // set TSS Kernel mode stack to proper value
        tss.esp0 = K_MEM_END - (i) * K_STACK_SIZE - WORD_SIZE;
        
        // set TSS Kernel stack segment to proper value
        tss.ss0 = KERNEL_DS;

        if(i != 0){
            // Set DS to the proper value then Push SS, ESP, 
            // EFLAGS, CS, and EIP to stack in that order then iret
            asm volatile("                        \n\
                movl    %%esp, %%ecx              \n\
                movl    %%ebp, %%edx              \n\
                movl    %%eax, %%esp              \n\
                movl    %%eax, %%ebp              \n\
                pushl   %0                        \n\
                pushl   %2                        \n\
                pushfl                            \n\
                pushl   %1                        \n\
                pushl   %%ebx                     \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $0                        \n\
                pushl   $pit_int_ret              \n\
                pushl   %%eax                     \n\
                movl    %%ecx, %%esp              \n\
                movl    %%edx, %%ebp              \n\
                "
                : 
                : "g"(USER_DS), "g"(USER_CS), "g"(USR_STACK_ADDR), "a"(tss.esp0), "b"(EIP)
                : "cc"
            );
        }

        pcb->saved_ebp = K_MEM_END - (i) * K_STACK_SIZE - PUSHED_REGS_SIZE;
        pcb->saved_esp = K_MEM_END - (i) * K_STACK_SIZE - PUSHED_REGS_SIZE;
    }

    // set current terminal to the 0th shell's terminal, then return to user
    curr_terminal = pcb->terminal;

    sti();

    return_to_user(pcb->pid);

    return 0; 
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
    uint8_t args[ARGS_SIZE];

    for(i = 0; i < NAME_SIZE; i++)
    {
        fname[i] = '\0';
    }

    for(i = 0; i < ARGS_SIZE; i++)
    {
        args[i] = '\0';
    }

    // Null check command
    if(!command) return FAILURE;

    // if no available next process ID, return -1
    if((pid = get_next_pid()) >= MAX_OPEN_PROCESSES) return FAILURE;


    // get valid command
    for(i = 0; i < MAX_NAME_LENGTH && command[i] != ' '; i++) fname[i] = command[i];

    i++;

    for(j = 0; command[i+j] != '\0'; j++) args[j] = command[i+j];
    
    args[j] = '\0';

    // if file is valid, continue
    if(read_dentry_by_name((int8_t*)fname, &dentry)) return FAILURE;
    if(!file_read(dentry.inode_num, buf, 4, offset)) return FAILURE;
    if(strncmp((const int8_t*)buf, "ELF", 4)) return FAILURE;

    change_process(pid);

    if(load_program(dentry.inode_num, (uint8_t *)USR_START_ADDR)) return FAILURE;

    process_array[pid] = 1;

    terminal_array[curr_terminal] = pid;

    /* for every open process, set PCB fields */
    pcb_t* pcb = (pcb_t*)(K_MEM_END - KSTACK_SIZE * (pid+1));

    for(i = 0; i < ARGS_SIZE; i++)
    {
        pcb->args[i] = '\0';
    }

    for(i = 0; args[i] != '\0'; i++) pcb->args[i] = args[i];


    for(i = 0; i < MAX_OPEN_FILES; i++){
        for(j = 0; j < NUM_FOPS; j++) pcb->file_array[i].fop_jump_table[j] = (i < 2) ? std_table[i][j] : nofunc_table[j];
        pcb->file_array[i].inode = 0;
        pcb->file_array[i].file_type = -1;
        pcb->file_array[i].file_position = 0;
        pcb->file_array[i].flags = (i < 2) ? 1 : 0;
    }
    
    pcb->pid = pid;

    int esp, ebp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    asm("movl %%ebp, %0" : "=r"(ebp) :);
    pcb->parent_esp = esp;
    pcb->parent_ebp = ebp;

    pcb->terminal = ((pcb_t *)(esp & PCB_BITMASK))->terminal;

    ((pcb_t *)(esp & PCB_BITMASK))->leaf = 0;

    pcb->leaf = 1;

    /* close out of function safely by handling registers */
    //pcb->ret_addr = *(uint32_t*)(ebp);
    pcb->parent_pid =  ((pcb_t *)(esp & PCB_BITMASK))->pid;
    
    terminal_array[(int)((pcb_t *)(esp & PCB_BITMASK))->terminal] = pid;

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
    pcb_t* pcb = (pcb_t *)(esp & PCB_BITMASK);

    //if out of bounds fail
    if(fd < 0 || fd >= MAX_OPEN_FILES || !buf) return FAILURE;


    sti();

    //read file from array and file directory, return nbytes read
    retval = pcb->file_array[fd].fop_jump_table[READ_INDEX](pcb->file_array[fd].inode, buf, nbytes, pcb->file_array[fd].file_position);


    pcb->file_array[fd].file_position += (pcb->file_array[fd].file_type == 1) ? (retval > 0) : retval;

    if(pcb->file_array[fd].flags == 0 && retval == 1) return FAILURE;

    return (retval);
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
    int test;
    asm("movl %%esp, %0" : "=r"(esp) :);
    pcb_t* pcb = (pcb_t *)(esp & PCB_BITMASK);

    //checks bounds
    if(!filename) return FAILURE;

    test = read_dentry_by_name((const int8_t*)filename, &dentry);

    if(test == -1)
    {
        return FAILURE;
    }

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
    pcb_t* pcb = (pcb_t *)(esp & PCB_BITMASK);

    //check bounds
    if(fd < 0 || fd >= MAX_OPEN_FILES) return FAILURE;
    if(pcb->file_array[fd].flags == 0) return FAILURE;

    retval = pcb->file_array[fd].fop_jump_table[CLOSE_INDEX](pcb->file_array[fd].inode);

    // reset the fields of the file array corresponding to the closed file
    pcb->file_array[fd].inode = 0;
    pcb->file_array[fd].file_type = -1;
    pcb->file_array[fd].file_position = 0;
    pcb->file_array[fd].flags = 0;

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
    int i;

    /* check if parameters are valid */
    if( buf == NULL || nbytes == 0 )
	{
		return -1;
	}
	
    /* get current pcb struct */
    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (terminal_array[curr_terminal] + 1) * K_STACK_SIZE);
	
    /* check if arguments too long */
	if( strlen((const int8_t*)curr_pcb->args) > nbytes )
	{   
		return -1;
	}

	/* update pcb->args with buf data */
    for(i = 0; i < 1024; i++)
    {
        buf[i] = '\0';
    }

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
    if(get_curr_pcb_term() == 0)
    {
        *screen_start = (uint8_t *)(VID1);
    }
    else if(get_curr_pcb_term() == 1)
    {
        *screen_start = (uint8_t *)(VID2);
    }
    else if(get_curr_pcb_term() == 2)
    {
        *screen_start = (uint8_t *)(VID3);
    }

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
        uint32_t EIP = (*(uint32_t *)(USR_START_ADDR + ADDR_DIST_EIP));

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

void pit_int(){
    cli();

    send_eoi(PIT_IRQ);

    if(terminal_array[curr_terminal] == -1) return;

    uint8_t next_pid;

    for(next_pid = (terminal_array[curr_terminal] + 1) % MAX_OPEN_PROCESSES; next_pid != terminal_array[curr_terminal]; next_pid = (next_pid + 1) % 6){
        if(process_array[next_pid] && ((pcb_t *)(K_MEM_END - (next_pid + 1) * K_STACK_SIZE))->leaf) break;
    }

    if(next_pid == terminal_array[curr_terminal]) return;

    pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (terminal_array[curr_terminal] + 1) * K_STACK_SIZE);
    pcb_t * next_pcb = (pcb_t *)(K_MEM_END - (next_pid + 1) * K_STACK_SIZE);

    uint32_t ebp, esp;
    asm("movl %%esp, %0" : "=r"(esp) :);
    asm("movl %%ebp, %0" : "=r"(ebp) :);
    curr_pcb->saved_esp = esp;
    curr_pcb->saved_ebp = ebp;

    change_process(next_pid);
    tss.esp0 = K_MEM_END - K_STACK_SIZE * next_pid - WORD_SIZE;
    
    curr_terminal = next_pcb->terminal;

    asm volatile("movl %0, %%esp"::"g"(next_pcb->saved_esp)); // Asm stuff put the "ksp" into the %ESP
	asm volatile("movl %0, %%ebp"::"g"(next_pcb->saved_ebp)); // Asm stuff put the "kbp" into the %EBP
    asm volatile("leave");
	asm volatile("ret");

    sti();
}

void initialize_pit() {
    disable_irq(PIT_IRQ);

    /* we gonna turn on periodic interrupts on the RTC */
    outb(PIT_MODE, MCREG);                   // selects channel 0, sets access mode 10 lobyte/hibyte, and enables mode 2                                    
    outb(FREQ_40HZ & BYTEMASK, CHANNEL_0);             // Sets the frequency to 40 HZ (108/55 * 1000000 / 40 = 29830)
    outb((FREQ_40HZ >> 8) & BYTEMASK, CHANNEL_0);   


    enable_irq(PIT_IRQ);
}


/* TODO
* get_curr_pcb_term
*   DESCRIPTION:    gets current terminal
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   terminal of current pcb    
*   SIDE EFFECTS:   none
*
*/
uint32_t get_curr_pcb_term() {
        pcb_t * curr_pcb = (pcb_t *)(K_MEM_END - (terminal_array[curr_terminal] + 1) * K_STACK_SIZE);
        return curr_pcb->terminal;
}

