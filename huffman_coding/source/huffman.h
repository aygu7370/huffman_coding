 /****************************************************************************************
 * @file huffman.h
 *****************************************************************************************
 * Date:        04-27-2022
 * Author:      Ayush Gupta
 * Description: PES - Final Project. This file handles all the operations related to
 * 				huffman.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/

#ifndef SOURCE_HUFFMAN_H_
#define SOURCE_HUFFMAN_H_

#include <stdint.h>

//HUFFMAN encoded minimum & maximum lengths
#define HUFFMAN_MIN_LENGTH              							(2)
#define HUFFMAN_MAX_LENGTH              							(11)

//Huffman table: symbol, encoded messages and length of encoded messages
typedef struct {
	uint8_t symbol;
	uint32_t code;
	int nBits;
} encode_huffmanCode_t;

/**************************************************************************
 * @name encodeHuffman
 *
 * @param in
 * 	msg: message string
 * 	buffer: buffer to store encoded message
 * 	nBytes: number of bytes to encode
 *
 * @description encode the msg string passed to the function using HUFFMAN
 * 				CODING algorithm. returns the length of huffman encoded
 * 				message.
 *************************************************************************/
int encodeHuffman(const char* msg, uint8_t* buffer, size_t nBytes);

#endif /* SOURCE_HUFFMAN_H_ */
