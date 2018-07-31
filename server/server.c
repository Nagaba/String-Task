/*
	The main server program that implements the functionality of the string task program
*/

#include "server.h"


char request[BUFFER_SIZE];	//request message from the client
char buffer[BUFFER_SIZE];	//the buffer to write to the request from the client
char *reply;				//the response to send back to the client

SocketAddress client_soc_addr;	//socket address of the client
SocketAddress server_soc_addr;	//socket address of the server
int server_socket, new_socket;	//listen on server_socket and for new connections on new_socket
int server_sa_len;				//the size of the server socket address
struct sigaction signal_action;	//signal action handler
pid_t child_process_id;			//the child process id

//int number_of_tasks = 0;		//this variable is for the total number of tasks commited to the server

/*
	################# MAIN ##################
*/
int main(int argc, char const *argv[])
{
	/* run the server */
	server_run();
	return 0;
}
/*
	##########################################
*/

/* 
    function to handle the signal incase of new incoming  connections
    it takes the socket file descripter attempting to connect
    it returns nothing
*/
void signal_chld_handler(int soc){

    
    while(wait(NULL) > 0){
        /* wait for connection signal*/
    }
    
}

/*
    Function to run the main server routine
*/
void server_run(void){
	NEW_LINE;
    printf(" Initializing String Task server......");
    NEW_LINE;


    int accepted = 1;							//for connection acceeptance
    server_sa_len = sizeof(struct sockaddr_in);
	size_t request_size = sizeof(request);		//size of the request message
	reply = (char *)malloc((2 * BUFFER_SIZE));	//allocate memory to the reply message
	
	

	/* create the server socket file descripter */
	print_msg( MESSAGE,"Creating server socket.....");
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        print_msg(SOCKET_ERROR, "Could not create server socket.");
        close(server_socket);   
        exit(EXIT_FAILURE);
    }
    print_msg("SUCCESS", "Server socket created successfully.");

	/* set the options on the socket file descripter */
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &accepted, sizeof(int)) == -1){
        print_msg(SOCKET_ERROR, "Could not set server socket option");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

	/* prepare the server socket address */
	prepare_local_socket_address(&server_soc_addr);
    bzero(&(server_soc_addr.sin_zero), 8);              //zero the rest of the struct
	print_socket_address(server_soc_addr, "The server socket address is");

	/* bind the server socket to the designated port (DEFAULT_PORT) */
	print_msg(MESSAGE,"Binding.......");
    if(bind(server_socket, (struct sockaddr *)&server_soc_addr, sizeof(struct sockaddr)) == -1){
        print_msg("BIND ERROR", "Server failed to bind the port to the socket address");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    print_msg("SUCCESS", "Server successfully bound the port to the socket address");
    NEW_LINE;
	
	/* set server to listen for incoming connections */
	if(listen(server_socket, BACKLOG) == -1){
        print_msg(ERROR, "Server failed to listen for incoming connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

	/* harvest all the pending processes */
	signal_action.sa_handler = signal_chld_handler;
    sigemptyset(&signal_action.sa_mask);    //initailize the signal set with with all signals excluded from the set
    signal_action.sa_flags = SA_RESTART;

    if(sigaction(SIGCHLD, &signal_action, NULL) == -1){
        print_msg("SIGNAL FAILURE", "Signal handleing failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    print_msg("SUCCESS", "Server set up and is ready to accept connection...");

	/* accept the connection and start executing the tacks */
	//loop to accept more connections
	while(true){
		/* accept the connection on success, continue listenning for in coming connections on failure */
        if((new_socket = accept(server_socket, (struct sockaddr *)&client_soc_addr, &server_sa_len)) == -1){
			
			exit(EXIT_FAILURE);
        }

		/* create a child process to handle the client's request*/
		if((child_process_id = fork()) == 0){
			close(server_socket);	//close the listening file descripter, we don't need it anymore
			print_socket_address(client_soc_addr, "SERVER: connection accepted from ");
			print_msg("SUCCESS", "process created to service the client");
			NEW_LINE;

			/* receive the request from the client */
			while(true){
				
				if(recv(new_socket, request,request_size, 0) == -1){
					print_msg(RECIEVE_FAILURE, "failed to receive the client's request!");
					break;	//client left
				}
				/* if the client wants to exit*/
				if(strcmp(request, "exit") == 0){
					print_socket_address(client_soc_addr, "SERVER: disconnected from");
					break;
				}
				else{
					
					//make the tasks initially nulls
					char *_tasks[10];
					for(int i = 0; i < 10; ++i){
						_tasks[i] = NULL;
					}
					
					/* service the client from here*/
					//###############################
					printf(" clients's request: %s",request);NEW_LINE;

					extract_tasks(request,_tasks);
					service_client(_tasks);
					//printf("\n %s",service_client(_tasks));

					/* clean the plates after eating */
					bzero(request, request_size);
					memset(_tasks, '\0',10);	//clean the requests container
				}
			}
		
	
			free(reply);		//relinquish the memory of reply after sending the reply
			close(new_socket);	//close the client's socket file descripter after when we are done serviceing its requests
		}
		else{
			print_msg("PROCESS FAILURE", "failed to create child process to service the client");
			close(new_socket);
		}
		close(new_socket);	//the parent process does not this either
	}//end the main while
}

/*
    Function to service the client's requests, it takes the client's tasks to process
*/
char *service_client(char *tasks[]){
	
	char *result = (char *)malloc(BUFFER_SIZE);
	int task_id = 1;

	int task_cntr = 0;	//counter to loop through the *tasks[]

	char *task_result = (char *)malloc(BUFFER_SIZE);
	//loop through the *tasks[] to pick each task
	while(tasks[task_cntr] != NULL){

		//process the first task
		task_result = (char *)process_task(tasks[task_cntr], result, task_id);
		
		//concatnate the result on the reply message
		strcat(reply,(const char *)task_result);
		//join_strings(reply, task_result);
		//concatnate the separator
		strcat(reply, SEPARATOR);

		//go to the next task
		++task_id;
		++task_cntr;

		//clean up the result before loading it with next result
		memset(result, '\0', BUFFER_SIZE);
		memset(task_result, '\0', sizeof(task_result));

	}
	printf(" %s\n",reply);NEW_LINE;
	
	return reply;
}

/*
    Function to process the task, eg delete, double, reverse string etc
	takes the task grepped from the request message, where to write the result 'result'
	and the task id
	returns the rsult in a well packaged format the client understands
*/
char *process_task(char *task, char *result, int task_id){

	char *word = (char *)malloc((BUFFER_SIZE / 2)); 
	char *_task = malloc((strlen(task) + 1)*sizeof(char));

	strcpy(_task, task);

	/* check to see if the received task is null, return null if command == null*/
    if(task == NULL){
        return NULL;
    }

	/* otherwise perform the task */

	//for doubling the word
	if(strncmp(_task, "double", 6) == 0){
        /* implement the  double word task double <word> */
		char *doubled_word = NULL;
        strtok(_task, " ");          //discard the double keyword in the command
		
        /* ensure that the argument <word> is provided */
        if((word = strtok(NULL, " ")) == NULL){   //extract the word from the command typed
        
            //few arguments provided for command double <word>
			result = package_error(FEW_ARGS, task_id, "you did not provide an argument to double !", buffer);
            return result;
        }     
		else{
			
			doubled_word = string_doubel(word);								//double the word
			result = package_result(task_id, task, doubled_word, buffer);	//package the result
		}

    }
	//for reversing the word
	else if(strncmp(_task, "reverse", 7) == 0){
		/* implement the reverse <word> command */
		strtok(_task, " ");					//discard the reverse keyword from the command typed
		char *reversed_word = NULL;
		/* ensure that the argument <word> is passed to the command reverse */
		if((word = strtok(NULL, " ")) == NULL){ //extract the word from the command
			//few arguments provided for command double <word>
			result = package_error(FEW_ARGS, task_id, "you did not provide an argument to reverse !", buffer);
            return result;
		}
		else{

			reversed_word = string_reverse(word);							//reverse the word
			result = package_result(task_id, task, reversed_word, buffer);	//package result
		}
	}
	//for deleting characters in a word
	else if(strncmp(_task, "delete", 6) == 0){
		/* implement the delete <word> <posn1, posn2,......, posnn>; command */
		strtok(_task, " ");           	//discard the delete keyword from the command

        char *postions_string = NULL;			//the string that contains the positions to delete
		char *new_word = NULL;					//the word after deleting some characters

		/* extract the word and ensure it was provided */
		if((word = strtok(NULL, " ")) == NULL){
			result = package_error(FEW_ARGS, task_id, "you didn't give a word to delete from!", buffer);
			return result;
		}
		else{
			/* extract the positins string */
			if((postions_string = strtok(NULL, " ")) == NULL){
				result = package_error(FEW_ARGS, task_id, "you didn't give the positions to delete!", buffer);
				return result;
			}
			else{

				int index = 0;	//index to go thru the positions[]
				char *positn = NULL;	//positon to extract from the positions_string
				int positions[strlen(postions_string)];	//array to contain the positions to delete (these will be ints)

				positn = strtok(postions_string, ",");	//extract the first position (this is still in string form)

				while(positn != NULL){
					/* convert the string position to an int */
					if(is_anumber(positn)){
						positions[index] = atoi(positn);
						++index;
					}
					else{
						result = package_error(WRONG_ARGS, task_id, "the positions provided are not numbers!", buffer);
						return result;
					}
					positn = strtok(NULL, ",");	//pick another position
				}//end while

				/* do the deletion of the specified characters */
				int pos_size = (int)(sizeof(positions) / sizeof(positions[0]));	//get the number of positions to delete
				sort_positions(positions, pos_size);	//sort the positions array

				new_word = string_delete(word, positions, pos_size);	//deletet the characters
				result = package_result(task_id, task, new_word, buffer);

			}//end else
		}
	}
	//for replacing characters in the word
	else if(strncmp(_task, "replace", 7) == 0){
		/* implement the replace <word>  <posn1-char1, posn2-char2, ...... posnn-charn>; command */
		strtok(_task, " ");		//discard the replace keyword from the command

		char *new_chars_and_pos = NULL;	//the string to contain the position-char combinations
		char *new_char_pos = NULL;		//the string that contains pos-char combination
		char *new_word = NULL;			//the word after replacing the characters at the specified locations

		/* extract the word and ensure it was provided */
		if((word = strtok(NULL, " ")) == 0){
			result = package_error(FEW_ARGS, task_id, "you didn't give a word to replace from!", buffer);
			return result;
		}
		else{
			/* extract the new_chars_and_positions string */
			if((new_chars_and_pos = strtok(NULL, " ")) == 0){
				result = package_error(FEW_ARGS, task_id, "you didn't specify the new characters!", buffer);
				return result;
			}
			else{
				int number_of_elements = (int)((strlen(new_chars_and_pos) + 1) / 4); //the number of new characters to replace
            	char new_chars[number_of_elements];                                 //the array to contain the new characters (the new char extracted form <pos-char> eg "h")
            	int positions[number_of_elements];                                  //the position extracted form <pos-char> eg "3"

				/* obtain the individual segments of the position and new char combination ie <pos-char> */
            	char pos_char_tokens[number_of_elements][3];
            	int char_counter,index = 0;

				new_char_pos = strtok(new_chars_and_pos, ",");	//extract the first pos-char combination
				/* loop to obtain more pos-char combinations */
				while(new_char_pos != NULL){
					for(char_counter = 0; char_counter < 3; ++char_counter){
						pos_char_tokens[index][char_counter] = new_char_pos[char_counter];
					}
					pos_char_tokens[index][char_counter] = '\0';	//terminate the pos-char combination

					new_char_pos = strtok(NULL, ",");				//extract the next pos-char combination
					++index;
				}//end while

				/*load the new_chars[] array and the positions[] array with their respective contents*/
				index = 0;
				char temp[2];	//temporary buffer to hold the <pos>
				while(index < number_of_elements){
					//check if the <pos> is a number
					if(isdigit(pos_char_tokens[index][0])){
						temp[0] = pos_char_tokens[index][0];	//pick the pos
						temp[1] = '\0';	//terminate the temp buffer

						positions[index] = atoi(temp); //convert the "<pos>" to an integer and load it to the positions[] array
						new_chars[index] = pos_char_tokens[index][2];	//add the char to the new_chars[]
					}
					else{
						result = package_error(WRONG_ARGS, task_id, "you have provided unsupported arguments!", buffer);
						return result;
					}

					++index;
				}//end while

				/* do the replacements of the characters in the word */
				new_word = string_replace(word, positions, new_chars, number_of_elements);
				result = package_result(task_id, task, new_word, buffer);
			}
		}

	}
	//for encrypt command
	else if(strncmp(_task, "encrypt", 7) == 0){
		/* implement the encrypt <word>; command */
		strtok(_task, " ");		//discard the encrypt keyword form the command

		char *encrypted_str = NULL;
		char *encrypted = malloc(BUFFER_SIZE);	//the string to contain the encrypted string

		/* ensure that the argument <word> is passed to the command encrypt */
		if((word = strtok(NULL, " ")) == NULL){
			//few arguments provided for command encrypt <word>
			result = package_error(FEW_ARGS, task_id, "you didn't provide an argument to encrypt !", buffer);
            return result;
		}
		else{

			/* do the encryption on the string */
			
			encrypted_str = string_encrypt(word, encrypted);
			result = package_result(task_id, task, encrypted_str, buffer);
			free(encrypted);

		}

	}
	//for the decrypt command
	else if(strncmp(_task, "decrypt", 7) == 0){
		/* implement the decrypt <word>; command */
		strtok(_task, " ");		//discard the decrypt keyword from the command

		char *decrypted_str = NULL;
		char *decrpt = malloc(BUFFER_SIZE);

		/* ensure that the argument <word> is passed to the command decrypt */
		if((word = strtok(NULL, " ")) == NULL){
			//few arguments provided for the command decrypt
			result = package_error(FEW_ARGS, task_id, "you didn't provide an argument to decrypt!", buffer);
			return result;
		}
		else{
			decrypted_str = string_decrypt(word, decrpt);
			result = package_result(task_id, task, decrypted_str, buffer);
			free(decrpt);
		}
	}
	//for unknow command
	else{
		result = package_error(UNKNOWN_COMMAND, task_id, "the command is not supported!", buffer);
		return result;
	}
	memset(word, '\0', sizeof(word));		//clean the word
	return result;
}

/*
    Function to extract the individual tasks from request message sent to the server
*/
void extract_tasks(char *request, char *tasks[]){

	int task_counter = 0;		//counter to loop through the tasks

	char *atask = strtok(request, ";");	//extract the first task

	while(atask != NULL){
		
		tasks[task_counter] = malloc(BUFFER_SIZE);	//allocate memory for the next task in the requests

		tasks[task_counter] = atask;

		atask = strtok(NULL, ";");				//get another task

		task_counter++;
		
	}
}

/*
    Function to pakage the result obtained after processing the task in 
    the format that the client understands.
*/
char *package_result(int task_id, char *task, char *result, char buffer[]){

	sprintf(buffer, "task_id %d : %s is %s", task_id, task, result);
	return buffer;
}

/*
    Function to pakage the error obtained after processing the task and 
    identifies an error in the command, in the format that the client understands.
*/
char *package_error(char *error, int task_id, char *err_msg, char buffer[]){
	sprintf(buffer, " %s for task_id %d: %s",error, task_id, err_msg);
	return buffer;
}

/*
    Function to joint two strings together
*/
char *join_strings(char dest[], char src[]){

	int cntr = 0;
	int size_of_dest = strlen(dest);
	while(src[cntr] != '\0'){
		dest[size_of_dest + cntr] = src[cntr];
		cntr++;
	}
	dest[size_of_dest + cntr] = '\0';

	return dest;
}
/*
    function to check if the entered string is a number, it looks at each character in the string entered, 
    when it encounters a character that is not a number, it returns 0 (false)
    it takes a string 'str' to be checked if it is a number
    it returns 1(true) if all the characters in the string are numbers or 0 (false) if at least one of the characters in the string 
    is not a number
*/

int is_anumber(char *str){
    int counter = 0;
    int isDigit = 0;
	/*	loop through the string 'str' and if a character at any position is not a number return false (0)
		otherwise if all the characters in the str are numbers return true (1) 
	*/
	do{
		if(isdigit(*str)){
			isDigit = 1;		//the char at this position is a number (true)
		}
		else{
			isDigit = 0;		//the char at this position is not a number (false)
		}
		*str++;
		++counter;
	}while((isDigit == 1) && (*str != '\0'));	

    return isDigit;
}
/* to be cleaned */
// #define PORT 2056
/*
void process_task(char *message, char *response)
{
	char str[MAX_STRING], token_str[MAX_STRING], result[MAX_STRING], command[16];
	char **tokens;
	char *messageptr = message;
	int bytes_read = 0;

	sscanf(messageptr, "%s%n", command, &bytes_read);
	messageptr += bytes_read;

	if (strcmp(command, "double") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		string_double(str, result);
		sprintf(response, "String after double: %s\n", result);
	}
	else if (strcmp(command, "rev") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		string_reverse(str, result);
		sprintf(response, "String after reverse: %s\n", result);
	}
	else if (strcmp(command, "delete") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		
		sscanf(messageptr, "%[^\n]", token_str);
		string_delete(str, messageptr, result);
		sprintf(response, "String after delete: %s\n", result);
		
	}
	else if (strcmp(command, "replace") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		
		sscanf(messageptr, "%[^\n]", token_str);
		string_replace(str, token_str, result);
		sprintf(response, "String after replace: %s\n", result);
		
	}
	else if (strcmp(command, "encrypt") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		
		string_encrypt(str, result);
		sprintf(response, "String after encrypt: %s\n", result);
	}
	else if (strcmp(command, "decrypt") == 0)
	{
		sscanf(messageptr, "%s%n", str, &bytes_read);
		messageptr += bytes_read;
		
		string_decrypt(str, result);
		sprintf(response, "String after decrypt: %s\n", result);
	}
}

int m()
{
    // /* 	servers BLAB
    //  * 	B - Bind to socket
    //  * 	L - Listen for connections
    //  * 	A - Accept connections
    //  * 	B - Begin talking (recv, send)
    //  *

	char message[MAX_STRING], response[MAX_STRING];
	int serversock, clientsock, bytes_read;
	socklen_t client_length;
	struct sockaddr_in server_addr;

	// create server socket
	serversock = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &server_addr, sizeof(server_addr));
	printf("Starting String Task Server...\n");

	// setup server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// bind to a port
	if (bind(serversock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("Error: Unable to bind to port");
		exit(1);
	}

   // Listen for connections
	listen(serversock, 5);
	printf("Listening on port %d\n", PORT);

	while (1)
	{
		struct sockaddr_in client_addr;
        // Accept Connections
		client_length = sizeof(client_addr);
		clientsock = accept(serversock, (struct sockaddr *)&client_addr, &client_length);

       // Begin talking now
		bzero(message, MAX_STRING);
		bzero(response, MAX_STRING);

		bytes_read = recv(clientsock, message, MAX_STRING - 1, 0);
		process_task(message, response);

		send(clientsock, response, strlen(response), 0);

		close(clientsock);
	}
	
}
*/