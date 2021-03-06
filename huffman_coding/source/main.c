/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include <stdio.h>
#include "sysclock.h"
#include "uart.h"
#include "test_cbfifo.h"
#include "test_huffman.h"



int main(void)
{
	//clock initialization given
	sysclock_init();

	//uart initialization call
	uart0Init(BAUD_RATE);

#ifdef DEBUG
	//test the Huffman implementation
	testHuffman();
	//test the FIFO implementation
	testFIFO();
#endif

	printf("ECEN5813 Principles of Embedded Software Final Project\r\n");
	// enter infinite loop
  while (1) {
	  userInputProcessing();
  }

  return 0 ;
}
