/********************************************************************************
 * @file test_cbfifo.c
 ********************************************************************************
 *
 *
 * Date:        01-23-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 2. This file will have all the test functions
 *              required to test cbfifo in assignment 2 - Circular Buffer.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ********************************************************************************/

#include "fifo.h"
#include "cbfifo_utils.h"
#include "test_cbfifo.h"

/*************************************************************
 * @name stringcmp
 * 
 * This function returns a true if all the n characters in a 
 * string src matches with string des, otherwise it returns
 * false.
 * **********************************************************/
static bool stringcmp(uint8_t *src, uint8_t *des, int n){
    int i = 0;
    for(i = 0; i<n; i++){
        if(*(src+i) != *(des+i)){
            return false;
        }
    }
    return true;
}


/**************************************************************
 * @name test_cbfifo_enqueue
 * 
 * This function test the cbfifo for multiple enqueues.
 * ***********************************************************/
bool test_cbfifo_enqueue(cbfifo_t *testFifo){
    int x = 12321, y;
    int* nullptr = NULL;
    
    //get the length of empty circular buffer
    assert(cbfifo_length(testFifo) == 0);
    
    //added an integer x
    assert((cbfifo_enqueue(&x, sizeof(int), testFifo))== sizeof(int));
    assert(cbfifo_length(testFifo) == 4);

    assert((cbfifo_enqueue(nullptr, sizeof(int), testFifo))== -1);
    assert(cbfifo_length(testFifo) == 4);

    //for nbytes less than or equal to 0
    assert((cbfifo_enqueue(&x, 0, testFifo))== 0);
    assert(cbfifo_length(testFifo) == 4);

    //dequeue integer x
    assert(cbfifo_dequeue(&y, sizeof(int), testFifo) == sizeof(int));
    assert(cbfifo_length(testFifo) == 0);
    assert(y == x);
    
    return true;
}


/**************************************************************
 * @name test_llfifo_scenario_1
 *
 * This function test the cbfifo for multiple enqueues and
 * dequeues with different data types.
 * ***********************************************************/
bool test_cbfifo_scenario_1(cbfifo_t *testFifo){
    int x = 310;
    int y;
    char * str = "";
    char * str1 = "Principles of Embedded Software";
    char word_str_out[40];


    //get the length of empty circular buffer
    assert(cbfifo_length(testFifo) == 0);

    //added an integer x
    assert((cbfifo_enqueue(&x, sizeof(int), testFifo))== sizeof(int));
    assert(cbfifo_length(testFifo) == 4);

    //adding a string "Principles of Embedded Software" in fifo
    str1 = "Principles of Embedded Software";
    assert((cbfifo_enqueue(str1, 32*sizeof(char), testFifo)) == 32*sizeof(char));
    assert(cbfifo_length(testFifo) == 36);

    //adding an empty string
    assert((cbfifo_enqueue(str, sizeof(char), testFifo)) == sizeof(char));
    assert(cbfifo_length(testFifo) == 37);

    //dequeue integer x
    assert(cbfifo_dequeue(&y, sizeof(int), testFifo) == sizeof(int));
    assert(cbfifo_length(testFifo) == 33);
    assert(y == x);

    //dequeue "Principles of Embedded Software" from fifo and compare
    assert(cbfifo_dequeue(word_str_out, 32*sizeof(char), testFifo) == 32*sizeof(char));
    assert(cbfifo_length(testFifo) == 1);
    assert(strcmp(str1, word_str_out) == 0);

    assert(cbfifo_dequeue(word_str_out, sizeof(char), testFifo) == sizeof(char));
    assert(cbfifo_length(testFifo) == 0);
    assert(strcmp(word_str_out, str) == 0);
    return true;
}


/**************************************************************
 * @name test_llfifo_scenario_2
 *
 * This function test the cbfifo for multiple enqueues and
 * dequeues with integers and check for length and capacity
 * of FIFO.
 * ***********************************************************/
bool test_cbfifo_length_capacity(cbfifo_t *testFifo){
    //circular buffer is empty
    int x = 0, y;
    char ch = 'A', ch_out;

    //get the length of empty circular buffer
    assert(cbfifo_length(testFifo) == 0);

    //enqueue an interger and then a character
    assert((cbfifo_enqueue(&x, sizeof(int), testFifo))== sizeof(int));
    assert(cbfifo_length(testFifo) == sizeof(int));
    assert((cbfifo_enqueue(&ch, sizeof(char), testFifo))== sizeof(char));
    assert(cbfifo_length(testFifo) == (sizeof(int) + sizeof(char)));

    //dequeue interger and then character
    assert(cbfifo_dequeue(&y, sizeof(int), testFifo) == sizeof(int));
    assert(x == y);
    assert(cbfifo_length(testFifo) == sizeof(char));
    assert(cbfifo_dequeue(&ch_out, sizeof(char), testFifo) == sizeof(char));
    assert(ch == ch_out);
    assert(cbfifo_length(testFifo) == 0);
    return true;
}


