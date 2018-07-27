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

					/* service the client from here*/
					//###############################
					printf(" clients's request: %s",request);NEW_LINE;
					/* clean the plates after eating */
					bzero(request, request_size);
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

/*gc
    Function to service the client's requests
*/
char *service_client(char *request, char reply[]){
	
	char *task = (char *)malloc(BUFFER_SIZE);
	char *result = (char *)malloc(BUFFER_SIZE);
	int task_id = 1;

	task = strtok(request, ";");		//obtain the first task in the request message

	//loop through the request to pick more tasks
	while(task != NULL){

		//process the first task
		result = (char *)process_task(task, result, task_id);

		//concatnate the result on the reply message
		strcat(reply,(const char *)result);
		//concatnate the separator
		strcat(reply, SEPARATOR);

		//obtain the next task in the request message
		task = strtok(NULL, ";");
		//clean up the result before loading it with next result
		memset(result, '\0', BUFFER_SIZE);

		//go to the next task
		++task_id;

	}
	//you got to free my memory. You nolonger need it anyway
	free(task);
	free(result);

	return reply;
}

/*
    Function to process the task, eg delete, double, reverse string etc
	takes the task grepped from the request message, where to write the result 'result'
	and the task id
	returns the rsult is a well packaged format the client understands
*/
char *process_task(char *task, char *result, int task_id){

	return result;
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