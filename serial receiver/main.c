/**********************************************************************************
 * @file main.c
 ********************************************************************************** 
 * Date:        04-30-2022
 * Author:      Ayush Gupta
 * Description: This file deals deals with the functions in the read_serial.h file.
 *              Show the related statistic to compare HUFFMAN CODING compression.
 *********************************************************************************/
#include <stdio.h>
#include <string.h>
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
#define USER_MESSAGE_LENGTH                                                             (50)
#define ASCII_BACKSPACE_CHARACTER                                                       (8)

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

    char userInput[USER_MESSAGE_LENGTH];
    int userIndex = 0;
    char ch;
    while(1){
        printf("\r\n>");
        userIndex = 0;
        while (userIndex < USER_MESSAGE_LENGTH){
            scanf("%c", &ch);
            
            if (ch == ASCII_BACKSPACE_CHARACTER){
                //ignore backspace character
                if(userIndex > 0)
                    userIndex--;
            }
            else if ((ch == '\r') || (ch == '\n')){
                //break if newline character found
                break;
            }
            else{
                //copy the character and increment the index
                userInput[userIndex] = ch;
                userIndex++;
            }
        }

        if (strncmp("compare", userInput, userIndex) == 0){
            //display the stats
            getStats();
        }
        else{
            userInput[userIndex++] = '\r';
            userInput[userIndex++] = '\n';

            
            int countWriteBytes = 0;
            int msgLength = strlen(userInput);

            char * cmd = userInput;
            //wait till all the bytes are sent to the serial port
            while(msgLength > 0){
                //send data
                countWriteBytes = write(serialPort, cmd, msgLength);
                msgLength -= countWriteBytes;
                cmd += countWriteBytes;
            }
            
            //receive and decode the bytes
            decodeMessages(serialPort, msg, RECEIVED_MESSAGE_LENGTH);
            printf("%s", msg);
            memset(msg, 0 , RECEIVED_MESSAGE_LENGTH);
        }
    }

    //close the serial port
    close(serialPort);
    return 0;
};