 /****************************************************************************************
 * @file cli.c
 *****************************************************************************************
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related to
 * 				CLI commands.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ****************************************************************************************/

#include "cli.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "hexdump.h"

/*********************** String Handling Macros *******************************************/
#define XSTR(x)	#x
#define STR(x) 	XSTR(x)


typedef void (*command_handler_t)(int, char *argv[]);

//Look-up table data structure
typedef struct{
	const char *name;
	command_handler_t handler;
	const char *help_string;
} command_table_t;




/*****************************************************************************************
 * @name: handle_author
 *
 * @param: in
 * 	argc: number of arguments
 * 	argv: argument values
 *
 * @description: handle user command: author.
 *****************************************************************************************/
static void handle_author(int argc, char *argv[]){
	printf("Ayush Gupta\r\n");
}


/*****************************************************************************************
 * @name: handle_invalid
 *
 * @param: in
 * 	argc: number of arguments
 * 	argv: argument values
 *
 * @description: handle all the invalid commands enterd by user.
 *****************************************************************************************/
static void handle_invalid(int argc, char *argv[]){
	printf("Invalid Command: %s\r\n", argv[0]);
}

/*****************************************************************************************
 * @name: handle_dump
 *
 * @param: in
 * 	argc: number of arguments
 * 	argv: argument values
 *
 * @description: handle user command: dump.
 *****************************************************************************************/
static void handle_dump(int argc, char *argv[]){
	uint32_t origin = 0;
	size_t len = 0;
	if(argc != 3){
		handle_invalid(argc, argv);
		return;
	}
	sscanf(*(&argv[1]), "%x", &origin);
	sscanf(*(&argv[2]), "%i", &len);
	displayHexDump((void*)origin, len);
}

/*****************************************************************************************
 * @name: handle_help
 *
 * @param: in
 * 	argc: number of arguments
 * 	argv: argument values
 *
 * @description: handle user command: help.
 *****************************************************************************************/
static void handle_help(int argc, char *argv[]){
	//list all the commands user can enter in the serial monitor
	printf("| Author | Args: <> | Prints a string with your name\r\n");
	printf("| Dump | Args: <start> <len> | Dumps the memory requested; <start> in Hex and <len> can be in any format\r\n");
	printf("| Help | Args: <> | Prints the user menu\r\n");
	printf("| Info | Args: <> | Prints a string with build information\r\n");
}

static void handle_info(int argc, char *argv[]){
#ifndef BUILD_COMMIT
	printf("ERR: BUILD_COMMIT is not defined\r\n");
#endif

#ifndef HOSTNAME
	printf("ERR: HOSTNAME is not defined\r\n");
#endif

#ifndef DATE_TIME
	printf("ERR: DATE_TIME is not defined\r\n");
#endif

	//print the info string with hostname, date and build information
	printf("Version 1.0 built on %s at %s Commit %s\r\n", STR(HOSTNAME), STR(DATE_TIME), STR(BUILD_COMMIT));
}

//commands look-up table
static const command_table_t cmds[] = {
		{"author",  handle_author, 	"Command: Author | Args: <>				| Message: Prints a string with your name\r\n"},
		{"dump",  	handle_dump, 	"Command: Dump   | Args: <start> <len>	| Message: Prints a hexdump of the memory requested, with up to 16 bytes per line of output. <start> in Hex and <len> can be in any format\r\n"},
		{"help",  	handle_help, 	"Command: Help   | Args: <> 		 	| Message: Prints the user menu\r\n"},
		{"info",  	handle_info, 	"Command: Info	 | Args: <> 		 	| Message: Prints a string with build information\r\n"},
		{" ",  		handle_invalid}
};


/*****************************************************************************************
 * @description: number of commands handled by the CLI.
 *****************************************************************************************/
static const int nCommands = sizeof(cmds)/sizeof(command_table_t);

/*****************************************************************************************
 * @name: processCommand
 *
 * @description: Refer cli.h for more detail.
 *****************************************************************************************/
void processCommand(char *userInput){
	char *p = userInput;
	char *end;
	if(*userInput == '\0'){
		return;
	}
	//To find the end pointer
	for(end = userInput; *end != '\0'; end++);

	//Tokenize input in place
	bool inToken = false;
	char *argv[10];
	int argc = 0;
	memset(argv, 0, sizeof(argv));

	for (p = userInput; p < end; p++) {
		// TODO: Your code here!
		//if space is encountered, and in_token is true, replace it with \0
		if(*p == ' '){
			if(!inToken){
				*p = ' ';
			}
			else{
				*p = '\0';
				inToken = false;
			}
		}
		else{
			if(!inToken){
				//if in_token is false, copy the string in argv and increment argc by 1
				argv[argc] = p;
				argc++;
			}
			inToken = true;
		}
	}

	argv[argc] = NULL;
	// if no command
	if (argc == 0)
		return;
	// TODO: Dispatch argc/argv to handler
	for(int i=0; i<nCommands; i++){
		if(strcasecmp(argv[0], cmds[i].name) == 0){
			cmds[i].handler(argc, argv);					//calls corresponding function handler
			return;
		}
	}
	handle_invalid(argc, argv);
	return;
}
