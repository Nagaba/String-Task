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
char *package_result(int task_id, char *task, char *result, char buffer[]);

/*
    Function to pakage the error obtained after processing the task and 
    identifies an error in the command, in the format that the client understands.
*/
char *package_error(char *error, int task_id, char *err_msg, char buffer[]);

/*
    Function to service the client's requests
*/
char *service_client(char *tasks[]);

/*
    Function to extract the individual tasks from request message sent to the server
*/
void extract_tasks(char *request, char *tasks[]);

/*
    Function to joint two strings
*/
char *join_strings(char dest[], char src[]);
/*to be romoved. just for debugging*/
void debug(void){
    print_msg("DEBUG", "failed");
}
#endif