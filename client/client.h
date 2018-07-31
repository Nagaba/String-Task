/* file to contatain the function prototypes and constants for the client */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "../includes/utility.h"

#define BUFFER_SIZE 1024        //size of the buffer in bytes


/*
    Function to provide run the main client program 
*/
void client_run(const char **args);

/*
    Function to prompt the user to enter the string from the terminal
*/
const char *prompt(char buff[]);
/*
    Function to interprete the commands typed by the client from the prompt
*/
void command_interpretor(char *command);

/*
    Function to print the help menu showing the list of permitted commands 
    the string Task program can accept
*/
void print_help(void);

/*
    Function to print the reply from the server to the client's terminal
*/
void print_reply(char *reply);

#endif