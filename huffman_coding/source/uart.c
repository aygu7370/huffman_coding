 /****************************************************************************************
 * @file uart.c
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related to
 * 				UART.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 *****************************************************************************************/

#include "MKL25Z4.h"


#include <stdbool.h>
#include <string.h>

#include "uart.h"
#include "fifo.h"
#include "cli.h"
#include "huffman.h"
/*****************************************************************************************
 *                                 UART Configuration									 *
 *****************************************************************************************/
#define UART_OVERSAMPLE_RATE														(15)
#define SYS_CLOCK																	(24e6)
#define BACKSPACE_ASCII_VALUE														(8)

#define DATA_SIZE																	(8)
#define PARITY																		(None)
#define	STOP_BITS																	(1)

#if (DATA_SIZE == 8)
#define BIT_MODE																	(0)
#else
#define BIT_MODE																	(1)
#endif

#if (PARITY == None)
#define PARITY_ENABLE																(0)
#else
#define PARITY_ENABLE																(1)
#endif

#if (STOP_BITS == 1)
#define STOP_CONFIG																	(0)
#elif (STOP_BITS == 2)
#define STOP_CONFIG																	(1)
#endif

#define TEMP_BUFFER_SIZE															(256)
//initialize RX, Tx buffer with the default parameters
cbfifo_t rxBuffer, txBuffer;


/*****************************************************************************************
 * @name: uart0Init
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
void uart0Init(uint32_t baudRate){
	uint16_t sbr;

	//Enable clock gating for UART00 and PORTA
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Disabling transmitter and receiver before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set UART clock to 24MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	//Set pins to UART0 Tx and Rx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);			//Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);			//Tx

	//Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baudRate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_CONFIG) | UART_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(BIT_MODE) | UART0_C1_PE(PARITY_ENABLE) | UART0_C1_PT(0);
	//Don't invert transit data, do enable interrupt for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0) | UART0_C3_NEIE(0)
				| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	//Configure to send LSB first
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	//Initializing Queue for further use
	fifoInit(&txBuffer);
	fifoInit(&rxBuffer);

	//Enable UART0 interrupts
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//Enable receiver interrupts
	UART0->C2 |= UART_C2_RIE(1);

	//Enable UART transmitter and receiver
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
}


/*****************************************************************************************
 * @name: __sys_write
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
int __sys_write(int handle, char *buf, int size){
	if(buf == NULL){
		//returns an error when buf is pointing to NULL
		return size;
	}

	uint8_t data[256];

	int nBytesStored = 0;
	//wait if the TX Buffer is full.
	while(txBuffer.bufferFullFlag){};


	// encode data using HUFFMAN coding algorithm
	int bytesToStore = encodeHuffman(buf, data, size);
	//store data in TX Buffer
	nBytesStored = cbfifo_enqueue(data, bytesToStore, &txBuffer);


	if(!(UART0->C2 & UART0_C2_TIE_MASK)){
		UART0->C2 |= UART0_C2_TIE(1);
	}
	if(nBytesStored < bytesToStore){
		return -1;
	}
	return 0;
}

/*****************************************************************************************
 * @name: __sys_readc
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
int __sys_readc(void){
	char ch;
	//check if RX buffer is empty
	if (QIsEmpty(&rxBuffer) == 0){
		cbfifo_dequeue(&ch, 1, &rxBuffer);
		return ch;
	}
	return -1;
}

/*****************************************************************************************
 * @name: serialOut
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
void serialOut(void *buf, size_t count){
	cbfifo_enqueue(buf, count, &txBuffer);
	//Enable TIE flag, start transmitting passed string
	if(!(UART0->C2 & UART0_C2_TIE_MASK)){
		UART0->C2 |= UART0_C2_TIE(1);
	}
}

/*****************************************************************************************
 * @name: serialIn
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
size_t serialIn(void *buf, size_t count){
	return cbfifo_dequeue(buf, count, &rxBuffer);
}


/******************************************************************************************
 * @name putstr
 *
 * @param in
 *  s - string to be sent on serial terminal.
 *
 * @description Function displays the string on serial terminal and returns the number of
 *              characters sent.
 * ***************************************************************************************/
int putstr(char* s)
{
    int i = 0;
    while (*s) {            // output characters until NULL found
        putchar(*s++);
        i++;
    }
    return i + 1;
}


/******************************************************************************************
 * @name getUserInput
 *
 * @param in
 *  inputString - character buffer to store user input.
 *
 * @description gets the user input from serial terminal and store it in the passed string.
 * ***************************************************************************************/
void getUserInput(char *inputString){
    char ch;
    int i;
    i = 0;
    while(1){
        ch = getchar();
        putchar(ch);
        if (ch == '\r')
            break;
        if (ch != 8){
            *(inputString + i) = ch;
            i++;
        }
    }
    putchar('\r');
    putchar('\n');

}


/*****************************************************************************************
 * @name: userInputProcessing
 *
 * @description: Refer uart.h for more detail.
 *****************************************************************************************/
void userInputProcessing(void){
	char userInput[TEMP_BUFFER_SIZE];
	char ch = '@';
	int i = 0;
	while(1){
		//wait for incoming character
		while(QIsEmpty(&rxBuffer)){};
		cbfifo_dequeue(&ch, 1, &rxBuffer);
		if(!(UART0->C2 & UART0_C2_TIE_MASK)){
			UART0->C2 |= UART0_C2_TIE(1);
		}
		if((ch == '\r') || (ch == '\n')){
			break;
		}

		//handling backspace character
		if (ch != BACKSPACE_ASCII_VALUE){
			userInput[i] = ch;
			i++;
		}
		else{
			if(i != 0){
				i--;
			}
		}
	}
	userInput[i] = '\0';
	processCommand(userInput);
	memset(userInput, 0, TEMP_BUFFER_SIZE);
	i = 0;
	return;
}



/*****************************************************************************************
 * @name: UART0_IRQHandler
 *
 * @description: UART0 IRQ Handler.
 *****************************************************************************************/
void UART0_IRQHandler(void) {
	uint8_t ch;
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		ch = UART0->D;
		if (!(rxBuffer.bufferFullFlag)) {
			//if receive buffer is not full
			cbfifo_enqueue(&ch, 1, &rxBuffer);
			if(!(UART0->C2 & UART0_C2_TIE_MASK)){
				UART0->C2 |= UART0_C2_TIE(1);
			}
		}
	}

	if((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->C2 & UART0_S1_TDRE_MASK)){
		//transmit the character back on the terminal, if txBuffer is not empty
		if(cbfifo_dequeue(&ch, 1, &txBuffer) == 1){
			UART0->D = ch;
			if(!(UART0->C2 & UART0_C2_TIE_MASK)){
				UART0->C2 |= UART0_C2_TIE(1);
			}
		}
		else{
			//TX buffer is empty, disable TX interrupts
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

