/* this file provides the definitions to the function headers in utility.h */

#include "utility.h"


/*
    function to print the socket address
    it takes the structure for the socket address and prints information about it ie <ip-address : port> combination
    and the message to describe the nature of the socket address
    it returns nothing
*/
void print_socket_address(SocketAddress socket_address, char *msg){
    NEW_LINE;
	printf(" %s %s:%d",msg,inet_ntoa(socket_address.sin_addr), ntohs(socket_address.sin_port));
    NEW_LINE;
}

/*
    function to prepare / make the socket address of the destintion machine 
    it takes the machine name of the machine to receive the packets '<hostname>', the port number '<port>' on which the 
    destination machine listens and the a pointer to the socket address to be prepared '<dest_socket_addr>'.
    it returns nothing
*/
void prepare_destination_socket_address(SocketAddress *dest_socket_addr, char *hostname, int port){

    struct hostent *host;               //the host's ip addresse

    host = gethostbyname(hostname);     //get the host's ip address from its  DNS name <hostname>

    /* check if the valid host name is supplied otherwise close the program */
    if(host == NULL){
        print_msg("UNKNOWN_HOST_NAME", hostname);
        NEW_LINE;
        print_msg("HELP","Enter the correct host name name");
        exit(EXIT_FAILURE);
    }

    dest_socket_addr->sin_family = AF_INET;             //set the network domain to IPv4
    dest_socket_addr->sin_addr = *(struct in_addr *)(host->h_addr);
    dest_socket_addr->sin_port = htons(port);           //set the port
}

/*
    function to prepare / make a socket address using any free addresses of the local computer for the local socket on 
    the DEFAULT_PORT
    it takes a pointer to the socket address to be prepared '<local_socket_addr>'
    it returns nothing
*/
void prepare_local_socket_address(SocketAddress *local_socket_addr){

    local_socket_addr->sin_family = AF_INET;                    //set the network domain to IPv4
    local_socket_addr->sin_port = htons(DEFAULT_PORT);          //set the port to the DEFAULT_PORT
    local_socket_addr->sin_addr.s_addr = htonl(INADDR_ANY);     //assign any free addreess of the local machine available
}

/* 	function to print a message at the terminal
    it takes a pointer to the character string to be printed to the screen and the type of the message
    it ruturns nothing
*/
void print_msg(char *type, char *msg){
    
    printf(" %s: %s",type, msg);
    NEW_LINE;
}

/*
	function to swap two integers
	it takes two pointers to integers and interchanges them i.e 'from' is set 'to' and 'to' is set to 'from'
*/
void swap(int *from, int *to){
	int temp;			//temporary holder for the value to be swapped
	temp = *from;
	*from = *to;
	*to = temp;
}

/*
	function to sort an array of integers in ascending order (bubble sort)
	it takes an array of integers before sorting
	it returns the array after sorting in ascending order
*/
int *sort_positions(int positions_array[], int size_){

	int outer_counter;		//passes through the positions_array[]
	int swap_counter;		//for comparing and swapping any two positions

	for(outer_counter = 0; outer_counter < size_; ++outer_counter){	//go through the entire positions_array

		for(swap_counter = 0; swap_counter < size_; ++swap_counter){

			if(positions_array[swap_counter] > positions_array[outer_counter]){
				swap(&positions_array[swap_counter], &positions_array[outer_counter]);	//swap the two numbers if the next value in the positions_array is greater than the previous
			}
		}
	}
	return positions_array;
}



/*
    Function to double the string 'str'
*/
char *string_doubel(char *str){
    /* check if the string is empty and return NULL if true */
    if(*str == 0){
        return NULL;
    }
    /* check if the string is NULL and return NULL */
    if(str == NULL){
        return NULL;
    }
    char *str_duplicate = malloc(sizeof(str));
    str_duplicate = strdup(str);  //create a duplicate copy of the provided string

    strcat(str, (const char *)str_duplicate);   //concatenate it to the previous str
    
    free(str_duplicate);        //relinquish the memory 
    return str;
}

/*
    Function to reverse the string 'str'
*/
char *string_reverse(char *str){
    //check for null
	if(str == 0){
		return NULL;
	}
	//skip empty string
	if(*str == 0){
		return NULL;
	}

	//obtain the range of the string
	char *beginning = str;
	char *end = beginning + strlen(str) - 1; //remove the null terminater '\0'
	char temp;	//for swapping

	//as long as end is greater than beginning continue to reverse
	while(end > beginning){

		//swap
		temp = *beginning;
		*beginning = *end;
		*end = temp;

		//go to the next position
		++beginning;
		--end;
	}

	return str;
}

