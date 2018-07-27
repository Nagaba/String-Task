/* file to contatain the function prototypes and constants for the server */

#ifndef SERVER_H_
#define SERVER_H_


 //for signal handling
#include <sys/wait.h>
#include <signal.h>

#include "../includes/utility.h"

#define BUFFER_SIZE 1024        //size of the buffer in bytes
#define BACKLOG    10          //the maximum number of clients the server can suppport at a time

#define SOCKET_ERROR "SOCKET ERROR"         //the socket error macro
#define RECIEVE_FAILURE "RECEIVE FAILURE"   //receive failure macro

/* 
    function to handle the signal incase of new incoming  connections
    it takes the socket file descripter attempting to connect
    it returns nothing
*/
void signal_chld_handler(int soc);

/*
    Function to run the main server routine
*/
void server_run(void);

/*
    Function to process the task, eg delete, double, reverse string etc
	takes the task grepped from the request message, where to write the result 'result'
	and the task id
	returns the rsult is a well packaged format the client understands
*/
char *process_task(char *task, char *result, int task_id);

/*
    Function to pakage the result obtained after processing the task in 
    the format that the client understands.
*/
char *package_result(char *result, char *msg, int task_id);

/*
    Function to service the client's requests
*/
char *service_client(char *request, char reply[]);

#endif