/**************************************************************
 * @name test_llfifo_scenario_3
 *
 * This function test the cbfifo for multiple enqueues and
 * dequeues.
 * ***********************************************************/
bool test_cbfifo_dequeue(cbfifo_t *testFifo){
    char * buf = "An embedded system is a microprocessor-based computer hardware system with software that is designed to perform a dedicated function, either as an independent system or as a part of a large system. At the core is an integrated circuit designed to carry out computation for real-time operations.\nComplexities range from a single microcontroller to a suite of processors with connected peripherals and networks; from no user interface to complex graphical user interfaces. The complexity of an embedded system varies significantly depending on the task for which it is designed.\nEmbedded system applications range from digital watches and microwaves to hybrid vehicles and avionics. As much as 98 percent of all microprocessors manufactured are used in embedded systems.";
    int windex = 0;
    int rindex = 0;
    char out_str[130];

    //get the length of empty circular buffer
    assert(cbfifo_length(testFifo) == 0);

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));


    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t*)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));


    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 20, testFifo) == 20);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 20) == true);
    rindex += 20;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr < rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //check length of cbfifo
    assert(cbfifo_length(testFifo) == 110);

    //enqueue 20 more bytes, only 18 should be enqueued and 2 bytes will be ignored
    assert((cbfifo_enqueue(buf+ windex, 18, testFifo))== 18);
    windex += 18;
    assert(cbfifo_length(testFifo) == (windex-rindex));
    assert(cbfifo_length(testFifo) == 128);

    //dequeue the whole buffer
    assert(cbfifo_dequeue(out_str, 128, testFifo) == 128);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t*)out_str, 128) == true);
    rindex += 128;
    assert(cbfifo_length(testFifo) == (windex-rindex));
    assert(cbfifo_length(testFifo) == 0);

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));


    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t*)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));


    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 20, testFifo) == 20);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 20) == true);
    rindex += 20;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr < rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //check length of cbfifo
    assert(cbfifo_length(testFifo) == 110);

    //enqueue 20 more bytes, only 18 should be enqueued and 2 bytes will be ignored
    assert((cbfifo_enqueue(buf+ windex, 18, testFifo))== 18);
    windex += 18;
    assert(cbfifo_length(testFifo) == (windex-rindex));
    assert(cbfifo_length(testFifo) == 128);

    //dequeue the whole buffer
    assert(cbfifo_dequeue(out_str, 128, testFifo) == 128);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 128) == true);
    rindex += 128;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    windex = 0;
    rindex = 0;

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t*)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));


    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 10, testFifo) == 10);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 10) == true);
    rindex += 10;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr > rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //dequeue interger and then character
    assert(cbfifo_dequeue(out_str, 20, testFifo) == 20);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t *)out_str, 20) == true);
    rindex += 20;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //enqueue an 50 character from the string buf (wptr < rptr)
    assert((cbfifo_enqueue(buf+ windex, 50, testFifo))== 50);
    windex += 50;
    assert(cbfifo_length(testFifo) == (windex-rindex));

    //check length of cbfifo
    assert(cbfifo_length(testFifo) == 110);

    //enqueue 20 more bytes, only 18 should be enqueued and 2 bytes will be ignored
    assert((cbfifo_enqueue(buf+ windex, 18, testFifo))== 18);
    windex += 18;
    assert(cbfifo_length(testFifo) == (windex-rindex));
    assert(cbfifo_length(testFifo) == 128);

    //dequeue the whole buffer
    assert(cbfifo_dequeue(out_str, 128, testFifo) == 128);
    assert(stringcmp((uint8_t *)(buf+rindex), (uint8_t*)out_str, 128) == true);
    rindex += 128;
    assert(cbfifo_length(testFifo) == (windex-rindex));
    assert(cbfifo_length(testFifo) == 0);
    return true;
}

/**************************************************************
 * @name test_FIFO
 *
 * This function test the cbfifo for multiple scenarios.
 * ***********************************************************/
void testFIFO(){

	cbfifo_t testFifo;
	//initialize fifo instance
	fifoInit(&testFifo);

	test_cbfifo_enqueue(&testFifo);
	test_cbfifo_scenario_1(&testFifo);
	test_cbfifo_dequeue(&testFifo);
	test_cbfifo_length_capacity(&testFifo);
}
