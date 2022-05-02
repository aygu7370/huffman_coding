 /****************************************************************************************
 * @file hexdump.h
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related hex dump.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/******************************************************************************************
 * @name displayHexDump
 *
 * @param in
 *  buf - buffer to be displayed on serial terminal.
 *  nBytes - number of bytes requested.
 *
 * @description Function displays the hexdump for the requested memory.
 * ***************************************************************************************/
void displayHexDump(const void *buf, size_t nBytes);

#endif /* HEXDUMP_H_ */
