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
#define HUFFMAN_END_CODE_SYMBOL                                     ('\0')

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


/**************************************************************************
 * @name decodeHuffman
 *
 * @param in
 * 	msg: message string to store the decoded bytes
 * 	buffer: encoded message buffer
 * 	nBytes: number of bytes to decode
 *
 * @description Refer huffman.h for more information.
 *************************************************************************/
void decodeHuffman(uint8_t* buffer, size_t nBytes, char * msg);

#endif /* SOURCE_HUFFMAN_H_ */
