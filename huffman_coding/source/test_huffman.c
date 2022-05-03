 /****************************************************************************************
 * @file test_huffman.c
 *****************************************************************************************
 * Date:        04-27-2022
 * Author:      Ayush Gupta
 * Description: PES - Final Project. This file has the test cases to test huffman algorithm
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course project.
 ****************************************************************************************/


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include "huffman.h"


#define NUMBER_OF_TEST_CASES                                                           (17)
#define BUFFER_LENGTH                                                                  (40)

/*****************************************************************************************
 * This array has all the combinations possible to test the HUFFMAN coding algorithm.
 *
 * Note: Keep in mind that the characters in the strings below should be a part of the
 * 		corpus passed to python script. Otherwise there will be no encoded bits and the
 * 		program will stuck in a loop.
 * 		'\0' is used as a HUFFMAN ending character, anything after HUFFMAN_LAST_SYMBOL
 * 		will not be decoded and lost.
 *
 ****************************************************************************************/
char huffmanInputs[NUMBER_OF_TEST_CASES][BUFFER_LENGTH+1] = {
    "Final Project",            //normal string
    "Embedded Software",        //normal string
    "Principles of Embedded",   //normal string
    "eeeeeeeeeeee",             //string with one character repeated all its length
    "ECEN 5813",                //alphanumeric string
    "r",                        //one character string
    ">",                        //punctuation character
    "ECEN>",                    //combination of alphabets and punctuation
    "abcdefghijklmnop",         //string with all unique character
    "Huffman Coding API for Final Project", //a normal debug message length
    "random_script from my mind",	//normal string
    "FRDM KL25Z Development board",	//normal string
    "",								//blank string
    "1234567891011121314151617",	//numbers
    ":::::::::::::::::::::::::::::::::::", //punctualtion
    "ahahhvdfbkr-ngrpgkfnjkd-lnfdjbf",		//alphanumeric string
    "KL25z -  a 32-bit micro-controller"	//normal string
};


/*****************************************************************************************
 * @testHuffman
 *
 * @description Refer to test_huffman.h for more details.
 ****************************************************************************************/
void testHuffman(){
    //define buffers to store the encoded message and de-compressed message
    uint8_t encodedString[BUFFER_LENGTH+1];
    char outputsString[BUFFER_LENGTH+1];

    //iterate over the test cases to test the huffman algorithm
    for(int i = 0; i<NUMBER_OF_TEST_CASES; i++){
        //clear both the buffers
        memset(encodedString, 0, BUFFER_LENGTH+1);
        memset(outputsString, 0 , BUFFER_LENGTH+1);

        //encode the message
        int count = encodeHuffman(huffmanInputs[i], encodedString, strlen(huffmanInputs[i]));

        //decode the message
        decodeHuffman(encodedString, count, outputsString);

        //compare the input and output strings
        assert(strncmp(outputsString, huffmanInputs[i], strlen(huffmanInputs[i])) == 0);
    }
}

/*****************************************************************************************
 * @testHuffman
 *
 * @description Refer to test_huffman.h for more details.
 ****************************************************************************************/
void analyzeHuffmanEncoding(){
	for(int i = 0; i<NUMBER_OF_TEST_CASES; i++){
		printf("%s\r\n", huffmanInputs[i]);
	}
	printf("Compare");
}
