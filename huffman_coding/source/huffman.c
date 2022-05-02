 /****************************************************************************************
 * @file huffman.c
 *****************************************************************************************
 * Date:        04-27-2022
 * Author:      Ayush Gupta
 * Description: PES - Final Project. This file handles all the operations related to
 * 				huffman.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/


#include "huffman.h"
#include <string.h>

#define MIN(X, Y) 												(((X) < (Y)) ? (X) : (Y))
#define MASK(X)													((1<<X) - 1)
#define HUFFMAN_END_CODE_SYMBOL                                 '\0'

#define BITS_IN_A_BYTE											(8)

/**************************************************************************
 * @name huffmanCode
 *
 * @description huffman characters, encoded bits and length of encoded bits.
 *************************************************************************/
encode_huffmanCode_t encodeHuffmanCode[] = {
		{' ' , 0b00         ,2 },
		{'0' , 0b10         ,2 },
		{'\r', 0b11010      ,5 },
		{'\n', 0b11011      ,5 },
		{'i' , 0b01011      ,5 },
		{'p' , 0b010000     ,6 },
		{'r' , 0b010001     ,6 },
		{'E' , 0b010100     ,6 },
		{'1' , 0b011100     ,6 },
		{'m' , 0b011110     ,6 },
		{'a' , 0b011111     ,6 },
		{'t' , 0b110000     ,6 },
		{'o' , 0b110001     ,6 },
		{'e' , 0b110010     ,6 },
		{'d' , 0b111111     ,6 },
		{'n' , 0b111101     ,6 },
		{'>' , 0b1110100    ,7 },
		{'l' , 0b1110101    ,7 },
		{'I' , 0b1110110    ,7 },
		{'u' , 0b1110111    ,7 },
		{'4' , 0b1110011    ,7 },
		{'C' , 0b1110001    ,7 },
		{'_' , 0b1111001    ,7 },
		{'9' , 0b0110000    ,7 },
		{'D' , 0b0110001    ,7 },
		{'2' , 0b0101010    ,7 },
		{'A' , 0b0101011    ,7 },
		{'T' , 0b0110011    ,7 },
		{'f' , 0b0110100    ,7 },
		{'s' , 0b0110101    ,7 },
		{'M' , 0b0110111    ,7 },
		{'3' , 0b0111011    ,7 },
		{':' , 0b11111011   ,8 },
		{'h' , 0b11111000   ,8 },
		{'8' , 0b11111001   ,8 },
		{'N' , 0b11100000   ,8 },
		{'R' , 0b11100001   ,8 },
		{'B' , 0b11100100   ,8 },
		{'v' , 0b11100101   ,8 },
		{'c' , 0b11110000   ,8 },
		{'H' , 0b01001001   ,8 },
		{'G' , 0b01001100   ,8 },
		{'y' , 0b01001101   ,8 },
		{'L' , 0b01001110   ,8 },
		{'x' , 0b01001111   ,8 },
		{'w' , 0b01100100   ,8 },
		{'U' , 0b01100101   ,8 },
		{'S' , 0b01101100   ,8 },
		{'b' , 0b01101101   ,8 },
		{'5' , 0b01110100   ,8 },
		{'z' , 0b11001100   ,8 },
		{'P' , 0b11001101   ,8 },
		{'O' , 0b11001111   ,8 },
		{':' , 0b11111011   ,8 },
		{'h' , 0b11111000   ,8 },
		{'8' , 0b11111001   ,8 },
		{'N' , 0b11100000   ,8 },
		{'R' , 0b11100001   ,8 },
		{'B' , 0b11100100   ,8 },
		{'v' , 0b11100101   ,8 },
		{'c' , 0b11110000   ,8 },
		{'Z' , 0b010010000  ,9 },
		{'Y' , 0b010010001  ,9 },
		{'Q' , 0b010010100  ,9 },
		{'K' , 0b010010101  ,9 },
		{'X' , 0b010010110  ,9 },
		{'W' , 0b010010111  ,9 },
		{'7' , 0b011101010  ,9 },
		{'k' , 0b011101011  ,9 },
		{'V' , 0b110011101  ,9 },
		{'j' , 0b111100010  ,9 },
		{'F' , 0b111100011  ,9 },
		{'g' , 0b1111101000 ,10},
		{'6' , 0b1111101001 ,10},
		{'J' , 0b1111101010 ,10},
		{'q' , 0b1111101011 ,10},
		{'-' , 0b1100111000 ,10},
		{'.' , 0b11001110010,11},
		{HUFFMAN_END_CODE_SYMBOL , 0b11001110011,11}
};

