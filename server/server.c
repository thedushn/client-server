//
// Created by root on 13.6.17..
//

/*
uspe da posalje file i da ga imenuje posle 2 pokusaja
napravi file koji zelimo posalje
problem puni podatke sa tekstom koji se ispisuje u konzoli a ne tekstom koji se nalazi u drugom file
nece da se zakljuca chat2...
ne pravi file
treba ubaciti condtion variable a nece mamu mu  njegovu
bio je ubacen cond var i radilo je samo sto je ispisivalo jos uvek conslose type shit
sada ni to nece chat2 salje sve treba izbaciti nesto iz receiving da bi to radilo kako treba bufer2
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
#include "functions.h"
#include <unistd.h>
#include <arpa/inet.h>
#include "functions.h"

#define BUF_SIZE 2000
#define CLADDR_LEN 100
#define PACKET_SIZE 1400










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
    if(pthread_mutex_init(&mut, NULL)!=0){
        printf("mutex init failed \n");
        exit(1);
    }
    pthread_cond_init(&cond,NULL);

    printf("making threads\n");
    ret = pthread_create(&t,NULL,chat, (int *) newsockfd);
    if(ret) {
        printf("ERROR: Return Code from pthread_create() is %d\n", ret);
        exit(1);
    }
    printf("%d\n",ret);

    ret1 = pthread_create(&t1, NULL, chat2,  (int*) newsockfd);
    if (ret1) {
        printf("ERROR: Return Code from pthread_create() is %d\n", ret1);
        exit(1);
    }
    pthread_join( t1, NULL);
    pthread_join( t, NULL);



    close(newsockfd);
    close(sockfd);
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return;



}