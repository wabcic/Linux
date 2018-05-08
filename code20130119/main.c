#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>			 /*文件控制*/
#include <sys/time.h>	 		/*时间方面的函数*/
#include <errno.h>		 	/*有关错误方面的宏*/
#include <sys/poll.h> 			//poll()
#include <string.h> 			//memset()

#include "test.h"
#include "testlib.h"
extern void *recv_pthread();
extern void *Task_thread();	
extern void  *main_thread();
extern int upDI();
extern void *interrupt();
extern void *bc_1553();
extern int fdint;
int fd ;
char *dev_file1 = "/dev/fpga_device";

int main()
{
        pthread_t tid1,tid2,tid3,tid4,tid5;
        int res;
        void *thrd_ret;
    
	fdint=open("/dev/buttons1",O_RDWR);
	if(fdint<0)
	{

	printf("can not open /dev/buttons1 \n");
	
	}
      

	fd = open (dev_file1, O_RDWR);
	if (fd < 0) {
		printf("Failed to open %s.", dev_file1);
		return fd;
	}

       //set up the 1553BC thread
	res = pthread_create(&tid5,NULL,bc_1553,NULL);							
	if(res!=0)
	{		
	   printf("can not create thread:%s\n",strerror(res));
	}
	printf("create bc1553b success\n waiting for interrupt to finish...\n");
        
	 //create recv_pthread to set up the net	
        res=pthread_create(&tid1,NULL,recv_pthread,NULL);
        if(res!=0)
        {
           printf("can not create thread:%s\n",strerror(res));
           //exit(0);
         }
         printf("create recv_pthread success\n waiting for recv_pthread to finish...\n");

	//set up the Task_thread
         res=pthread_create(&tid2,NULL,Task_thread,NULL);
        if(res!=0)
        {
           printf("can not create thread:%s\n",strerror(res));
           //exit(0);
         }
        printf("create Task_thread success\n waiting for Task_thread to finish...\n");

	//set up the Task_thread
         res=pthread_create(&tid3,NULL,main_thread,NULL);
        if(res!=0)
        {
           printf("can not create thread:%s\n",strerror(res));
           //exit(0);
         }
        printf("create main_thread success\n waiting for main_thread to finish...\n");
	  
	//set up the Task_thread
         res=pthread_create(&tid4,NULL,interrupt,NULL);
        if(res!=0)
        {
           printf("can not create thread:%s\n",strerror(res));
           //exit(0);
         }
	printf("create interrupt success\n waiting for interrupt to finish...\n");	

	
	res=pthread_join(tid5,&thrd_ret);
         if(!res)
         {
             printf("thread %u joined\n",(unsigned int)tid5);
         }
         else
         {
             printf("thread %u joined failed\n",(unsigned int)tid5);
          } 
       	 res=pthread_join(tid1,&thrd_ret);
         if(!res)
         {
             printf("thread %u joined\n",(unsigned int)tid1);
         }
         else
         {
             printf("thread %u joined failed\n",(unsigned int)tid1);
          }

	 res=pthread_join(tid2,&thrd_ret);
         if(!res)
         {
             printf("thread %u joined\n",(unsigned int)tid2);
         }
         else
         {
             printf("thread %u joined failed\n",(unsigned int)tid2);
          }
         
	 res=pthread_join(tid3,&thrd_ret);
         if(!res)
         {
             printf("thread %u joined\n",(unsigned int)tid3);
         }
         else
         {
             printf("thread %u joined failed\n",(unsigned int)tid3);
          } 

	 res=pthread_join(tid4,&thrd_ret);
         if(!res)
         {
             printf("thread %u joined\n",(unsigned int)tid4);
         }
         else
         {
             printf("thread %u joined failed\n",(unsigned int)tid4);
          } 

	
	close(fdint);
        close(fd);
         return 0;
       
}
