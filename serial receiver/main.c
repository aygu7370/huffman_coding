/**********************************************************************************
 * @file main.c
 ********************************************************************************** 
 * Date:        04-30-2022
 * Author:      Ayush Gupta
 * Description: This file deals deals with the functions in the read_serial.h file.
 *              Show the related statistic to compare HUFFMAN CODING compression.
 *********************************************************************************/
#include <stdio.h>

//include Linux headers
#include <fcntl.h>          // Contains file controls like O_RDWR
#include <errno.h>          // Error integer and strerror() function
#include <termios.h>        // Contains POSIX terminal control definitions
#include <unistd.h>         // write(), read(), close() functions


#include "read_serial.h"

#define NUMBER_OF_COMMANDS                                                              (6)
#define COMMAND_LENGTH                                                                  (15)
#define NUMBER_OF_TEST_CASES                                                            (17)
#define BUFFER_LENGTH                                                                   (40)


int main() {
    //define am integer to hold serialport value
    int serialPort;
    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    //configure serial port, open the port, set baudrate
    serialPort = configureSerialPort(&tty);
    if(serialPort == -1){
        return -1;
    }
    
    //store received messages 
    char msg[RECEIVED_MESSAGE_LENGTH];
    //clearing the buffer
    memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);

    //receive and decode the bytes
    decodeMessages(serialPort, msg, RECEIVED_MESSAGE_LENGTH);
    printf("%s", msg);
    memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);

    //receive and decode the bytes
    // decodeMessages(serialPort, msg, RECEIVED_MESSAGE_LENGTH);
    // printf("%s", msg);
    // memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);

    char userInput[50];
    int userIndex = 0;
    char ch;
    while(1){
        printf("\r\n>");
        for (userIndex = 0; userIndex < 50; userIndex++){
            scanf("%c", &ch);
            if ((ch == '\r') || (ch == '\n')){
                break;
            }
            userInput[userIndex] = ch;
        }

        userInput[userIndex++] = '\r';
        userInput[userIndex++] = '\n';

        //send characters
        int countWriteBytes = 0;
        int msgLength = strlen(userInput);

        char * cmd = userInput;
        while(msgLength > 0){
            countWriteBytes = write(serialPort, cmd, msgLength);
            msgLength -= countWriteBytes;
            cmd += countWriteBytes;
        }
        
            
        //sleep enough to transmit the strlen(writeStr) plus
        // usleep((strlen(userInput) + 100) * 100);
        //receive and decode the bytes
        decodeMessages(serialPort, msg, RECEIVED_MESSAGE_LENGTH);
        printf("%s", msg);
        memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);

        // for(int i = 0; i < NUMBER_OF_COMMANDS; i++){
        //     //send characters
        //     int countWriteBytes = 0;
        //     int msgLength = strlen(cmdInputs[i]);

        //     char * cmd = cmdInputs[i];
        //     printf("\nEntered Command: %s\n", cmdInputs[i]);
        //     while(msgLength > 0){
        //         countWriteBytes = write(serialPort, cmd, msgLength);
        //         msgLength -= countWriteBytes;
        //         cmd += countWriteBytes;
        //     }
            
                
        //     //sleep enough to transmit the strlen(writeStr) plus
        //     // usleep((strlen(cmdInputs[i]) + 10000) * 100);
        //     usleep((strlen(cmdInputs[i]) + 100) * 100);
        //     //receive and decode the bytes
        //     decodeMessages(serialPort, msg, RECEIVED_MESSAGE_LENGTH);
        //     printf("%s", msg);
        //     memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);
        //     usleep((strlen(cmdInputs[i]) + 100) * 100);


            
        // }
    }

    //close the serial port
    close(serialPort);
    return 0;
};