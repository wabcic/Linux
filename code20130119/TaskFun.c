#include <stdio.h>
//#include "xparameters.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "TaskManage.h"
#include "gk_opcua_header.h"
//#include "gpio.h"	
#include "test.h"
#include "testlib.h"
#ifdef FPAG_APP_DEBUG
#define FDEBUG(fmt, args...)	printf("APP: " fmt "\n", ##args)
#else
#define FDEBUG(fmt, args...)
#endif
#define FERR(fmt, args...)		printf("APP[ERR]: " fmt "\n", ##args)
extern unsigned char	currentTimerTask;			//当前该执行的定时任务index
extern unsigned char	currentSynTask;				//当前该执行的同步任务index

extern unsigned char timerTaskCount;			//how many timer tasks
extern unsigned char unhandledTTs;			//未处理的定时任务启动信号计数
extern unsigned char synTaskCount;			//how many syn tasks

extern unsigned long	gpioData;
extern int DIRECT_TASK_PARA_LEN; 
extern int Ext_funcode;
extern int fd ;

extern unsigned char TestManageCtrlMsgBuffer[67];
extern unsigned char SwirTestCtrlMsgBuffer[132];
extern unsigned char VnirTestCtrlMsgBuffer[100];
// used for Timer task
extern GK_OPCUA_TASK	TIMER_TASK_BUF[];

// used for task para
extern unsigned char	TASK_PARA_BUF[PARANODE_COUNT][MAX_TASK_PARA_LEN];

extern unsigned long gpioPulseEN;
extern int	unReceivedDI;

void OPCUA_SEND_DATAUP();

char BRAM_BUF[BRAM_READ_BUF_LEN];
unsigned long timeValue[2];
//lily
unsigned int addr=0;

int setBICC_DBPara( GK_OPCUA_TASK *pTask , int count);
int startBICC_DB( GK_OPCUA_TASK *pTask );
int START_DI_sy( GK_OPCUA_TASK *pTask );
int STOP_DI_sy( GK_OPCUA_TASK *pTask );

extern GK_OPCUA_TASK DIRECT_TRIGGER_TASK, DIRECT_SETPARA_TASK;


//get task para and write to BRAM
int setPara( GK_OPCUA_TASK *pTask )
{

	unsigned long *pPara;
	int para_len;
	printf("this is setPara............\n");
	
	printf("DIRECT_SETPARA_TASK.ulParaLen=%x\n",DIRECT_SETPARA_TASK.ulParaLen);
	
	printf("Ext_funcode=%x\n",Ext_funcode);	
	if ( pTask->ulParaLen >= 4 )
	{
		//first parameter(unsigned long) means the owner of the para data
		pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
		pTask->ulParaStart += 4;
		pTask->ulParaLen -= 4;
//		if ( Ext_funcode != GK_OPCUA_DIRECT_TASK )
//			*pPara=ntohl(*pPara);                          	//32bits
		
		printf("*pPara=%x\n",*pPara);
                printf("DIRECT_SETPARA_TASK.ulParaLen-4=%x\n",DIRECT_SETPARA_TASK.ulParaLen);
		switch ( *pPara )
		{

			case START_BICC_DB:
				setBICC_DBPara( pTask , pTask->ulParaLen );
				printf("this is setBICC_DBPara( pTask )\n");
				break;
                        case START_BICC_K_422:
                                setBICC_K_422_Para( pTask , pTask->ulParaLen );
				printf("this is setBICC_k_422Para( pTask )\n");
				break;
                        case START_BICC_D_422:
                                setBICC_D_422_Para( pTask , pTask->ulParaLen );
				printf("this is setBICC_D_422Para( pTask )\n");
				break;
                        case START_BICC_SYN:
                                setBICC_SYN_Para( pTask , pTask->ulParaLen );
				printf("this is setBICC_SYNPara( pTask )\n");
				break;   
			case START_BICC_YC:
                                setBICC_YC_Para( pTask , pTask->ulParaLen );
				printf("this is setBICC_YC_Para( pTask )\n");
				break;
			case BROADCASE_TIME_1553:
				printf("this is broadcasting time.........\n");
				timecodedeal((unsigned char *) pPara);
				printf("this is broadcasting time.........\n");
				break;
			case BROADCASE_STATUS_1553:
				printf("this is broadcasting status....... \n");
				sysstatusdeal((unsigned char *) pPara);
				printf("this is broadcasting status....... \n");
				break;
			case BROADCASE_PALTPARA_1553:
				platparadeal((unsigned char *) pPara);
				printf("this is broadcasting paltpara......\n"); 
				break;
			case SEND_CONTROLCMD_1553:
				controlcmddeal((unsigned char *)pPara);
				printf("this is sending control cmd.......\n");
				break;
			case SEND_DATAINJECT_1553:
				dataindeal((unsigned char *) pPara);
				printf("this is sending datainjecting.....\n");
				break;
			case SEND_STRESSTEST_1553:
				stresstest((unsigned short *) pPara);
				printf("this is sending stresstesting.....\n");
			default:
				break;
		
		}
	}

	return 0;
}
//lily
int setBICC_SYN_Para( GK_OPCUA_TASK *pTask , int count )
{
	addr=0x0e02;
	common(pTask,count);
	addr=0;
}

