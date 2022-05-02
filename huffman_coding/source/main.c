/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include <stdio.h>
#include "sysclock.h"
#include "uart.h"
#include "test_cbfifo.h"



int main(void)
{
	//clock initialization given

	sysclock_init();

	//uart initialization call
	uart0Init(BAUD_RATE);

#ifdef DEBUG
	//test the FIFO implementation
	test_FIFO();
#endif


	printf("ECEN5813 Principles of Embedded Software Final Project\r\n");
	// enter infinite loop
  while (1) {
	  printf("> ");
	  userInputProcessing();
  }

  return 0 ;
}
