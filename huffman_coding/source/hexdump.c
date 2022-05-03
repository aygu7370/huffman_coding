 /****************************************************************************************
 * @file hexdump.c
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related hex dump.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/
#include "hexdump.h"
#include <string.h>
#include <stdio.h>

#define BYTES_ON_A_SINGLE_LINE														(16)
#define MAX_HEXDEMP_LIMIT_IN_BYTES													(640)
#define HEX_DUMP_LENGTH																(496)



/******************************************************************************************
 * @name getNibble
 *
 * @param in
 *  nibble - nibble to be displayed on Serial terminal.
 *
 * @description Function returns the nibble in hex.
 * ***************************************************************************************/
static char getNibble(uint8_t nibble){
    char ch;
    if ((nibble <= 9)){
        ch = (nibble + '0');
    }
    else{
        ch = (nibble - 10 + 'A');
    }
    return ch;
}

char hexString[HEX_DUMP_LENGTH];
/******************************************************************************************
 * @name displayHexDump
 *
 * @param in
 *  buf - buffer to be displayed on serial terminal.
 *  nBytes - number of bytes requested.
 *
 * @description Refer hexdump.h for more details.
 * ***************************************************************************************/
void displayHexDump(const void *buf, size_t nBytes){
	//clear the buffer

	memset(hexString, 0, HEX_DUMP_LENGTH);
	int hexIndex = 0;
	if(nBytes > MAX_HEXDEMP_LIMIT_IN_BYTES){
		//requested memory is outside the limit; print till MAX_HEXDEMP_LIMIT_IN_BYTES and ignore the rest for now.
		nBytes = MAX_HEXDEMP_LIMIT_IN_BYTES;
	}

	int charsOnALine = 0;
	uint8_t *startingLoc = (uint8_t *)buf;
	uint8_t *endLoc = (uint8_t *)buf + nBytes;


	while(startingLoc < endLoc){
		//break if hexdump limit reached
		if(hexIndex >= HEX_DUMP_LENGTH){
			break;
		}
		if((charsOnALine == BYTES_ON_A_SINGLE_LINE) || (charsOnALine == 0)){
			//move cursor to the next line
			hexString[hexIndex++] = '\r';
			hexString[hexIndex++] = '\n';

			//print starting location of the new line in the format 0000_0000
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0xF0000000) >> 28);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x0F000000) >> 24);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x00F00000) >> 20);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x000F0000) >> 16);
			hexString[hexIndex++] = '_';
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x0000F000) >> 12);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x00000F00) >> 8);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x000000F0) >> 4);
			hexString[hexIndex++] = getNibble(((uint32_t)(startingLoc) & 0x0000000F));

			//2 spaces in between address and data
			hexString[hexIndex++] = ' ';
			hexString[hexIndex++] = ' ';
			charsOnALine = 0;

		}

		hexString[hexIndex++] = getNibble((((uint8_t)(*startingLoc) & 0xF0) >> 4));
		hexString[hexIndex++] = getNibble(((uint8_t)(*startingLoc) & 0x0F));
		hexString[hexIndex++] = ' ';

		startingLoc++;
		charsOnALine++;
	}
	//move cursor to the next line
	hexString[hexIndex++] = '\r';
	hexString[hexIndex++] = '\n';
	//string end character
	hexString[hexIndex++] = '\0';

	//print the remaining bytes
	printf("%s", hexString);
	hexIndex = 0;


}

