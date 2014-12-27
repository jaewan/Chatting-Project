//	Jae
//	SungKyunkwan university Software Engineering
//	2014/12/28
//	Chatting program in c
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 256

void *thread_read(void *);
void *thread_write(void *);

int main(int argc, char *argv[])
{
	int port, connfd[1];
	char Host[200];
	struct hostent *h;
	struct sockaddr_in saddr;
	pthread_t tid[2];

	port = 10001;
	if(argc != 1)
	{
		port = atoi(argv[1]);
	}
	strcpy(Host,"localhost");

	//connecting server**********************************************************************
	if ((connfd[0] = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		write(2,"socket() failed.\n",17);
		write(2,"E_CONN, ",8);
		write(2,"\n",1);
		exit(2);
	}
	if((h = gethostbyname(Host)) == NULL)
	{
		write(2,"invalid hostname ",17);
		exit(2);
	}
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	bcopy((char *)h->h_addr,(char*)&saddr.sin_addr.s_addr,h->h_length);
	saddr.sin_port = htons(port);
	if(connect(connfd[0],(struct sockaddr *)&saddr,sizeof(saddr))<0)
	{
		//write(1,"connect() failed\n",17);
		write(2,"E_CONN, ",8);
		write(2,"\n",1);
		exit(3);
	}
	//connecting server**********************************************************************
	pthread_create(&tid[0],NULL,thread_read,connfd);
	pthread_create(&tid[1],NULL,thread_write,connfd);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
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
