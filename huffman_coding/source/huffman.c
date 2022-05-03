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
#include <stdbool.h>

#define MIN(X, Y) 												(((X) < (Y)) ? (X) : (Y))
#define MASK(X)													((1<<X) - 1)

#define INVALID_HUFFMAN_SYMBOL                                  (0xFFFFFFFF)
#define BITS_IN_A_BYTE											(8)

//Huffman table: symbol, encoded messages and length of encoded messages
typedef struct {
	uint8_t symbol;
	uint32_t code;
	int nBits;
} encode_huffmanCode_t;

typedef struct {
	uint8_t symbol;
	char code[20];
	int nBits;
} decode_huffmanCode_t;

/**************************************************************************
 * @name encodeHuffmanCode
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
 * @name decodeHuffmanCode
 *
 * @description huffman characters, encoded bits and length of encoded bits.
 *************************************************************************/
static decode_huffmanCode_t decodeHuffmanCode[] = {
	{' ' , "00"         ,2 },
    {'0' , "10"         ,2 },
    {'\r', "11010"      ,5 },
    {'\n', "11011"      ,5 },
    {'i' , "01011"      ,5 },
    {'p' , "010000"     ,6 },
    {'r' , "010001"     ,6 },
    {'E' , "010100"     ,6 },
    {'1' , "011100"     ,6 },
    {'m' , "011110"     ,6 },
    {'a' , "011111"     ,6 },
    {'t' , "110000"     ,6 },
    {'o' , "110001"     ,6 },
    {'e' , "110010"     ,6 },
    {'d' , "111111"     ,6 },
    {'n' , "111101"     ,6 },
    {'>' , "1110100"    ,7 },
    {'l' , "1110101"    ,7 },
    {'I' , "1110110"    ,7 },
    {'u' , "1110111"    ,7 },
    {'4' , "1110011"    ,7 },
    {'C' , "1110001"    ,7 },
    {'_' , "1111001"    ,7 },
    {'9' , "0110000"    ,7 },
    {'D' , "0110001"    ,7 },
    {'2' , "0101010"    ,7 },
    {'A' , "0101011"    ,7 },
    {'T' , "0110011"    ,7 },
    {'f' , "0110100"    ,7 },
    {'s' , "0110101"    ,7 },
    {'M' , "0110111"    ,7 },
    {'3' , "0111011"    ,7 },
    {':' , "11111011"   ,8 },
    {'h' , "11111000"   ,8 },
    {'8' , "11111001"   ,8 },
    {'N' , "11100000"   ,8 },
    {'R' , "11100001"   ,8 },
    {'B' , "11100100"   ,8 },
    {'v' , "11100101"   ,8 },
    {'c' , "11110000"   ,8 },
    {'H' , "01001001"   ,8 },
    {'G' , "01001100"   ,8 },
    {'y' , "01001101"   ,8 },
    {'L' , "01001110"   ,8 },
    {'x' , "01001111"   ,8 },
    {'w' , "01100100"   ,8 },
    {'U' , "01100101"   ,8 },
    {'S' , "01101100"   ,8 },
    {'b' , "01101101"   ,8 },
    {'5' , "01110100"   ,8 },
    {'z' , "11001100"   ,8 },
    {'P' , "11001101"   ,8 },
    {'O' , "11001111"   ,8 },
    {':' , "11111011"   ,8 },
    {'h' , "11111000"   ,8 },
    {'N' , "11100000"   ,8 },
    {'R' , "11100001"   ,8 },
    {'B' , "11100100"   ,8 },
    {'v' , "11100101"   ,8 },
    {'c' , "11110000"   ,8 },
    {'Z' , "010010000"  ,9 },
    {'Y' , "010010001"  ,9 },
    {'Q' , "010010100"  ,9 },
    {'K' , "010010101"  ,9 },
    {'X' , "010010110"  ,9 },
    {'W' , "010010111"  ,9 },
    {'7' , "011101010"  ,9 },
    {'k' , "011101011"  ,9 },
    {'V' , "110011101"  ,9 },
    {'j' , "111100010"  ,9 },
    {'F' , "111100011"  ,9 },
    {'g' , "1111101000" ,10},
    {'6' , "1111101001" ,10},
    {'J' , "1111101010" ,10},
    {'q' , "1111101011" ,10},
    {'-' , "1100111000" ,10},
    {'.' , "11001110010",11},
    {HUFFMAN_END_CODE_SYMBOL, "11001110011",11}
};

