 /****************************************************************************************
 * @file cli.h
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related to
 * 				CLI commands.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/

#ifndef CLI_H_
#define CLI_H_


/*****************************************************************************************
 * @name: processCommand
 *
 * @param in
 * 	userInput: user input string.
 *
 * @description: this Function processes the incoming message.
 *****************************************************************************************/
void processCommand(char *userInput);

#endif /* CLI_H_ */
