/**********************************************************************************
 * @file read_serial.c
 ********************************************************************************** 
 * Date:        04-30-2022
 * Author:      Ayush Gupta
 * Description: This file deals with the reception and decoding of huffman encoded 
 *              serial data.
 *********************************************************************************/

#include "read_serial.h"

#define MIN(X, Y) 												(((X) < (Y)) ? (X) : (Y))
#define MASK(X)													((1<<X) - 1)
#define INVALID_HUFFMAN_SYMBOL                                  (0xFFFFFFFF)
#define BITS_IN_A_BYTE                                          (8)

int countBytes = 0;
int countDecodedBytes = 0;

//Huffman table: symbol, encoded messages and length of encoded messages
typedef struct {
	uint8_t symbol;
	char code[20];
	int nBits;
} decodeHuffmanCode_t;

/**************************************************************************
 * @name decodehuffmanCode
 * 
 * @description huffman characters, encoded bits and length of encoded bits.
 *************************************************************************/
decodeHuffmanCode_t decodehuffmanCode[] = {
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
 * @param in
 *  str - copy integer into passed string
 *  code - code to find in the huffman table
 *  len - length of the code
 * 
 * @description convert intger into string of bits.
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
 * @param in
 *  code - code to find in the huffman table
 *  len - length of the code
 * 
 * @description find if a code is present in the Huffman Table based on the
 *              code and length of code passed in.
 *************************************************************************/
static uint32_t getHuffmanSymbol(uint32_t code, size_t len){
    char temp[20]; 
    
    //iterate over huffman table to find if the code is available or not
    for(int i = 0; i < sizeof(decodehuffmanCode)/sizeof(decodeHuffmanCode_t); i++){
        //if len is equal to the length of number of bits in huffman encoded table
        if(len == decodehuffmanCode[i].nBits){
            //convert integer to bit string
            intToStr(temp, code, len);
            //if code matches with huffman encoded code return symbol
            if (strncmp(temp, decodehuffmanCode[i].code, decodehuffmanCode[i].nBits) == 0){
                return decodehuffmanCode[i].symbol;
            }
        }
    }
    return INVALID_HUFFMAN_SYMBOL;
}

/**************************************************************************
 * @name setTTYFlags
 * 
 * @param in
 *  tty - pointer to structure termios 
 * 
 * @description intialize the termios structure for UART reception.
 *************************************************************************/
static void setTTYFlags(struct termios* tty){
    //clear parity bit, disabling parity (most common)
    tty->c_cflag &= ~PARENB;    
     //clear stop field, only one stop bit used in communication (most common)                                        
    tty->c_cflag &= ~CSTOPB;     
    //clear all bits that set the data size                                       
    tty->c_cflag &= ~CSIZE;     
    //8 bits per byte (most common)                                        
    tty->c_cflag |= CS8;      
    //disable RTS/CTS hardware flow control (most common)                                          
    tty->c_cflag &= ~CRTSCTS;    
    //turn on READ & ignore ctrl lines (CLOCAL = 1)                                       
    tty->c_cflag |= CREAD | CLOCAL;           
    //clear ICANON flag                          
    tty->c_lflag &= ~ICANON;     
    //disable echo                                       
    tty->c_lflag &= ~ECHO;             
    //disable erasure                                 
    tty->c_lflag &= ~ECHOE;
    //disable new-line echo                                             
    tty->c_lflag &= ~ECHONL;                                           
    //disable interpretation of INTR, QUIT and SUSP
    tty->c_lflag &= ~ISIG;                                             
    //turn off s/w flow ctrl
    tty->c_iflag &= ~(IXON | IXOFF | IXANY);  
    //disable any special handling of received bytes                          
    tty->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);   
    //prevent special interpretation of output bytes (e.g. newline chars)
    tty->c_oflag &= ~OPOST;   
    //prevent conversion of newline to carriage return/line feed                                          
    tty->c_oflag &= ~ONLCR;                                             
    //wait for up to 1s (5 deciseconds), returning as soon as any data is received.
    tty->c_cc[VTIME] = 5;                                               
    //set vmin to 0
    tty->c_cc[VMIN] = 0;                                                
}

/**************************************************************************
 * @name configureSerialPort
 * 
 * @description Refer read_serial.h for more details.
 *************************************************************************/