/**************************************************************************
 * @name intToStr
 *
 * @description Refer read_serial.h for more details.
 *************************************************************************/
static void intToStr(char * str, uint32_t code, size_t len){
    int index = 0;
    //store the least significant bit into str
    while(index < len){
        str[index] = ((code >> (len - index - 1)) & 1) + '0';
        index++;
    }
    str[index] = '\0';
}

/**************************************************************************
 * @name getHuffmanSymbol
 *
 * @description Refer read_serial.h for more details.
 *************************************************************************/
static uint32_t getHuffmanSymbol(uint32_t code, size_t len){
    char temp[20];
    //convert integer to bit string
    intToStr(temp, code, len);
    //iterate over huffman table to find if the code is available or not
    for(int i = 0; i < sizeof(decodeHuffmanCode)/sizeof(decode_huffmanCode_t); i++){
        //if len is equal to the length of number of bits in huffman encoded table
        if(len == decodeHuffmanCode[i].nBits){
            //if code matches with huffman encoded code return symbol
            if (strncmp(temp, decodeHuffmanCode[i].code, decodeHuffmanCode[i].nBits) == 0){
                return decodeHuffmanCode[i].symbol;
            }
        }
    }
    return INVALID_HUFFMAN_SYMBOL;
}


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


/**************************************************************************
 * @name decodeHuffman
 *
 * @description Refer huffman.h for more information.
 *************************************************************************/
void decodeHuffman(uint8_t* buffer, size_t nBytes, char * msg){
    //initialize the index for both the buffers to 0
	int bufIndex = 0;
    int msgIndex = 0;

    //code bits out of which we will search for the chunks in HUFFMAN TABLE
    uint32_t codeValueToSearch = 0;

    //code in HUFFMAN TABLE for a particular encoded bits
    uint32_t getCharFromHuffmanCode = INVALID_HUFFMAN_SYMBOL;
    //length of encoded bits in picture
    uint8_t bitsToConsiderInCurrByte = 0;

    //loop break variable
    bool breakLoop = false;
    bool endDecoding = false;
    uint32_t temp = 0;

    while(nBytes > 0){
    	//update the bits in picture
        codeValueToSearch = (codeValueToSearch << BITS_IN_A_BYTE) | buffer[bufIndex];
        //update the number of bits in picture
        bitsToConsiderInCurrByte = bitsToConsiderInCurrByte + BITS_IN_A_BYTE;

        while(1){
        	//make encoded string from HUFFMAN_MIN_LENGTH: i and search for it in the HUFFMAN Table
            for(int i = HUFFMAN_MIN_LENGTH; i <= HUFFMAN_MAX_LENGTH; i++){
            	//if the remaining bits are not in the HUFFMAN table, break the loop and append it to next byte
                if((bitsToConsiderInCurrByte - i) < 0){
                    breakLoop = true;
                    break;
                }
                //update the encoded bits in picture
                temp = codeValueToSearch >> (bitsToConsiderInCurrByte - i);
                //check if the encoded bits are in the HUFFMAN table or not
                getCharFromHuffmanCode = getHuffmanSymbol(temp, i);
                //if HUFFMAN_END_CODE_SYMBOL is found, stop decoding further
                if(getCharFromHuffmanCode == HUFFMAN_END_CODE_SYMBOL){
                	//update the decoded string
                    msg[msgIndex] = getCharFromHuffmanCode;
                    msgIndex++;
                    //break the loops
                    nBytes = 0;
                    endDecoding = true;
                    break;
                }
                else if(getCharFromHuffmanCode != INVALID_HUFFMAN_SYMBOL){
                	//add the found symbol in the msg string
                    msg[msgIndex] = getCharFromHuffmanCode;
                    msgIndex++;
                    //update the bits to search in the next iteration
                    codeValueToSearch = codeValueToSearch & MASK((bitsToConsiderInCurrByte - i));
                    //update the number of bits to search in the next iteration
                    bitsToConsiderInCurrByte = bitsToConsiderInCurrByte - i;
                    break;
                }
            }

            //if there are not enough bits to search or breakloop condition is true, break the loop
            if(bitsToConsiderInCurrByte < HUFFMAN_MIN_LENGTH || breakLoop){
                bufIndex++;
                nBytes--;
                breakLoop = false;
                break;
            }
            //end decoding
            if(endDecoding){
                break;
            }
        }
        //end decoding
        if(endDecoding){
            endDecoding = false;
            break;
        }
    }
}

