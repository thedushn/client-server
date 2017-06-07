
#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"
#include"pthread.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "functions2.h"

#define PORT 4440
#define BUF_SIZE 2000
#define PACKET_SIZE 1400
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
char *test ="send";
char *test2 =".";
char *test3= "Type the name  of the file u want to receive\n";
char *message= "Iste su extensions mozemo da nastavimo sa slanjem\n";
char *message2= "reci kako oces da se zove file koji ti primas\n";

char *end ="end of file";

void * chat(void * socket) {
 int sockfd, ret;
 char buffer[BUF_SIZE];
 sockfd = (int) socket;
pthread_t t;
int new;
 memset(buffer, 0, BUF_SIZE);
	printf("chat \n");
 while (1) {
	printf("we are in chat (receiving messages) \n");
  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
  if (ret < 0) {
   printf("Error receiving data!\n");
  } else {
   printf("server: ");
   fputs(buffer, stdout);
   //printf("\n");
	printf("\ndosli smo \n");
	printf("sta pise u bufferu %s \n",buffer);
	printf("sta pise u test %s\n",test);
		int rezultat=strcmp(buffer, test);
	printf("rezultat %d\n",rezultat);
	if (rezultat==0){
    	printf("it worked oce da primi file\n");
		printf("izlazimo iz chat\n");
	*conformation(sockfd);
	}
	else{
	printf("u didnt type it right u lemon \n");
	}


  }
 }
}

void *conformation (void * socket){
 int sockfd, ret;
 char buffer[BUF_SIZE];
 sockfd = (int) socket;

printf("we made it conformation\n");

ret = send(sockfd,test3,BUF_SIZE,0);

		if (ret < 0) {
	   	printf("Error sending data!\n\t-%s", test3);
		exit(1);
	 	}
	printf("sending server to tell him to type the name of the new file \n");
	*send_some_files(sockfd);
}


void  *send_some_files(void * socket){
	int sockfd, ret;
 	char buffer[BUF_SIZE];
	char buffer_test[BUF_SIZE];
	char buffer_file[PACKET_SIZE];
 	sockfd = (int) socket;
	struct sockaddr_in addr;
	FILE *filefd = NULL;
	//ssize_t i;
    	//ssize_t read_return;
	char test_buffer[BUF_SIZE];
	double	file_size;


	printf("usli smo u send_some_files \n");
	//zakljucamo chat da ne mozemo da primamo vise poruka od servera dok se ne zavrsi slanje
	pthread_mutex_lock (&m);
	ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
	printf("primio je ime file od servera\n");
	if (ret < 0) {
	   printf("Error sending data!\n\t-%s", buffer);
	  }
	//upisujemo u buffer_test ime file koji server oce da primi
	printf("buffer %s\n",buffer);
	strcat(buffer_test, buffer);
	printf("buffer_test %s\n",buffer_test);
	//proveravamo da li file postoji
	filefd= fopen(buffer_test,"r");
	  fgets(test_buffer, 1000, filefd);

	  printf("\n%s\n",test_buffer);

	if (filefd == NULL) {
            perror("open");
            exit(EXIT_FAILURE);
        }
	printf("we opened the file \n");
	printf("sending to server that we found the file\n");
	ret = send(sockfd,message2,BUF_SIZE,0);
	if(ret<0){
		printf("Error sending data!\n\t-%s", message2);
		}
	fseek(filefd, 0, SEEK_END);
//da vidimo koliki je file
	file_size = ftell(filefd);
	fseek(filefd, SEEK_SET, 0);// da postavimo pokazivac na pocetak file
	//Provera da li je isti extension lakse bi bilo u server ali ajde da se igramo
	ret = recv(sockfd,buffer,BUF_SIZE,0);
	printf("kako server oce da nazove file %s \n",buffer);
	if(ret<0){
		printf("Error sending data!\n\t-%s", buffer);
		}
	printf("da li je primio ime file %s\n", buffer);
 	 char test2 ='.';
	const char *buffer_const=  (const char *) buffer;
	const char *buffer_test_c=  (const char *)buffer_test;

	char *r;
	char *r2;
	r= strchr(buffer_test_c,'.');
	r2= strchr(buffer_const,'.');
	int compare;
	compare=strcmp(r,r2);
	printf("buffer_test: %s buffer: %s r:%s r2:%s \n",buffer_test,buffer,r ,r2);
	if(compare!=0){
	printf("the extensions arent the same we fucked up \n");

	printf("comparing %d ",compare);
	printf("return to begging of the program");
		pthread_mutex_unlock(&m);
		chat(&sockfd);

	}
	memset(buffer_test,0,BUF_SIZE);
	printf("comparing %d \n",compare);
	ret=send(sockfd,buffer,BUF_SIZE,0);//dodato 6.5
	ret= send(sockfd,message,BUF_SIZE,0);
	if(ret<0){
		printf("Error sending data!\n\t-%s", message);

		}

	//sending files o yeee
	//see if the file exists
	//devide it into packets and send :P
	//printf("prodje sve sada pada segmentation fault\n");
	/*filefd= fopen(buffer_test,"rb");
	if (filefd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

	printf("we opened the file \n");
	fseek(filefd, 0, SEEK_END);
	file_size = ftell(filefd);
	*/

	printf("file size is %f mb\n",file_size/1000000);
	double packets = (file_size/PACKET_SIZE+1);
	int ipackets = (int)packets;
	printf("number of packets %d\n",ipackets);
	//int cpackets = htonl(packets);
	snprintf(buffer, BUF_SIZE, "%d", ipackets);
		ret=0;
	printf("%s\n",buffer);
	ret=send(sockfd,buffer,BUF_SIZE,0);
	printf("%s\n",buffer);
	if(ret<0){
	 	printf("Error sending data!\n\t-%s", buffer);

	}







	while(1){

		printf("usli smo u nread while petlju\n");
	int nread =fread(buffer_file,1,PACKET_SIZE,filefd);

	if(nread >0){

	write(sockfd,buffer_file,nread);

	}
	if(nread < PACKET_SIZE){

		if(feof(filefd)){
			printf("end of file \n");
			printf("the file was sent to %d\n",sockfd);
			ret= send(sockfd,end,BUF_SIZE,0);
				if(ret<0){
					printf("Error sending data!\n\t-%s", end);

				}

		}
		if (ferror(filefd)){
                    printf("Error reading\n");
		}
		pthread_mutex_unlock(&m);
                break;
        }
	}

	fclose(filefd);//6.6
	printf("Closing file\n");
	/*while(1){
	read_return = read(filefd,buffer,BUF_SIZE);
	if(read_return ==0)
		printf("neuspesna mizija za izija\n");
		break;
		if(read_return==-1){
			 perror("read");
            		exit(EXIT_FAILURE);
		}
		 if (write(sockfd, buffer, read_return) == -1) {
           	 perror("write");
           	 exit(EXIT_FAILURE);
        	}

	close(filefd);
	}*/

}




void * chat2(void * socket){
	int sockfd, ret;
 	char buffer[BUF_SIZE];
 	sockfd = (int) socket;
	struct sockaddr_in addr;

	printf("we are in chat2,(sending messages)  \n");

 	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		printf("YOU : %s",buffer);
	 	ret = send(sockfd,buffer,BUF_SIZE,0);
	  if (ret < 0) {
	   printf("Error sending data!\n\t-%s", buffer);
	  }
	 }



}