/*
    Helper Function: used to set a character at specified position in the string 'str' to the new_char
*/
char *set_char(char *str, char new_char, int position){

    char buffer[strlen(str)];		//create a buffer of the same size as the string where u want to store the characters that you need
	int counter = 1;                //counter to go through the entire string 'str'
	int pos = 0;                    //tracks the position in the buffer
	
	/*	check if the position is less than or greater than the number of characters in the str 
		i.e. strlen(str) >= position 
	*/
	
	if(position > strlen(str)){
		return NULL;
	}
	else if(position < 0){
		return NULL;
	}
	
	/* loop through the str and identify the character that is to be replaced */
	while(*str != '\0'){
		if(counter != position){
			buffer[pos] = *str;	//add the character to the buffer 
		}
		else{
			buffer[pos] = new_char;		//replace that character at the required position
		}

		*str++;
		pos++;
		counter++;
	}
	buffer[pos] = '\0';		//terminate the buffer
	
	strcpy(str,buffer);		//copy the buffer to the str
	
	return str;
}


/*
    Function to replace specified characters into the string 'str' at specifed positios
*/
char *string_replace(char *str, int possn[], char new_chars[], int size){
    /* check that the string 'str' is not null  */
	if(str == NULL){
		return NULL;
	}
	
	int pos = 0;	//position in a string str
	
	while(pos < size){
		str = set_char(str, new_chars[pos], possn[pos]);	//replace that specific char
		++pos;
	}
	return str;
}

/*
    Function to delete the characters at specified positions
*/
char *string_delete(char *str, int positions[], int size){
    if(str == NULL){
		return NULL;		//if the string 'str' is empty return NULL
	}

	int length = strlen(str);	//number of characters in the  string 'str'
	char buffer[length];		//create a buffer of the same size as the string where u want to store the characters that you need

	int index = 0;				//counter to loop through the buffer
	for(int i = 1; i <= length; ++i){
		bool keep = true;		//boolean to use to decide whether the character at the specified position is needed or not
		for(int j = 0; j < size; ++j){
			if(i == positions[j]){
				keep = false;	//set keep to false and skip this character if it is at the unwanted position
				break;
			}
		}
		//if the character at the position i in the str is wanted then copy it to the buffer
		if(keep){
			buffer[index] = str[i-1];
			++index;
		}
	}

	buffer[index] = '\0';			//terminate the buffer
	strcpy(str, buffer);			//copy the buffer back to the string 'str'

	return str;
}

/*
    Function to encrypt a word
*/

/*
    Helper Function: gets the letter value it corresponds to eg a = 1
    This function is used by the string_encrypt()
*/
int get_letter_value(char c)
{
	if (c >= 97 && c <= 122)
		return (c - 96);
	else if (c >= 65 && c <= 90)
		return (c - 38);
	else
		return -1;
}

/*
    Helper Function: gets the character which correspondes to the enterd value eg 1 = a
    this function is used by the string_encrypt()
*/
char get_letter(int value) 
{
	if (value < 27)
		return (value + 96);
	else if (value >= 27)		
		return (value + 38);
	else
		return '\0';
}

char *string_encrypt(char *str, char *response)
{
	int length = strlen(str);
	int chvals[10];
	int pos = 0;    //position in the respinse string

    //loop through the string str character by character
	for (int i = 0; i < length; ++i) {
		int count = 0;
		int chval = get_letter_value(str[i]);

		while (chval > 9) {
			chvals[count] = chval;
			chval -= 9;
			count++;
		}

		if (count < 1) {
			response[pos++] = toupper(get_letter(chval));
			continue;
		}

		for (int n = 0; n < count; ++n) {
			response[pos++] = (chvals[n] / 10) + '0';

			if (n == count - 1) {
				response[pos++] = toupper(get_letter(chval));
			}
			response[pos++] = tolower(get_letter(chvals[n] - 1));
		}

		for (int n = count-1; n >= 0; --n) {
			response[pos++] = (chvals[n] % 10) + '0';
		}

	}
	response[pos] = '\0';   //terminate the response

    return response;
}

/*
    Function to deccrypt the string str
*/
char *string_decrypt(char *str, char *response){
	int length = strlen(str);
	int spos = 0, rpos = 0;

	while (spos < length)
	{
		int hops = 0;
		int chval;
		while(isdigit(str[spos]))
		{
			spos += 2;
			hops++;
		}
		if (islower(str[spos]) && isupper(str[spos - 1]))
			chval = get_letter_value(tolower(str[spos - 1]));
		else if (isupper(str[spos]))
			chval = get_letter_value(tolower(str[spos]));
		else break;

		chval += (hops * 9);

		response[rpos++] = get_letter(chval);
		spos += hops + 1;

	}
	
	response[rpos] = '\0';  //terminate the response

    return response;
}

/*to be romoved. just for debugging*/
void debug(void){
    print_msg("DEBUG", "failed");
}