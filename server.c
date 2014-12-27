//	Jae
//	SungKyunkwan university Software Engineering
//	2014/12/28
//	Chatting program in c
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
//#include "my_string.h"
#define MAXLINE 256

void *thread_read(void *);
void *thread_write(void *);

int main(int argc,char *argv[])
{
	int listenfd;
	unsigned int caddrlen;
	struct sockaddr_in saddr, caddr;
	int port = 10001;
	int *connfdp;
	pthread_t tid[2];
	//initializing port
	if(argc != 1)
	{
		port = atoi(argv[1]);
	}
	
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		write(1,"socket() failed.\n",17);
		return -1;
	}
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family= AF_INET;
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(port);
	
	if(bind(listenfd, (struct sockaddr*)&saddr,sizeof(saddr)) < 0) 
	{
		write(1," address already in use\n",24);
		write(1,"bind() failed.\n",15);
		return -1;
	}
	if (listen(listenfd, 5) < 0) 
	{
		write(1,"listen() failed.\n",17);
		return -1;
	}
	
	connfdp = (int*)calloc(1,sizeof(int));
	if ((*connfdp= accept(listenfd, (struct sockaddr*)&caddr,&caddrlen)) < 0) 
	{
		write(1,"accept() failed.\n",17);
		return -1;
	}
	write(2,"Connection  made\n",17);
	pthread_create(&tid[0],NULL,thread_read,connfdp);
	pthread_create(&tid[1],NULL,thread_write,connfdp);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	free(connfdp);
	//pthread_create(&tid,NULL,thread_path,connfdp);
	

	return 0;
}

void *thread_read(void *arg)
{
	char message[MAXLINE];
	int connfd = *((int*)arg);
	pthread_detach(pthread_self());

	while(1)
	{
		memset(message,0,MAXLINE);
		//read(connfd,message,MAXLINE);
		if(recv(connfd,message,MAXLINE,0)<=0)
		{
			printf("connection ended!!\n");
			pthread_exit(NULL);
		}
		printf("server:%s", message);
	}
}

void *thread_write(void *arg)
{
	char message[MAXLINE];
	int connfd = *((int*)arg),n;
	pthread_detach(pthread_self());

	while(1)
	{
		memset(message,0,MAXLINE);
		n = read(1,message,MAXLINE);
		write(connfd,message,n);
	}
}
