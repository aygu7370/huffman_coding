/**********************************************************************************
 * @file read_serial.h
 ********************************************************************************** 
 * Date:        04-30-2022
 * Author:      Ayush Gupta
 * Description: This file deals with the reception and decoding of huffman encoded 
 *              serial data.
 *********************************************************************************/

//include header files
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

//include Linux headers
#include <fcntl.h>          // Contains file controls like O_RDWR
#include <errno.h>          // Error integer and strerror() function
#include <termios.h>        // Contains POSIX terminal control definitions
#include <unistd.h>         // write(), read(), close() functions

//serial port on UBUNTU system
#define SERIAL_PORT_KL25Z                                       ("/dev/ttyS8")
#define HUFFMAN_END_CODE_SYMBOL                                 '\0'
#define BAUD_RATE                                               B115200
#define HUFFMAN_MIN_LENGTH                                      (2)
#define HUFFMAN_MAX_LENGTH                                      (11)
#define RECEIVED_MESSAGE_LENGTH                                 (100)

/**************************************************************************
 * @name configureSerialPort
 * 
 * @param in
 *  serialPort - open the serial port and assign it to serialPort
 *  tty - pointer to structure termios 
 * 
 * @description configure the Serial Port, open the port, set tty flags,
 *              and set the baud rate
 *************************************************************************/
int configureSerialPort(struct termios* tty);


/**************************************************************************
 * @name decodeMessages
 * 
 * @param in
 *  serialPort - open the serial port and assign it to serialPort
 *  msg - message string
 *  msgLen - message length
 * 
 * @description decode the received message from serial port.
 *************************************************************************/
void decodeMessages(int serialPort, char* msg, int msgLen);