#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"pthread.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "functions.h"
#include <unistd.h>
#include <arpa/inet.h>
#include "functions.h"
#define PORT 4445
#define BUF_SIZE 2000
#define PACKET_SIZE 1400
 pthread_mutex_t m;
/*char *test ="send";
char *test2 =".";
char *test3= "Type the name  of the file u want to receive"; */
//char *message= "Iste su extensions mozemo da nastavimo sa slanjem";



int main(int argc, char *argv[]){

	struct sockaddr_in addr, cl_addr;
	int sockfd, ret,ret2;
	char buffer[BUF_SIZE];
	char * serverAddr;
	pthread_t t,t1;
    printf("for the love of god");
  /*  FILE *filefd ;
    printf("for the love of god");
    fopen("ffs.dat","r");
    fgets(buffer, 1000, filefd);
    printf("\n%s\n",buffer);
    if (filefd == NULL) {
       printf("file didnt open");
         perror("open");
      //  exit(EXIT_FAILURE);
    }*/

/*	if (argc < 2) {
	printf("usage: client < ip address >\n");
	exit(1);
	}
   serverAddr =argv[1]; */
	if(argc <2){
	printf("port number fail");
	exit(1);
	}
 	int portnum= atoi(argv[1]);
 serverAddr = "127.0.0.1";

	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
		printf("Error creating socket!\n");
		exit(1);
		}
	printf("Socket created...\n");
	printf("socket %d",sockfd);
	 memset(&addr, 0, sizeof(addr));
	 addr.sin_family = AF_INET;
	 addr.sin_addr.s_addr = inet_addr(serverAddr);
	 addr.sin_port = htons(portnum);

	ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	 if (ret < 0) {
	  printf("Error connecting to the server!\n");
	  exit(1);
	 }
	 printf("Connected to the server...\n");

	 memset(buffer, 0, BUF_SIZE);
	 printf("Enter your messages one by one and press return key!\n");

	//inicijalizunjemo mutex za zakljucavanje chata
	pthread_mutex_init(&m, NULL);
	 ret = pthread_create(&t, NULL, chat,(void *)sockfd);
	 if (ret) {
	  printf("ERROR: Return Code from pthread_create() is %d\n", ret);
	  exit(1);
	 }

	ret2 = pthread_create(&t1, NULL, chat2, (void *)sockfd);
 	if (ret2) {
	  printf("ERROR: Return Code from pthread_create() is %d\n", ret);
	  exit(1);
	 }
	 pthread_join( t1, NULL);
	 pthread_join( t, NULL);

	 close(sockfd);
	//unistavanje mutexa
	pthread_mutex_destroy(&m);

	 pthread_exit(NULL);

	 return 0;
	}

