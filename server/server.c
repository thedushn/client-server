/*
uspe da posalje file i da ga imenuje posle 2 pokusaja
napravi file koji zelimo posalje
problem puni podatke sa tekstom koji se ispisuje u konzoli a ne tekstom koji se nalazi u drugom file


nece da se zakljuca chat2...
ne pravi file
treba ubaciti condtion variable a nece mamu mu  njegovu



bio je ubacen cond var i radilo je samo sto je ispisivalo jos uvek conslose type shit
sada ni to nece chat2 salje sve treba izbaciti nesto iz receiving da bi to radilo kako treba bufer2


slanje paketa radi samo sto pokupi ime file i upise ga i skupi jos nekih praznih karatketera

*/
#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"pthread.h"
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 4445

#define BUF_SIZE 2000
#define CLADDR_LEN 100
char * test="Type the name  of the file u want to receive\n";
char *message= "Iste su extensions mozemo da nastavimo sa slanjem";
int num_packets;
char *message2= "reci kako oces da se zove file koji ti primas\n";
char *end ="end of file";
pthread_mutex_t m  = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond ;
int rezultat=1;
void *receiving(void *socket){
	printf("\nusli smo tu gde treba: receiving \n");
	int sockfd, ret;
 	char buffer[BUF_SIZE];
	memset(buffer,0,BUF_SIZE);
 	sockfd = (int) socket;
	char buffer2[BUF_SIZE];
	char buffer_file[BUF_SIZE];
	memset(buffer_file,0,BUF_SIZE);
	memset(buffer2,0,BUF_SIZE);
	ssize_t dataReceived;
	FILE *filefd = NULL;
	//zakljucamo chat2
	if(pthread_mutex_lock (&m)!=0){
	printf("lock didnt work fuck my life\n");
	exit(1);
	}
	printf("kako oces da se zove file \n");

	/*if(gets(buffer2, BUF_SIZE, stdin)!=NULL){
		ret = send(sockfd,buffer2,BUF_SIZE,0);
		if(ret<0){
			printf("Error sending data!\n\t-%s", buffer2);
		}*/
	ret =recvfrom(sockfd,buffer,BUF_SIZE,0,NULL,NULL);
		if(ret<0){
		printf("Error sending data!\n\t-%s", buffer);
		//printf("slanje broja paketa nije uspelo\n");
		}

	printf("sta salje client %s\n", buffer);
	//sada u ovom receive treba da primi file kako treba da se zove
	ret =recvfrom(sockfd,buffer2,BUF_SIZE,0,NULL,NULL);
			if(ret<0){
			printf("Error sending data!\n\t-%s", buffer2);
			}
			printf("sta salje client %s\n", buffer2);//ovde primi da su file iste extensions
	//num_packets = atoi(buffer);
	//printf("broj packeta %d \n",num_packets);
	filefd = fopen(buffer,"w+");
	printf("file opened %s",buffer);
			if (filefd == -1) {
           	 		perror("open");
            			exit(EXIT_FAILURE);
        		}
		while((dataReceived = read(sockfd,buffer_file,1024))	> 0){
			//printf("data %s",buffer_file);
			if(strcmp(buffer_file,end)==0){
				fclose(filefd);
				printf("zatvorili smo file");
				break;
			}
			fwrite(buffer_file,1,dataReceived,filefd);
		}
			if(dataReceived < 0)
    			{
       			 printf("\n Read Error \n");
    			}
			printf("\nFile OK....Completed\n");
	//}

	pthread_cond_signal(&cond);
	printf("izasli smo iz receiving\n");


}


