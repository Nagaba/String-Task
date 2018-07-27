#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING 255

void string_double(char *str, char *response)
{
	int length = strlen(str);
	int position = 0;

	for (int count = 0; count < 2; ++count) {
		for (int i = 0; i < length; ++i, ++position)
		{
			response[position] = str[i];
		}
	}
	response[position] = '\0';
}

void string_reverse(char *str, char *response)
{
	int length = strlen(str);
	int position = length - 1;

	for (int i = 0; i < length; ++i, --position)
	{
		response[i] = str[position];
	}
	response[length] = '\0';
}

int string_split(char *str, char *delimeter, char **tokens)
{
	int length = strlen(str);
	int count = 0;
	char *token, *saveptr, *listptr;

	for (listptr = str; ; ++count, listptr = NULL)
	{
		token = strtok_r(listptr, delimeter, &saveptr);
		if (token == NULL) break;
		tokens[count] = token;
	}
	return count;
}

void string_delete(char *str, char *token_str, char *response)
{
	int length = strlen(str);
	char *tokens[24];
	int count = string_split(token_str, ", ", tokens);
	int ch = 0;

	for (int i = 0; i < length; ++i)
	{
		int in_list = 0;
		for (int x = 0; x < count; ++x)
		{
			int pos = atoi(tokens[x]) - 1;
			if (i == pos) {
				in_list = 1;
				break;
			}
		}

		if (in_list) continue;
		response[ch] = str[i];
		ch++;
	}
	response[ch] = '\0';
}

void string_replace(char *str, char *token_str, char *response)
{
	int length = strlen(str);
	char *tokens[24], *num_token[2];
	int count = string_split(token_str, ", ", tokens);

	strcpy(response, str);

	for (int i = 0; i < count; ++i)
	{
		string_split(tokens[i], "-", num_token);
		int pos = atoi(num_token[0]) - 1;
		response[pos] = num_token[1][0];
	}
}
/* taking these */

int get_letter_value(char c)
{
	if (c >= 97 && c <= 122)
		return (c - 96);
	else if (c >= 65 && c <= 90)
		return (c - 38);
	else
		return -1;
}

char get_letter(int value) 
{
	if (value < 27)
		return (value + 96);
	else if (value >= 27)		
		return (value + 38);
	else
		return '\0';
}



void string_encrypt(char *str, char *response)
{
	int length = strlen(str);
	int chvals[10];
	int pos = 0;

	for (int i = 0; i < length; ++i) {
		int count = 0, mid = 0;
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
	response[pos] = '\0';
}

void string_decrypt(char *str, char *response)
{
	int length = strlen(str);
	int spos = 0, rpos = 0;

	while(spos < length)
	{
		int hops = 0;
		int chval;
		while (isdigit(str[spos]))
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
	
	response[rpos] = '\0';

}


//int main()
//{
//
//	char str[MAX_STRING], token_str[MAX_STRING], response[MAX_STRING], command[16];
//	strcpy(str, "cOolZ");
//	string_encrypt(str, response);
//	printf("%s\n", response);
//	string_decrypt(response, token_str);
//	printf("%s\n", token_str);
	// char **tokens;

	// while (1)
	// {
	// 	printf(">>> ");
	// 	scanf("%s", command);

		// if (strcmp(command, "double") == 0)
		// {
		// 	scanf("%s", str);
		// 	string_double(str, response);
		// 	printf("String after double: %s\n", response);
		// }
		// else if (strcmp(command, "rev") == 0)
		// {
		// 	scanf("%s", str);
		// 	string_reverse(str, response);
		// 	printf("String after reverse: %s\n", response);
		// }
		// else if (strcmp(command, "delete") == 0)
		// {
		// 	scanf("%s", str);
		// 	fgets(token_str, MAX_STRING, stdin);
		// 	string_delete(str, token_str, response);
		// 	printf("String after delete: %s\n", response);

		// }
		// else if (strcmp(command, "replace") == 0)
		// {
		// 	scanf("%s", str);
		// 	fgets(token_str, MAX_STRING, stdin);
		// 	string_replace(str, token_str, response);
		// 	printf("String after replace: %s\n", response);

		// }
		// else if (strcmp(command, "exit") == 0) break;

// 	}

//	return 0;
//}

/*
int m()
{
    int sockfd, bytes_read;
    struct sockaddr_in server_addr;
    struct hostent *server;

    // create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &server_addr, sizeof(server_addr));

    // setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);


    // Connect to the server
    printf("Connecting to String Task Server...\n");
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR: Unable to connect to server");
        exit(1);
    }
    printf("Connected\n");
    printf("\nEnter command: ");

    bzero(buffer, 1024);
    fgets(buffer, 1023, stdin);

    int n = send(sockfd, buffer, strlen(buffer), 0);

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    bytes_read = recv(sockfd, buffer, 1023, 0);


    if (bytes_read == -1) {
        perror("Cannot read from the server");
        exit(1);
    }

    buffer[bytes_read] = '\0';
    printf("%s", buffer);



    close(sockfd);


}
*/