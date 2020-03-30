#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "paging.h"
#include "keyboard.h"
#include "file_system.h"

#define PASS 1
#define FAIL 0

#define KEYBOARD_TEST_BUF 	5

/* format these macros as you see fit */

#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}


// add more tests here

/* Null ptr test
 * 
 * Attempts to dereference null pointer
 * Inputs: None
 * Outputs: None
 * Side Effects: Should result in page fault
 * Coverage: Exception Handler
 * Files: idt.c, ex_handler.c
 */
void nullptr_test(){
	TEST_HEADER;
	printf("vro this test finna page fault: ");
	int* nullptr = NULL;
	int rip = *nullptr;
	rip = 0;
}

/* div_test
 *
 * Test divide by zero exception
 * Inputs: None
 * Outputs: None
 * Side Effects: should cause divide by zero exception.
 * Coverage: Exception handler.
 * Files: idt.c, ex_handler.c
 */
void div_test(){
	TEST_HEADER;
	printf("vro this test finna div error: ");
	int a = 1;
	int b = 0;
	a = a/b;
}



/* Page doesnt exist test
 * 
 * Attempts to dereference page that isn't in range
 * Inputs: None
 * Outputs: None
 * Side Effects: Should result in page fault
 * Coverage: Exception Handler, Paging
 * Files: idt.c, ex_handler.c, paging.c
 */
void page_doesnt_exist_test_end(){
	TEST_HEADER;
	printf("vro this test finna page fault: ");
	int* pageptr = (int*)(0x800069);
	int rip = *(pageptr);
	rip = 0;
}

void page_doesnt_exist_test_start(){
	TEST_HEADER;
	printf("vro this test finna page fault: ");
	int* pageptr = (int*)(0xB7FF4);
	int rip = *(pageptr);
	rip = 0;
}


/* Page does exist test
 * 
 * Attempts to dereference pages in both video memory 
 * and kernel memory
 * Inputs: None
 * Outputs: None
 * Side Effects: Should not result in page fault
 * Coverage: Exception Handler, Paging
 * Files: idt.c, ex_handler.c, paging.c
 */
void page_does_exist_test(){
	TEST_HEADER;

	int * pageptr = (int*)(0xB8069);
	int rip = *pageptr;
	printf("Video memory test hella passed vro, ");

	pageptr = (int*)(0x400069);
	rip = *pageptr;
	printf("Kernel memory hella passed too vro \n");
}


/* rtc_test
 * 
 * WIP
 * Inputs: None
 * Outputs: None
 * Side Effects: 
 * Coverage: RTC
 * Files: rtc.c
 */
void rtc_test(){
	
	// change_rate(20);
	
	// send_eoi(RTC_IRQ);

	
	/* in order to leave RTC in usable (able to send more interrupts) state 
      after handling the current interrupt, must read register C */
    outb(REGISTERC, RTCPORT);
    inb(RTCDATA);

	send_eoi(RTC_IRQ);

}


/* Sys call test
 * 
 * Tests is syscalls are accounted for
 * Inputs: None
 * Outputs: None
 * Side Effects: Should print out syscall handler
 * Coverage: Exception Handler
 * Files: idt.c, ex_handler.c
 */
void syscall_test() {
	TEST_HEADER;
	asm volatile ("\
		movl $69, %eax\n\
		int $0x80\n\
	");
}



/* Checkpoint 2 tests */

/* terminal_keyboard_test
 * 
 * Inputs: None
 * Outputs: None
 * Side Effects: Tests for typing and reading typed info by parsing typed data
 * Coverage: KEYBOARD
 * Files: keyboard.c
 */
void terminal_keyboard_test() {
	uint8_t buf[KEYBOARD_TEST_BUF];
	printf("Hella finna terminal read/write testing vro (5 char buffer)\n");
		while(1)
		{
			terminal_read(buf, KEYBOARD_TEST_BUF);
			terminal_write(buf, KEYBOARD_TEST_BUF);
		}
}

/* rtc_write_test
 * 
 * WIP
 * Inputs: None
 * Outputs: None
 * Side Effects: Opens RTC, checks to see if frequency is changing properly using
 * 				 write and change_rate functions
 * Coverage: RTC
 * Files: rtc.c
 */
void rtc_write_test() {
	TEST_HEADER;
	uint32_t * testRate;
	*testRate = DEFAULT_FREQ;
	uint32_t i;

	initialize_rtc();
	rtc_open(0);

	while (*testRate <= FREQ_UPPER_BOUND) {
		rtc_write(0, testRate);

		// display "test" 25 times at each possibly frequency
		for (i = 0; i < 25; i++) 
			rtc_read(0);
		printf ("SPEEDITUP");
		
		*testRate *= POWER_OF_TWO;

	}


	/* in order to leave RTC in usable (able to send more interrupts) state 
      after handling the current interrupt, must read register C */
    outb(REGISTERC, RTCPORT);
    inb(RTCDATA);
}

int read_file_test(const int8_t* fname) {
	TEST_HEADER;
    dentry_t file1;
	dentry_t file2;
    if(-1 == read_dentry_by_name(fname, &file1)) return FAIL;
	if(-1 == read_dentry_by_index(file1.inode_num, &file2)) return FAIL;
	if(file1.file_name != file2.file_name || file1.file_type != file2.file_type || file1.inode_num != file2.inode_num) return FAIL;
	return PASS;
}

int print_file_test(const int8_t* fname){
	TEST_HEADER;

	dentry_t file;

	if(-1 == read_dentry_by_name(fname, &file)) return FAIL;

	clear	();

	print_file(&file);

	return PASS;
}

int test_dir_open(){
	uint32_t  offset = 0;
	int32_t fd;
	uint8_t buf[32];
	int i;
	int num_bytes;
	while((num_bytes = dir_read (&fd , buf, 32, &offset))){
		for(i = 0; i<num_bytes; i++){
			putc(buf[i]);
		}
		printf("\n");
	}
	return PASS;
}

int open_file(const int8_t* fname){
	int32_t fd;
	file_open((uint8_t*)fname,&fd);
	printf("%d",fd);
	file_close(&fd);
	if(fd==0){
		return PASS;
	}
	return FAIL;
}

int open_dir(const int8_t* fname){
	int32_t fd;
	if ( 0 == dir_open((uint8_t*)fname,&fd)) return PASS;
	else return FAIL;
}
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	// div_test();
	// nullptr_test();
	// page_doesnt_exist_test_start();
	// page_doesnt_exist_test_end();
	// page_does_exist_test();
	// rtc_test();
	// syscall_test();
	// rtc_write_test();
	// TEST_OUTPUT("test_dir_open", test_dir_open());
	// TEST_OUTPUT("print_file_test", print_file_test("frame1.txt"));
	// terminal_keyboard_test();
	//TEST_OUTPUT("open_file",open_file("frame1.txt"));
	TEST_OUTPUT("open_dir",open_dir("frame1.txt"));
}
