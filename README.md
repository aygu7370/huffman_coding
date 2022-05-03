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

## Project Planning
The Project is divided into various submodules and then completed one step at a time. [Here](project_plan.xlsx) is the modular approach that has been followed during the execution of this project.

## Generation of Huffman Map Table
A [python script](huffman%20code%20generator/huffman.py) is used to make the [huffman table for encoding](huffman%20code%20generator/encodeHuffman.txt) and [huffman table for decoding](huffman%20code%20generator/decodeHuffman.txt) using a [corpus](huffman%20code%20generator/corpus.txt). This process is very flexible to do. All the user has to add sentences in the [corpus](huffman%20code%20generator/corpus.txt) and run the python script. It is generate the huffman tree for both encode and decode end. User then has to replace the previous huffman tables with the latest huffman table.

## Configuring the Serial Port
KL25z: The macro ```BAUD_RATE``` present in ```uart.h``` can easily be configured to work with any Baud Rate.
Ubuntu: The macro ```SERIAL_PORT_KL25Z``` to set the serial port (in my case: /dev/ttyS8) and ```BAUD_RATE``` (in my case B115200) to configure the serial port.

Currently, the serial port is working on <b>115200 Baud Rate, no parity and 1 stop bit mode</b>. 

## Software Flow Control
1. System Overview: There is a program running on KL25z which will receive the commands from Serial Port (UART0) and process it. If the command is valid it will send its response back which will be encoded using the Huffman Algorithm. 
   Here is a high-level system overview.
   ![overview](screenshots/pes3.png)

2. KL25z: Encode the outgoing messages using HUFFMAN coding technique. The string is currently being compressed in the ```__sys_write``` function associated RedLib library. 
   
   ![kl25z_software_flow](screenshots/pes1.png)
   
3. PC Front: On the serial front the data is being decompressed using the HUFFMAN TABLE (symbol, code & number of bits). If ```HUFFMAN_END_SYMBOL``` ('\0') is detected, the remaining bits are discarded. 
   
   ![PC_software_flow](screenshots/pes2.png)


### Test Results
The test plan for verifying the Huffman implementation and integration is given in the [Test Plan](test_plan.xlsx).  

Unit test cases were written to verify each step. Following are the tests that are executed on system start-up.
1. Test Huffman Coding Implementation: If system is in ```DEBUG``` mode, test APIs will execute on the KL25Z, which will execute the Huffman Encoding and Decoding and uses assert statements while comparing the two results. 
2. Test Circular Queue Implementation: If system is in ```DEBUG``` mode, test APIs will execute on the KL25Z, which will perform the various scenarios on Circular queue and uses assert statements while comparing the results. 
   
   Once these tests are passed, the system proceeds to the command processor part.


Initial scope of this project was to implement the Huffman algorithm. As that part is completed, I jumped on to the assignment 6 and extended it by integrating it with my implementation of Huffman Algorithm to compress the data.
    
3. The commands are kept as it is from assignment 6. Here are a few screenshots showing the working of various commands.
   
   ![cmd1](screenshots/author_cmd.jpg)

   ![cmd2](screenshots/dump_cmd.jpg)

   ![cmd1](screenshots/info_cmd.jpg)

4. A new command which is added to the system is ```compare```. Upon receiving this command the stats related to Huffman will be displayed on the terminal. Below is the picture depicting the same. 

      ![cmd1](screenshots/compare_cmd.jpg)

### Code Explanantion
![code_explanation](Documentation/code_explanation.mp4)

### Demo
![demo](Documentation/project_working.mp4)

![stats](Documentation/stats_display.mp4)


### Things to remember
 1. Keep in mind that the characters in the strings below should be a part of the corpus passed to python script. Otherwise there will be no encoded bits and the program will stuck in a loop.
 2. '\0' is used as a HUFFMAN ending character, anything after HUFFMAN_LAST_SYMBOL will be discarded and hence will be lost.


### System Limitations & Possible Future Improvements
1. The corpus that is selected for this application is a bit limited to what it can encode. As the size increases the number of encoded bits increases and with that the time of encoding the bits increases exponentially. A high-quality corpus can be chosen to make improvements. For this application, this corpus can work.
2. The dump function can print upto 130 bytes of data. This is due to the fact that for dump commands I am currently sending one encoded string which is limited by the size of buffers defined in the system. Although the change is minimalistic as all the buffer lengths are defined using a MACRO which are configurable in a single go.
3. Huffman encoding and decoding can be implemented on both the ends to compress the data further.

### References 
1. https://en.wikipedia.org/wiki/Huffman_coding
2. https://brilliant.org/wiki/huffman-encoding/
3. https://www.cyberciti.biz/faq/find-out-linux-serial-ports-with-setserial/
4. https://www.mouser.com/pdfdocs/FRDM-KL25Z.pdf
5. https://www.programiz.com/dsa/huffman-coding