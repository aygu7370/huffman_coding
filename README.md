# ECEN 5813 - Huffman Coding for Debug Messages
This repository contains the code for Huffman Coding Algorithm for UART messages on FRDM KL25z development board.

## Overview
Implementation of Huffman Coding algorithm on FRDM KL25Z development board to compress the messages sent over Serial Port.

## Hardware Components
1. FRDM KL25Z Development board
2. A PC with ubuntu operating systems

## Software Components
1. MCU Expresso IDE version - 1.2.0
2. GCC compiler
3. Teraterm/Putty to test the UART Transmitter and Receiver

##  Software Modules
Following are the software modules required to realize the proposed solution:
1. <b>UART0 module</b>: Initialize the UART0 module on KL25Z to be able to sent out data on the serial port.
2. <b>Huffman Coding Algorithm module</b>: This module will have all the Huffman coding logic and to be called before transmitting the string on to the serial port.
3. <b>Circular Queue Module</b>: This module will have the implementation of Circular Queue which will act as a buffer, required to store the transmitting data.
4. <b>Test Huffman Module</b>: This module will have the automated test cases to verify the Huffman Coding Algorithm.
5. <b>Test Circular Queue Module</b>: This module will have the automated test cases to verify the working of Circular Queue.

## Technologies Involved
1. Huffman Coding (Data Compression Technique)
2. Circular Buffers (to store the messages to be sent over the serial port)

## Generation of Huffman Map Table



## Software Flow Control
1. KL25z: Encode the outgoing messages using HUFFMAN coding technique. The string is currently being compressed in the ```__sys_write``` function associated RedLib library. 
   
   ![kl25z_software_flow](screenshots/pes1.png)
   
2. PC Front: On the serial front the data is being decompressed using the HUFFMAN TABLE (symbol, code & number of bits). If ```HUFFMAN_END_SYMBOL``` ('\0') is detected, the remaining bits are discarded. 
   
   ![PC_software_flow](screenshots/pes2.png)


### Things to remember
 1. Keep in mind that the characters in the strings below should be a part of the corpus passed to python script. Otherwise there will be no encoded bits and the program will stuck in a loop.
 2. '\0' is used as a HUFFMAN ending character, anything after HUFFMAN_LAST_SYMBOL will be discarded and hence will be lost.