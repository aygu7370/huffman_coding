 /****************************************************************************************
 * @file uart.h
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related to
 * 				UART.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/

#include <stdint.h>
#ifndef _TEST_CBFIFO_H_
#define _TEST_CBFIFO_H_


#define	BAUD_RATE		  															(115200)

/*****************************************************************************************/
/******************************** Functions Prototypes ***********************************/
/*****************************************************************************************/


/*****************************************************************************************
 * @name: uart0Init
 *
 * @param in
 * 		baudRate - baud rate to run the UART on.
 *
 * @description: UART0 initialization sequence.
 *****************************************************************************************/
void uart0Init(uint32_t baudRate);

/*****************************************************************************************
 * @name: __sys_write
 *
 * @param in
 * 		handle - baud rate to run the UART on.
 * 		buf - string to be sent on terminal.
 * 		size - number of characters to push.
 *
 * @description: UART0 write function.
 *****************************************************************************************/
int __sys_write(int handle, char *buf, int size);

/*****************************************************************************************
 * @name: __sys_readc
 *
 * @description: UART0 read function.
 *****************************************************************************************/
int __sys_readc(void);


/*****************************************************************************************
 * @name: userInputProcessing
 *
 * @description: API which deals with the cmds passed by user.
 *****************************************************************************************/
void userInputProcessing(void);


/*****************************************************************************************
 * @name: serialOut
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
void serialOut(void *buf, size_t count);


#endif
