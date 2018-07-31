// this file provide the reusable function used by both the server and the client
#ifndef UTILITY_H_
#define UTILITY_H_
//for generic functions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

//for sockets and ipc
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>


#define DEFAULT_PORT 2054     //the default port onto which bothe the server and the client listen

#define NEW_LINE {printf("\n");}            //printing newline
#define SEPARATOR ";"                       //the command separator macro

#define SOCKET_ERROR "SOCKET ERROR"         //the socket error macro
#define SEND_FAILURE "SEND FAILURE"         //send failure macro
#define RECV_FAILURE "RECEIVE FAILURE"      //receive failure macro

#define MESSAGE "MESSAGE"                   //message macro
#define ERROR "ERROR"                       //error macro
#define UNKNOWN_COMMAND "UNKNOWN COMMAND"   //unknown command macro
#define FEW_ARGS "FEW ARGUMENTS"            //the few arguments macro
#define WRONG_ARGS "WRONG ARGUMENTS"        //the wrong arguments macro


typedef struct sockaddr_in SocketAddress;       //structure to represent socket address structure

//function to load the buffer with the string 'str' character by character
void load_buffer(const char *str);

//function to pring a message to the terminal 'type' spsecifies the message category
void print_msg(char *type, char *msg);

/*
    function to check if the entered string is a number, it looks at each character in the string entered, 
    when it encounters a character that is not a number, it returns 0 (false)
    it takes a string 'str' to be checked if it is a number
    it returns 1(true) if all the characters in the string are numbers or 0 (false) if at least one of the characters in the string 
    is not a number
*/
int is_anumber(char *str);

/*
	function to sort an array of integers in ascending order (bubble sort)
	it takes an array of integers before sorting
	it returns the array after sorting in ascending order
*/
int *sort_positions(int positions_array[], int size_);

/*
	function to swap two integers
	it takes two pointers to integers and interchanges them i.e 'from' is set 'to' and 'to' is set to 'from'
*/
void swap(int *from, int *to);

/*
    function to print the socket address
    it takes the structure for the socket address and prints information about it ie <ip-address : port> combination
    and the message to describe the nature of the socket address
    it returns nothing
*/
void print_socket_address(SocketAddress socket_address, char *msg);

/*
    function to prepare / make the socket address of the destintion machine 
    it takes the machine name of the machine to receive the packets '<hostname>', the port number '<port>' on which the 
    destination machine listens and the a pointer to the socket address to be prepared '<dest_socket_addr>'.
    it returns nothing
*/
void prepare_destination_socket_address(SocketAddress *dest_socket_addr, char *hostname, int port);

/*
    function to prepare / make a socket address using any free addresses of the local computer for the local socket on 
    the DEFAULT_PORT
    it takes a pointer to the socket address to be prepared '<local_socket_addr>'
    it returns nothing
*/
void prepare_local_socket_address(SocketAddress *local_socket_addr);

/*
    Function to double the string 'str'
*/
char *string_doubel(char *str);

/*
    Function to reverse the string 'str'
*/
char *string_reverse(char *str);

/*
    Function to replace specified characters into the string 'str' at specifed positios
*/
char *string_replace(char *str, int possn[], char new_chars[], int size);

/*
    Helper Function: used to set a character at specified position in the string 'str' to the new_char
*/
char *set_char(char *str, char new_char, int position);

/*
    Function to delete the characters at specified positions
*/
char *string_delete(char *str, int positions[], int size);

/*
    Function to encrypt a word


    Helper Function: gets the letter value it corresponds to eg a = 1
    This function is used by the string_encrypt()
*/
int get_letter_value(char c);

/*
    Helper Function: gets the character which correspondes to the enterd value eg 1 = a
    this function is used by the string_encrypt()
*/
char get_letter(int value);

char *string_encrypt(char *str, char *response);

/*
    Function to deccrypt the string str
*/
char *string_decrypt(char *str, char *response);

/*to be romoved. just for debugging*/
void debug(void);
#endif