int setBICC_K_422_Para( GK_OPCUA_TASK *pTask , int count )
{
	addr=0x0f02;
	common(pTask,count);
	addr=0;
}

int setBICC_D_422_Para( GK_OPCUA_TASK *pTask , int count )
{
	addr=0x0c02;
	common(pTask,count);
	addr=0;
}

int setBICC_DBPara( GK_OPCUA_TASK *pTask , int count )
{
	addr=0x0d02;
	common(pTask,count);
	addr=0;
}
int setBICC_YC_Para( GK_OPCUA_TASK *pTask , int count )
{
	addr=0x0d02;//undecided
	common(pTask,count);
	addr=0;
}


int common( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i=0,m=0;
	short unsigned array[50];	

	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);

	if((count%2)==1)
	{
		count=count/2+1;
		m=1;
	}
	else
	{
		count=count/2;
	}

	printf("count= %x\n",count);

	for(i=0;i<count;)
	{
		array[i]=(short unsigned)*pPara;           //change 32bits to 16bits--get low 16bits
		array[i+1]=(short unsigned)(*pPara>>16);     //change 32bits to 16bits--get high 16bits
		
		pPara++;
		i+=2;
	}
	for(i=0;i<count;i++)
	{    
		if((m==1) && (i==count-1))
                {
			m=0;
                 	fdev_wword(fd,addr,array[i]);
                 	printf("write_k_arm=%4.4x ",array[i]);
                }
            	else
		{
			array[i] = ntohs(array[i]);
			fdev_wword(fd,addr,array[i]);
			printf("write_k_arm=%4.4x ",array[i]);
		}
	}
}



