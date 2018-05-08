#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TaskManage.h"

#define SERVPORT 8888
#define BACKLOG 10
#define MAX_CONNECTED_NO 10
#define MAXDATASIZE 960
extern void *Task_thread();
extern void opcua_instrcution_request(void *p);
extern void *opcua_send_server(void *p);
void opcua_instruction_thread();

char read_buf[FRAME_LEN];
int ret_sd;
int sin_size,recvbytes;

int Ext_RecThread;

void *recv_pthread()
{
        struct sockaddr_in server_sockaddr,client_sockaddr;
	
	int sockfd,client_fd,new_sd;

       
        pthread_t tid;
        int res;
        void *thrd_ret;	
        int temp;


	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
        {
		perror("sint sin_size,recvbytes socket");
		exit(1);
	}

	printf("socket success!,sockfd=%d\n",sockfd);

	temp = 1;	/* allow address reuse */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) < 0)
	 {
		printf("failed to set socket reuse\n");
	}
	

	server_sockaddr.sin_family=AF_INET;
	server_sockaddr.sin_port=htons(SERVPORT);
	server_sockaddr.sin_addr.s_addr=INADDR_ANY;
        system("ifconfig eth0 192.168.0.232");
        system("ifconfig eth0 up");
	bzero(&(server_sockaddr.sin_zero),8);
        int i=1;
        setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
	if(bind(sockfd,(struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr))==-1)
        {
		perror("bind");
		exit(1);
	}
	if(listen(sockfd,BACKLOG)==-1)
        {
		perror("listen");
		exit(1);
	}
        ret_sd=-1;

	Ext_RecThread = 1;
   	
   	while(1)
	{  
	    
		if((client_fd=accept(sockfd,(struct sockaddr *)&client_sockaddr,&sin_size))==-1)
	    	{
			perror("accept");
			exit(1);
	    	}

		if ( Ext_RecThread == 1 )
		{
			Ext_RecThread = 0;

		    	res=pthread_create(&tid,NULL,opcua_instrcution_request,(void *) client_fd);
		    	if(res)
		    	{
		       	 	printf("can not create thread:%s\n",strerror(res));
		        	exit(res);
		    	}
		    	printf("create opcua_instrcution_request success\n waiting for opcua_instrcution_request to finish...\n");

		   	res=pthread_create(&tid,NULL,opcua_send_server,(int *) client_fd);
		   	if(res)
		   	{
		       	 	printf("can not create thread:%s\n",strerror(res));
		       	 	exit(res);
		   	}
		   	printf("create opcua_send_server success\n waiting for thread to finish...\n");
		}
           
        }
	
}

