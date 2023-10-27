#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 150
#define PORT 8080
#define SA struct sockaddr

void printMenu();


int userLogedIn = 0;
char userID[30];

void doWork(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
        bzero(buff, sizeof(buff));
		printMenu();

		int option = 0;
        scanf("%i", &option);

        if(userLogedIn == 0) {

            switch(option) {
                case 1: {
                    char username[30];
                    char password[30];
                    printf("Enter username: ");
                    scanf("%s", &username);
                    printf("Enter password: ");
                    scanf("%s", &password);

                    strcat(buff, "1|");
                    strcat(buff, username);
                    strcat(buff, "|");
                    strcat(buff, password);

                    break;
                }
                case 2: {

                    char username[30];
                    char password[30];
                    printf("Enter username: ");
                    scanf("%s", &username);
                    getchar();
                    printf("Enter password: ");
                    scanf("%s", &password);
                    getchar();
                    strcat(buff, "2|");
                    strcat(buff, username);
                    strcat(buff, "|");
                    strcat(buff, password);

                    break;
                }
                default: printf("Wrong option\n");
            }
        }
        else {
            switch(option) {
                case 1: {
                    strcat(buff, "3|");
                    strcat(buff, userID);
                    userLogedIn = 0;
                    break;
                }
                case 2: {

                    int searchOption = 0;
                    printf("1. All\n");
                    printf("2. ID\n");
                    printf("3. Name\n");
                    printf("4. Year\n");
                    printf("5. Author\n");

                    printf("Enter option: ");
                    scanf("%i", &searchOption);

                    if(searchOption > 1) {
                        char term[30];
                        printf("Enter term: ");
                        scanf("%s", &term);

                        if(searchOption == 2) {
                            strcat(buff, "4|ID|");
                            strcat(buff, userID);
                            strcat(buff, "|");
                            strcat(buff, term);
                        }
                        if(searchOption == 3) {
                            strcat(buff, "4|NAME|");
                            strcat(buff, userID);
                            strcat(buff, "|");
                            strcat(buff, term);
                        }
                        if(searchOption == 4) {
                            strcat(buff, "4|YEAR|");
                            strcat(buff, userID);
                            strcat(buff, "|");
                            strcat(buff, term);
                        }
                        if(searchOption == 5) {
                            strcat(buff, "4|AUTHOR|");
                            strcat(buff, userID);
                            strcat(buff, "|");
                            strcat(buff, term);
                        }
                    }
                    else {
                            strcat(buff, "4|ALL|");
                            strcat(buff, userID);
                            strcat(buff, "|");
                    }

                    break;
                };
                case 3: {
                    char id[30];
                    printf("Enter ID: ");
                    scanf("%s", &id);
                    strcat(buff, "5|");
                    strcat(buff, userID);
                    strcat(buff, "|");
                    strcat(buff, id);
                    break;
                };
                case 4: {
                    char id[30];
                    printf("Enter ID: ");
                    scanf("%s", &id);
                    strcat(buff, "6|");
                    strcat(buff, userID);
                    strcat(buff, "|");
                    strcat(buff, id);
                    break;
                };
                default: printf("Wrong option\n");
            }
        }

        printf("\nWrite: %s\n", buff);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

        if(userLogedIn == 0 && option == 2) {

            if(strcmp(buff, "ERROR") != 0) {
                userLogedIn = 1;
                strcat(userID, buff);
            }
		}

		printf("\nResponse: %s\n", buff);

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
	{
        printf("connected to the server..\n");
	}

	// function for chat
	doWork(sockfd);

	// close the socket
	close(sockfd);
}

void printMenu() {

    printf("**** MENU ***\n");

    if(userLogedIn == 0) {
        printf("1. Register\n");
        printf("2. Login\n");
    }
    else {
        printf("1. Logout\n");
        printf("2. Search\n");
        printf("3. Check status\n");
        printf("4. Reserve");
    }

    printf("Enter option: \n");
}
