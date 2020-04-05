#include "syscall_handler.h"

// File Descriptor struct. If you need more info, RTDC 8.2
typedef struct fd_t {
    // I got no clue about these types so if another type is more convenient go for it
    uint32_t fop_jump_table[8];
    int32_t inode;
    int32_t file_position;
    uint32_t flags;
} fd_t;

// Process Control Block struct. If you need more info, RTDC 6.3.5
typedef struct pcb_t {
    fd_t file_array[8];
    // There should be a bunch more shit stored in this struct but the documentation says "signal information"
    // and idk what that means.  Probably u wanna store like the parent pcb pointer and also like if it is parent
    // or child process and other shit idk.
} pcb_t;

/*
* halt
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
int32_t halt(unit8_t status) {
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
