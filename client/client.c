/*
    The main client program that implements the functionality of the string task program
*/
#include "client.h"

char buffer[BUFFER_SIZE];                   //the buffer onto which to write the request
char *reply = NULL;                                //the buffer onto which to load the reply


SocketAddress client_soc_addr;             //the socket address of the client
SocketAddress server_soc_addr;             //the socket address for the server

int snder_length = sizeof(client_soc_addr);  //the size of the client socket address
int client_socket;                           //client's socket file descriptor
const char *terminal_string;                //string typed at the terminal

int main(int argc, char const *argv[])
{

    /* check if the arguments are passed correctly */
    if(argc <= 1){
        NEW_LINE;
        print_msg(ERROR, "Usage> name_of_cleint host_name");
        exit(EXIT_FAILURE);         //exit the program if the host name is not specifed
    }
    //if the destination / host is provided then run the client
    else if(argc == 2){
        /* do what you want here */
       client_run(argv); 
    }
    else {
        NEW_LINE;
        print_msg(ERROR,"Usage> name_of_client host_name");
        print_msg("HELP", "run the client by putting the name of the server machine as an argument.");
    }
    return 0;
}

//function to  run the main client program 
void client_run(const char **args){

    reply = malloc(2*BUFFER_SIZE);
    printf(" Initializing String Task client......");
    NEW_LINE;
    /****create the client socket file descriptor */
    print_msg(MESSAGE, "Creating client socket..........");
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        print_msg("SOCKET_ERROR", "Could not create client socket.");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    print_msg("SUCCESS", "Client socket created successfully");

    /* prepare the socket address in structure for the receiver / server */
    memset((char *)&server_soc_addr, 0, sizeof(server_soc_addr));
    prepare_destination_socket_address(&server_soc_addr, (char *)args[1], DEFAULT_PORT);
    print_socket_address(server_soc_addr,"Destination's socket address");

    /* connect to the server */
    if(connect(client_socket, (struct sockaddr *)&server_soc_addr, sizeof(struct sockaddr)) == -1){
        print_msg("CONNECTION ERROR", "Failed to connect to remeote server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    /* send and receive once the connection is established */
    terminal_string = malloc(BUFFER_SIZE);     //allocate memory to the input string at the terminal
    
    while(true){
        
        /* print prompt and get user input*/
        terminal_string = prompt((char *)terminal_string);

        if(terminal_string == NULL){
            print_msg(UNKNOWN_COMMAND, (char *)terminal_string);
            continue;
        }
        
        else if (strncmp(terminal_string, "help", 4) == 0)
        {
            /* print help menu */
            print_help();
        }
        
        else if (strncmp(terminal_string, "exit", 4) == 0)
        {
            /* tell the server first before you go away */
            if(send(client_socket,terminal_string, sizeof(buffer), 0) == -1){
                print_msg(ERROR, "it seems the server is off");
            }
            /* exit the program */
            NEW_LINE;
            print_msg("CLIENT CLOSED","Bye.....");
            close(client_socket);           //close the client socket file descripter
            exit(EXIT_SUCCESS);             //exit the program
        }
        else{
            /*user can now exechange with the server*/
            //#########################################
            if(send(client_socket,terminal_string, sizeof(buffer), 0) == -1){
                print_msg(SEND_FAILURE, "Failed to send the previous packet to the server");
                continue;
            }
            print_msg("SEND SUCCESS", "Task(s) sent to the server");

            /* receive the reply */
            if(recv(client_socket,reply, sizeof(reply), 0) == -1){
                print_msg(RECV_FAILURE, "whooops! failed to recive reply from server!");
                continue;
            }
            /* print the reply  */
            else{
                print_msg("RESULT", reply);
                //print_reply(reply);
            }
            //memset(reply, '\0', sizeof(reply)); //clean up the house
            free(reply);
        }
    }
    free((char *)terminal_string);      //free the memory allocated to terminal string
    close(client_socket);
}

//function to prompt the user to enter the string from the terminal
const char *prompt(char buff[]){
    NEW_LINE;
    printf(" Enter a command> ");
    fgets(buff, 1000, stdin);
    NEW_LINE;
    buff = strtok(buff, "\n");  //remove the new line character which is appended implicitly by the fgets() function

    return (const char *)buff;
}

/*
    Function to print the reply from the server to the client's terminal
*/
void print_reply(char *reply){
    printf("%s \n", reply);
    char *result = NULL;

    result = strtok(reply, SEPARATOR);      //extract the firts result
    printf(" %s\n", result);
    // while(result != NULL){
    //     print_msg("RESULT", result);
    //     debug();
    //     result = strtok(NULL, SEPARATOR);
    //     debug();
    // }

    //clean up the result container before loading it again
    memset(result, '\0', sizeof(result));
    
}

/*
    Function to print the help menu showing the list of permitted commands 
    the string Task program can accept
*/
void print_help(void){
    NEW_LINE
    print_msg("#","************************************ HELP *************************************#");
    printf("  below is the list of supported commands, the words in < and > are any arguments\n");
    print_msg(" tasks","the permited tasks are;");NEW_LINE;
    print_msg(" DOUBLE"," double <word>;");
    print_msg(" REVERSE", "reverse <word>;");
    print_msg(" ENCRYPT", "encrypt <word>;");
    print_msg(" DECRYPT", "decrypt <word>;");
    print_msg(" DELETE", " delete <word> <posn1, posn2,......, posnn>;");
    print_msg(" REPLACE", "replace <word>  <posn1-char1, posn2-char2,.....,posnn-charn>;");
    print_msg(" HELP","   help\t\t {to see the help menu}");
    print_msg(" EXIT", "   exit\t\t {to exit the program}");
    
    print_msg(" note","the format of the request message having more than on task is;");
    printf("\t <task1;task2;...............;taskn>\n");
    print_msg("  eg","double sara;reverse chair;delete house 1,2,3;replace boy 1-p,2-g;encrypt doop");

    print_msg("#","*******************************************************************************#");
    NEW_LINE;
}