/**************************************************************************
 * @name encodeHuffman
 *
 * @description Refer huffman.h for more information.
 *************************************************************************/
int encodeHuffman(const char* msg, uint8_t* buffer, size_t nBytes){
	int bufIndex = 0;			//current write position into encoded data
	int bitsWritten = 0;		//number of bits already written at bufIndex
	int hcIndex;			//huffman code for the current symbol

	//zeroing out the whole buffer
	memset(buffer, 0, nBytes);
    uint32_t temp = 0;
	for(const char* p = msg; *p != '\0'; p++){
		//find the huffman code for this symbol
		for(hcIndex = 0; encodeHuffmanCode[hcIndex].symbol != *p; hcIndex++);

		//read data from the matched code in HUFFMAN Table
		uint32_t code = encodeHuffmanCode[hcIndex].code;
		int nBits = encodeHuffmanCode[hcIndex].nBits;
		while(nBits > 0){
			//number of bits to be written in this iteration
			int bitsToBeWritten = MIN(BITS_IN_A_BYTE - bitsWritten, nBits);

			//position within code to get the bits from code
			int readShift = nBits - bitsToBeWritten;
			//bits to write, shifted left by readShift
			temp = (code >> readShift) & MASK(bitsToBeWritten);
			//shift to fit in the buffer
			int writeShift = BITS_IN_A_BYTE - bitsWritten - bitsToBeWritten;
			//write the encoded byte to the buffer
			buffer[bufIndex] |= (temp << writeShift);
			//update bitsWritten by number of encoded bits
			bitsWritten += bitsToBeWritten;
			//update nBits by number of encoded bits
			nBits -= bitsToBeWritten;
			//if whole byte is read, write to next byte
			if(bitsWritten == BITS_IN_A_BYTE){
				bitsWritten = 0;
				bufIndex++;
			}
        }
	}

    for(hcIndex = 0; encodeHuffmanCode[hcIndex].symbol != HUFFMAN_END_CODE_SYMBOL; hcIndex++);
    int code = encodeHuffmanCode[hcIndex].code;
    int nBits = encodeHuffmanCode[hcIndex].nBits;
    while(nBits > 0){
        //number of bits to be written in this iteration
        int bitsToBeWritten = MIN(BITS_IN_A_BYTE - bitsWritten, nBits);

        //position within code to get the bits from code
        int readShift = nBits - bitsToBeWritten;
        //bits to write, shifted left by readShift
        temp = (code >> readShift) & MASK(bitsToBeWritten);
        //shift to fit in the buffer
        int writeShift = BITS_IN_A_BYTE - bitsWritten - bitsToBeWritten;
        //write the encoded byte to the buffer
        buffer[bufIndex] |= (temp << writeShift);

        //update bitsWritten by number of encoded bits
        bitsWritten += bitsToBeWritten;
        //update nBits by number of encoded bits
        nBits -= bitsToBeWritten;

        //if whole byte is read, write to next byte
        if(bitsWritten == BITS_IN_A_BYTE){
            bitsWritten = 0;
            bufIndex++;
        }
    }

    if(bitsWritten != 0){
        bufIndex++;
    }
	return bufIndex;
}