int configureSerialPort(struct termios* tty){
    // Open the serial port)
    int serialPort = open(SERIAL_PORT_KL25Z, O_RDWR);

    // Read in existing settings, and handle any error
    if(tcgetattr(serialPort, tty) != 0) {
        printf("Error %i from tcgetattr: %s\r\n", errno, strerror(errno));
        return -1;
    }

    //set tty flags
    setTTYFlags(tty);
    
    // Set in/out baud rate
    cfsetispeed(tty, BAUD_RATE);
    cfsetospeed(tty, BAUD_RATE);

    // Save tty settings, also checking for error
    if (tcsetattr(serialPort, TCSANOW, tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return serialPort;
}

/**************************************************************************
 * @name readSerialPortData
 * 
 * @param in
 *  serialPort - open the serial port and assign it to serialPort
 *  data - data to be read from the serial port
 *  dataCapacity - length of data array 
 * 
 * @description read data from serial port and store it in data.
 *************************************************************************/
static int readSerialPortData(int serialPort, uint8_t* data, int dataCapacity){
    //making all entries 0
    memset(data, 0, dataCapacity);

    //read call on serial port
    int nBytes = read(serialPort, data, dataCapacity);

    //if rx bytes < 0, means error occured.
    if (nBytes <= 0) {
        printf("Error reading: %s", strerror(errno));
        return 0;
    }
    return nBytes;
}

/**************************************************************************
 * @name decodeMessages
 * 
 * @description Refer read_serial.h for more details.
 *************************************************************************/
void decodeMessages(int serialPort, char* msg, int msgLen){
    //temporary buffer to read serial data
    uint8_t readBuf[RECEIVED_MESSAGE_LENGTH];
    uint32_t getCharFromHuffmanCode = INVALID_HUFFMAN_SYMBOL;
    static uint32_t codeValueToSearch = 0;
    // Allocate memory for read buffer, set size according to your needs
    static int bufIndex = 0;
    static int msgIndex = 0;
    static uint8_t bitsToConsiderInCurrByte = 0;

    bool breakLoop = false;
    bool endDecoding = false;
    uint32_t temp = 0;
    while(1){
        int nBytes = readSerialPortData(serialPort, readBuf, sizeof(readBuf));
        if(nBytes == 0){
            return;
        }

        //increment the countBytes to keep track of received bytes
        countBytes += nBytes;
        //initialize buffer index = 0
        bufIndex = 0;

        //iterate over the received data, take a chunk and decode it
        while(nBytes > 0){
            //codeValueToSearch: total number out of it a part of it will be decoded using Huffman Table
            codeValueToSearch = (codeValueToSearch << BITS_IN_A_BYTE) | readBuf[bufIndex];
            //total number of bits in picture = number of bits in codeValueToSearch
            bitsToConsiderInCurrByte = bitsToConsiderInCurrByte + BITS_IN_A_BYTE;
            while(1){
                //pick encoded bits of size [HUFFMAN_MIN_LENGTH - i] and find symbol in the Huffman Table
                for(int i = HUFFMAN_MIN_LENGTH; i <= HUFFMAN_MAX_LENGTH; i++){
                    //if length of encoded bits (which is equal to (bitsToConsiderInCurrByte - i)) is less than 0
                    if((bitsToConsiderInCurrByte - i) < 0){
                        //break the loop and proceed to next step
                        breakLoop = true;
                        break;
                    }
                    //temp: encoded value to search in huffman table
                    temp = codeValueToSearch >> (bitsToConsiderInCurrByte - i);
                    //get the symbol if temp is present in the huffman table
                    getCharFromHuffmanCode = getHuffmanSymbol(temp, i);

                    //if the found symbol is HUFFMAN_END_CODE_SYMBOL, means the end of message
                    if(getCharFromHuffmanCode == HUFFMAN_END_CODE_SYMBOL){
                        //store the decoded character in the msg buffer
                        msg[msgIndex] = getCharFromHuffmanCode;
                        msgIndex++;

                        countDecodedBytes += strlen(msg);  

                        //reset the variables, to decode next message
                        msgIndex = 0;
                        nBytes = 0;
                        codeValueToSearch = 0;
                        bitsToConsiderInCurrByte = 0;
                        //break all loops and wait for next message
                        endDecoding = true;
                        return;
                        break;
                    }
                    else if(getCharFromHuffmanCode != INVALID_HUFFMAN_SYMBOL){
                        //store the decoded character in the msg buffer
                        msg[msgIndex] = getCharFromHuffmanCode;
                        msgIndex++;
                        //remove the found bits and rearrange the data
                        codeValueToSearch = codeValueToSearch & MASK((bitsToConsiderInCurrByte - i));
                        //update the number of bits to consider in the next iteration
                        bitsToConsiderInCurrByte = bitsToConsiderInCurrByte - i;
                        break;
                    }
                }
                //if bits left to consider is still greater than HUFFMAN_MIN_LENGTH, keep decoding otherwise break
                if((bitsToConsiderInCurrByte < HUFFMAN_MIN_LENGTH) || (breakLoop)){
                    bufIndex++;
                    nBytes--;
                    breakLoop = false;
                    break;
                }
                //stop decoding
                if(endDecoding){
                    break;
                }
            }
            //stop decoding
            if(endDecoding){
                endDecoding = false;
                break;
            }
        }
    }
}

