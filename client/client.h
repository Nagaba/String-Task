/* file to contatain the function prototypes and constants for the client */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "../includes/utility.h"

#define BUFFER_SIZE 1024        //size of the buffer in bytes



//function to provide run the main client program 
void client_run(const char **args);

//function to prompt the user to enter the string from the terminal
const char *prompt(char buff[]);

//function to interprete the commands typed by the client from the prompt
void command_interpretor(char *command);

#endif