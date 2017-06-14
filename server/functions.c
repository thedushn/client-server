//
// Created by root on 13.6.17..
//

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
#include "functions.h"
#include <unistd.h>
#define BUF_SIZE 2000
#define CLADDR_LEN 100
#define PACKET_SIZE 1400
char * test="Type the name  of the file u want to receive\n";
char *message= "Iste su extensions mozemo da nastavimo sa slanjem";
int num_packets;
char *message2= "reci kako oces da se zove file koji ti primas\n";
char *end ="end of file";

int rezultat=1;

void *receiving(void *socket) {
    printf("\nusli smo tu gde treba: receiving \n");
    int sockfd, ret;
    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    sockfd = (int) socket;
    char buffer2[BUF_SIZE];
    char buffer_file[BUF_SIZE];
    char num_packets[BUF_SIZE];
    memset(num_packets, 0, BUF_SIZE);
    memset(buffer_file, 0, BUF_SIZE);
    memset(buffer2, 0, BUF_SIZE);
    ssize_t dataReceived;
    FILE *filefd = NULL;
    //zakljucamo chat2
    if (pthread_mutex_lock(&mut) != 0) {
        printf("lock didnt work fuck my life\n");
        exit(1);
    }
    printf("kako oces da se zove file \n");

    /*if(gets(buffer2, BUF_SIZE, stdin)!=NULL){
        ret = send(sockfd,buffer2,BUF_SIZE,0);
        if(ret<0){
            printf("Error sending data!\n\t-%s", buffer2);
        }*/
    ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
    if (ret < 0) {
        printf("Error sending data!\n\t-%s", buffer);
        //printf("slanje broja paketa nije uspelo\n");
    }
    //	printf("sta salje client %s\n", buffer2);
    printf("sta salje client %s\n", buffer);//ovde primi da su file iste extensions
    //sada u ovom receive treba da primi file kako treba da sse zove
    ret = recvfrom(sockfd, buffer2, BUF_SIZE, 0, NULL, NULL);
    if (ret < 0) {
        printf("Error sending data!\n\t-%s", buffer2);
    }
    printf("sta salje client %s\n", buffer2);
    //primanje broja paketa
    ret = recvfrom(sockfd, num_packets, BUF_SIZE, 0, NULL, NULL);
    if (ret < 0) {
        printf("Error sending data!\n\t-%s", num_packets);
    }
    printf("broj paketa : %s\n", num_packets);
    int packets = atoi(num_packets);
    printf("broj packeta %d \n", packets);
    int count = 0;
    filefd = fopen(buffer, "w+");
    pthread_mutex_lock(&mut);
    if (filefd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<packets;i++){
  //  while(1){

        dataReceived = read(sockfd,buffer_file,PACKET_SIZE);
        printf("data %s",buffer_file);

        //for(int i=0; i<packets;i++){
          //  printf("i: %d",i);
        if(dataReceived>0) {
            if(strcmp(buffer_file,end)==0) {
                printf("dosli smo do kraja file");
                fclose(filefd);
                break;
            }
            fwrite(buffer_file, 1, dataReceived, filefd);
        }
            if(dataReceived < 0) {
                printf("\n Read Error \n");
            }

           // printf("\nFile OK....Completed\n");
            printf("%d",i);
        //}

  //  }


    	}
    fclose(filefd);
    printf("\nFile OK....Completed\n");
    printf("lets seee if this works\n");
    pthread_mutex_unlock(&mut);

    /*	while((dataReceived = read(sockfd,buffer_file,PACKET_SIZE))	> 0){
            printf("data %s",buffer_file);

            if(strncmp(buffer_file,end,11)==0){
                fclose(filefd);
                printf("zatvorili smo file");
                break;
            }
            for(count=0;count<=packets;count++){
                printf("count: %d\n",count);
            fwrite(buffer_file,1,dataReceived,filefd);
            }
            break;
        }
            if(dataReceived < 0)
                {
                    printf("\n Read Error \n");
                }
            printf("\nFile OK....Completed\n");
    //}
            fclose(filefd);
    pthread_cond_signal(&cond);
    printf("izasli smo iz receiving\n");*/


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
        receiving(sockfd);
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
            pthread_cond_wait(&cond,&mut);
            printf("condtion was met\n");
        }
        else{
            while (fgets(buffer,BUF_SIZE,stdin) != NULL) {
                //pthread_mutex_lock(&mut);
                //while(rezultat==0){pthread_cond_wait(&cond,&m);
                //}

                printf("YOU : %s\n",buffer);
                ret = send(sockfd,buffer,BUF_SIZE,0);

                if (ret < 0) {
                    printf("Error sending data!\n\t-%s", buffer);
                }
                // pthread_mutex_unlock(&mut);
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
            confromation(sockfd);
            //samo za test da vidimo sta client salje tj da li uopste salje
            //*receive_some_files(sockfd);
        }
        else{
            printf("nope didnt work lets try again\n");
            printf("rezultat %d ",rezultat);
        }
    }
}