#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "paging.h"
#include "keyboard.h"

#define PASS 1
#define FAIL 0

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
 * Files: idt.c
 */
void nullptr_test(){
	TEST_HEADER;
	printf("vro this test finna page fault: ");
	int* nullptr = NULL;
	int rip = *nullptr;
}

/* Page doesnt exist test
 * 
 * Attempts to dereference page that isn't in range
 * Inputs: None
 * Outputs: None
 * Side Effects: Should result in page fault
 * Coverage: Exception Handler, Paging
 * Files: idt.c, paging.c
 */
void page_doesnt_exist_test(){
	TEST_HEADER;
	printf("vro this test finna page fault: ");
	int* pageptr = (int*)(0x800069);
	int rip = *(pageptr);
}

/* Page does exist test
 * 
 * Attempts to dereference pages in both video memory 
 * and kernel memory
 * Inputs: None
 * Outputs: None
 * Side Effects: Should not result in page fault
 * Coverage: Exception Handler, Paging
 * Files: idt.c, paging.c
 */
void page_does_exist_test(){
	TEST_HEADER;

	int * pageptr = (int*)(0xB8069);
	int rip = *pageptr;
	printf("Video memory test hella passed vro, ");

	pageptr = (int*)(0x400069);
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
	change_rate(4);
}


/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	// nullptr_test();
	// page_doesnt_exist_test();
	// page_does_exist_test();
	// rtc_test();
}