/*

int setBICC_SYN_Para( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i=0,m=0;
	short unsigned array[50];	

	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);

	if((count%2)==1)
	{
		count=count/2+1;
		m=1;
	}
	else
	{
		count=count/2;
	}

	printf("count= %x\n",count);

	for(i=0;i<count;)
	{
		array[i]=(short unsigned)*pPara;           //change 32bits to 16bits--get low 16bits
		array[i+1]=(short unsigned)(*pPara>>16);     //change 32bits to 16bits--get high 16bits
		
		pPara++;
		i+=2;
	}
	for(i=0;i<count;i++)
	{    
		if((m==1) && (i==count-1))
                {
			m=0;
                 	fdev_wword(fd,0x0e02,array[i]);
                 	printf("write_k_arm=%4.4x ",array[i]);
                }
            	 else
		{
			array[i] = ntohs(array[i]);
			fdev_wword(fd,0x0e02,array[i]);
			printf("write_k_arm=%4.4x ",array[i]);}
		}

	printf("\n");
}

int setBICC_K_422_Para( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i;
	short unsigned array[50];	
        int m;


	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
	if(count && 0x00000001==0x00000000001){
        count=count/2+1;
        m=1;
        }
        else 
        count /=2;
	printf("count= %x\n",count);
	for(i=0;i<count;)
	{
		array[i]=(short unsigned)*pPara;           //change 32bits to 16bits--get low 16bits
		array[i+1]=(short unsigned)(*pPara>>16);     //change 32bits to 16bits--get high 16bits
		
		pPara++;
		i+=2;
	}
	for(i=0;i<count;i++)
	{    if(m==1 && i==count-1)
                {m=0;
                 fdev_wword(fd,0x0f02,array[i]);
                 printf("write_k_arm=%4.4x ",array[i]);
                }
             else{
		array[i] = ntohs(array[i]);
		fdev_wword(fd,0x0f02,array[i]);
		printf("write_k_arm=%4.4x ",array[i]);}
	}

	printf("\n");
}


int setBICC_D_422_Para( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i;
        int m;
	short unsigned array[50];	
	//int fd = 0;


	//fd = open (dev_file, O_RDWR);
	//if (fd < 0) {
	//	FERR("Failed to open %s.", dev_file);
	//	return fd;
	//}

	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
        if(count && 0x00000001==0x00000000001){
        count=count/2+1;
        m=1;
        }
        else 
        count /=2;
	printf("count= %x\n",count);

	for(i=0;i<count;)
	{
		array[i]=(short unsigned)*pPara;           //change 32bits to 16bits--get low 16bits
		array[i+1]=(short unsigned)(*pPara>>16);     //change 32bits to 16bits--get high 16bits
		
		pPara++;
		i+=2;
	}
	for(i=0;i<count;i++)
	{
		 if(m==1 && i==count-1)
                {m=0;
                 fdev_wword(fd,0x0c02,array[i]);
                 printf("write_k_arm=%4.4x ",array[i]);
                }
             else{
		array[i] = ntohs(array[i]);
		fdev_wword(fd,0x0c02,array[i]);
		printf("write_k_arm=%4.4x ",array[i]);}
	}

	printf("\n");
}




int setBICC_DBPara( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i;
	short unsigned array[10];	
//	int fd = 0;


	//fd = open (dev_file, O_RDWR);
	//if (fd < 0) {
	//	FERR("Failed to open %s.", dev_file);
	//	return fd;
	//}

	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
	count /=2;
	printf("count= %x\n",count);
	for(i=0;i<count;)
	{
		array[i]=(short unsigned)*pPara;           //change 32bits to 16bits--get low 16bits
		array[i+1]=(short unsigned)(*pPara>>16);     //change 32bits to 16bits--get high 16bits
		
		pPara++;
		i+=2;
	}
	for(i=0;i<count;i++)
	{
		array[i] = ntohs(array[i]);
		fdev_wword(fd,0x0d02,array[i]);
		printf("write_arm=%4.4x ",array[i]);
	}

	printf("\n");
}

*/







	

int performTask( GK_OPCUA_TASK *pTask )
{      
 
}

int startBICC_DB( GK_OPCUA_TASK *pTask )
{

	printf("this is startBICC_DB!!!!\n");	
	fdev_wword(fd,0x0d08,0xaaaa);
        fdev_wword(fd,0x0d06,0xaaaa);
}

int START_DI_sy( GK_OPCUA_TASK *pTask )
{
	
	int num;
	printf("this is START_DI\n");
	num=fdev_wword(fd,0x0908,0xaaaa);
	printf("num=%x \n",num);
        num=fdev_wword(fd,0x0906,0xaaaa);
	printf("num=%x \n",num);	
}

int STOP_DI_sy( GK_OPCUA_TASK *pTask )
{
	printf("this is STOP_DI!\n");	
	fdev_wword(fd,0x0908,0xaaaa);
        
}
int  START_BICC_K_422_zs( GK_OPCUA_TASK *pTask )
{
	printf("this is startBICC_K!!!!\n");	
	fdev_wword(fd,0x0f08,0xaaaa);
        fdev_wword(fd,0x0f06,0xaaaa);
}

int  START_BICC_D_422_zs( GK_OPCUA_TASK *pTask )
{
	
	printf("this is startBICC_D!!!!\n");	
	fdev_wword(fd,0x0c08,0xaaaa);
        fdev_wword(fd,0x0c06,0xaaaa);
}


int  START_BICC_SYN_zs( GK_OPCUA_TASK *pTask )
{
	
	printf("this is START_BICC_SYN_zs!!!!\n");	
	fdev_wword(fd,0x0e08,0xaaaa);
        fdev_wword(fd,0x0e06,0xaaaa);
}