void * confromation(void *socket){
	int sockfd, ret;
 	char buffer[BUF_SIZE];
 	sockfd = (int) socket;
	printf("usli smo u conformation \n");
	printf("treba poslati ime file to se radi u chat2\n");

	ret = recv(sockfd,buffer,BUF_SIZE,0);
	if(ret<0){
		printf("Error sending data!\n\t-%s", buffer);
	}
	printf("KLIJENT:%s\n",buffer);
	 rezultat =strcmp(message2,buffer);
	if(rezultat==0){
	*receiving(sockfd);
	}
	else{
	printf("KLIJENT kaze nisu iste extensions");

	}
	printf("izasli smo iz conformation\n");

}
void * chat2(void * socket){
	int sockfd, ret;
 	char buffer[BUF_SIZE];
 	sockfd = (int) socket;
	struct sockaddr_in addr;
while(1){
	 printf("we made it to chat2(sending messages)\n");
	 if(rezultat==0){
		 pthread_cond_wait(&cond,&m);
		 printf("condtion was met\n");
	}
	else{
		while (gets(buffer, BUF_SIZE, stdin) != NULL) {
		//pthread_mutex_lock(&m);
 		//while(rezultat==0){pthread_cond_wait(&cond,&m);
 		//}

 		printf("YOU : %s\n",buffer);
	 	ret = send(sockfd,buffer,BUF_SIZE,0);

	  if (ret < 0) {
	   printf("Error sending data!\n\t-%s", buffer);
	  }
	 // pthread_mutex_unlock(&m);
	  	  break;
		}
	}
	//sleep(10);
}
	printf("izasli smo iz while petlje#nismo\n");

}
void * chat(void * socket) {
	int sockfd , ret;
	char buffer [BUF_SIZE];
	sockfd =(int) socket;
	memset(buffer,0,BUF_SIZE);

	printf("we made it to chat(receving messages)\n");
	for(;;){
		printf("still in chat\n");
		ret =recv(sockfd,buffer,BUF_SIZE,0);
		if (ret < 0) {
			printf("Error receiving data!\n");
		}
		else {
			printf("\nclient: ");
			fputs(buffer, stdout);
			printf("\n");
		}
		if(strcmp(buffer,test)==0){
		printf("Iz chat client: %s",buffer);
		*confromation(sockfd);
		//samo za test da vidimo sta client salje tj da li uopste salje
		//*receive_some_files(sockfd);
		}
		else{
		printf("nope didnt work lets try again\n");
		printf("rezultat %d ",rezultat);
		}
	}
}


void main(int argc ,char *argv[]){

	struct sockaddr_in addr ,cl_addr;
	int sockfd, len ,ret, ret1, newsockfd;
	char buffer[BUF_SIZE];
char buffer2[BUF_SIZE];
	char clientAddr[CLADDR_LEN];
 	pthread_t t,t1;

	if(argc <2){

	printf("no port provided");
	exit(1);
	}
	int portnum=atoi(argv[1]);
	printf("port number %d ",portnum);
	sockfd =socket(AF_INET,SOCK_STREAM,0);
		if(sockfd<0){
			printf("Error creating socket!\n");
			exit(1);
		}
		printf("Socket created \n");

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(portnum);


	ret = bind(sockfd,(struct sockaddr *) &addr, sizeof(addr));
		if (ret < 0){
		printf("Error binding!\n");
		exit(1);
	}
	printf("Binding done...\n");

	printf("Waiting for a connection...\n");

	listen(sockfd, 5); //hello is anybody going to call me :{
	printf("da li smo prosli \n");
	len =sizeof(cl_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cl_addr, &len);
		if (newsockfd < 0) {
		printf("Error accepting connection!\n");
		exit(1);
		}

	inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
	printf("Connection accepted from %s...\n", clientAddr);

	memset(buffer,0,BUF_SIZE);
	printf("time to send some shit over the ethernet\n");
	if(pthread_mutex_init(&m, NULL)!=0){
	printf("mutex init failed \n");
		exit(1);
		}
	pthread_cond_init(&cond,NULL);

	printf("making threads\n");
	ret = pthread_create(&t,NULL,chat, (void *) newsockfd);
		if(ret) {
			printf("ERROR: Return Code from pthread_create() is %d\n", ret);
			exit(1);
		}
		printf("%d\n",ret);

	ret1 = pthread_create(&t1, NULL, chat2, (void *) newsockfd);
	if (ret1) {
	  printf("ERROR: Return Code from pthread_create() is %d\n", ret1);
	  exit(1);
	 }
	pthread_join( t1, NULL);
	 pthread_join( t, NULL);



	close(newsockfd);
	close(sockfd);
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&cond);
	pthread_exit(NULL);
	return;



}
