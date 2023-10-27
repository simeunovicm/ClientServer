#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int userExist(char*, char*);

struct user
{
   char id[50];
   char username[100];
   char password[50];
};


struct art
{
   char id[50];
   char name[100];
   char author[50];
   char year[20];
   char status[20];
};

int userCount = 1;
struct user users[50];
int artCount = 3;
struct art arts[50];
char input[10][30];
int currentInput = 2;
char* search(char* type, char* term);
char* checkStatus(char* id);
char* reserve(char* id);
// Function designed for chat between client and server.
void doWork(int sockfd)
{
	char buff[MAX];
	char buffOut[MAX];
	char* ptr;
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);
		bzero(buffOut, MAX);
        bzero(input[0], MAX);
        bzero(input[1], MAX);
        bzero(input[2], MAX);

		read(sockfd, buff, sizeof(buff));

		if (buff[0] == '\0')
        {
            continue;
        }

        ptr = strtok(buff, "|");
        currentInput = 0;
        while(ptr != NULL)
        {
            strcpy(input[currentInput], ptr);
            ptr = strtok(NULL, "|");
            currentInput++;
        }

        bzero(buffOut, MAX);

        if(strcmp(input[0], "1") == 0) {
            userCount++;
            strcpy(users[userCount].id, userCount);
            strcpy(users[userCount].username, input[1]);
            strcpy(users[userCount].password, input[2]);

            strcpy(buffOut, "User created");
        }
        if(strcmp(input[0], "2") == 0) {
            if(userExist(input[1], input[2])) {
                strcpy(buffOut, "OK");
            }
            else {
                strcpy(buffOut, "ERROR");
            }
        }
        if(strcmp(input[0], "4") == 0) {
            strcpy(buffOut, search(input[1], input[3]));
        }

        if(strcmp(input[0], "5") == 0) {
            strcpy(buffOut, checkStatus(input[2]));
        }
        if(strcmp(input[0], "6") == 0) {
            strcpy(buffOut, reserve(input[2]));
        }

		write(sockfd, buffOut, sizeof(buffOut));
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	strcpy(users[0].id, "1") ;
	strcpy(users[0].username, "test");
	strcpy(users[0].password, "123");

	strcpy(arts[0].id, "1");
    strcpy(arts[0].name, "Art1");
    strcpy(arts[0].author, "Author1");
    strcpy(arts[0].year, "1996");
    strcpy(arts[0].status, "FREE");

    strcpy(arts[1].id, "2");
    strcpy(arts[1].name, "Art2");
    strcpy(arts[1].author, "Author2");
    strcpy(arts[1].year, "1997");
    strcpy(arts[1].status, "FREE");

    strcpy(arts[2].id, "3");
    strcpy(arts[2].name, "Art3");
    strcpy(arts[2].author, "Author3");
    strcpy(arts[2].year, "1998");
    strcpy(arts[2].status, "FREE");

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
	{
        printf("Socket successfully created..\n");
	}

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
	{
        printf("Socket successfully binded..\n");
	}


	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
	{
        printf("Server listening..\n");
	}

	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
	{
        printf("server acccept the client...\n");
	}

	doWork(connfd);

	close(sockfd);
}

int userExist(char* username, char* password) {

    int i = 0;


    for(i = 0; i < userCount; i++) {

        if(strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return users[i].id;
        }

        return 0;
    }
}

char* search(char* type, char* term) {

    int i = 0;
    char* result = NULL;
    result = (char*)calloc(250 + 1, sizeof(char));

    strcat(result, "Arts: \n");

    for(i = 0; i < artCount; i++) {

        if(strcmp(type, "ID") == 0 &&  strcmp(arts[i].id, term) != 0) {
            continue;
        }

        if(strcmp(type, "AUTHOR") == 0 && strcmp(arts[i].author, term) != 0) {
            continue;
        }

        if(strcmp(type, "NAME") == 0 && strcmp(arts[i].name, term) != 0) {
            continue;
        }

        if(strcmp(type, "YEAR") == 0 && strcmp(arts[i].year, term) != 0) {
            continue;
        }

        strcat(result, arts[i].id);
        strcat(result, "  ");
        strcat(result, arts[i].name);
        strcat(result, "  ");
        strcat(result, arts[i].author);
        strcat(result, "  ");
        strcat(result, arts[i].year);
        strcat(result, "\n");

    }

    return result;
}

char* checkStatus(char* id) {

    int i = 0;
    char* result = NULL;
    result = (char*)calloc(250 + 1, sizeof(char));

    strcat(result, "Check status: \n");

    for(i = 0; i < artCount; i++) {

        if(strcmp(arts[i].id, id) == 0) {
            strcat(result, arts[i].id);
            strcat(result, "  ");
            strcat(result, arts[i].name);
            strcat(result, "  ");
            strcat(result, arts[i].author);
            strcat(result, "  ");
            strcat(result, arts[i].year);
            strcat(result, "  ");
            strcat(result, arts[i].status);
            strcat(result, "\n");

        }

    }

    return result;
}

char* reserve(char* id) {

    int i = 0;
    char* result = NULL;
    result = (char*)calloc(250 + 1, sizeof(char));

    strcat(result, "Reserve: \n");

    for(i = 0; i < artCount; i++) {

        if(strcmp(arts[i].id, id) == 0) {
            strcat(result, "OK\n");
            strcpy(arts[i].status, "RESERVED");
        }
    }

    return result;
}
