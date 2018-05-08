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
char *dev_file = "/dev/fpga_device";
extern unsigned char	currentTimerTask;			//当前该执行的定时任务index
extern unsigned char	currentSynTask;				//当前该执行的同步任务index

extern unsigned char timerTaskCount;		//how many timer tasks
extern unsigned char unhandledTTs;			//未处理的定时任务启动信号计数
extern unsigned char synTaskCount;		//how many syn tasks

extern unsigned long	gpioData;
extern int DIRECT_TASK_PARA_LEN; 
extern int Ext_funcode;

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

int setBICC_DBPara( GK_OPCUA_TASK *pTask , int count);
int startBICC_DB( GK_OPCUA_TASK *pTask );
int START_DI_sy( GK_OPCUA_TASK *pTask );
int STOP_DI_sy( GK_OPCUA_TASK *pTask );
extern GK_OPCUA_TASK DIRECT_TRIGGER_TASK, DIRECT_SETPARA_TASK;


//get task para and write to BRAM
int setPara( GK_OPCUA_TASK *pTask )
{
//	init();
	unsigned long *pPara;
	int para_len;
	printf("this is setPara............\n");
	//GK_OPCUA_REQUESTHEADER *p;
	printf("DIRECT_SETPARA_TASK.ulParaLen=%x\n",DIRECT_SETPARA_TASK.ulParaLen);
	//Ext_funcode = ntohs(Ext_funcode);
	printf("Ext_funcode=%x\n",Ext_funcode);
/*
	switch( Ext_funcode ){
		case GK_OPCUA_DIRECT_TASK:
			{
				para_len = DIRECT_SETPARA_TASK.ulParaLen;
				printf("para_len1=%x\n",para_len);
				break;
			}
		case GK_OPCUA_TIMER_TASK:
			//para_len = 
			break;
		default :
			break;
	}
	printf("para_len2=%x\n",para_len);	*/	
	if ( pTask->ulParaLen >= 4 )
	{
		//first parameter(unsigned long) means the owner of the para data
		pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
		pTask->ulParaStart += 4;
		pTask->ulParaLen -= 4;
		if ( Ext_funcode != GK_OPCUA_DIRECT_TASK )
		*pPara=ntohl(*pPara);                          //32bits
		
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
				printf("this is setBICC_DBPara( pTask )\n");
				break;
                        case START_BICC_D_422:
                                setBICC_D_422_Para( pTask , pTask->ulParaLen );
				printf("this is setBICC_DBPara( pTask )\n");
				break;
			default:
				break;
		
		}
	}

	return 0;
}
//lily
int setBICC_K_422_Para( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i;
	short unsigned array[50];	
	int fd = 0;
        int m;

	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}
        printf("count1= %x\n",count);
	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
	if((count & 0x00000001)==0x00000000001){
        printf("count==jishu");
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
	int fd = 0;


	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}

	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
        if(count & 0x00000001==0x00000000001){
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




int setBICC_DBPara( GK_OPCUA_TASK *pTask , int count )
{	
	unsigned long *pPara;		

	int i;
	short unsigned array[10];	
	int fd = 0;


	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}

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









	

int performTask( GK_OPCUA_TASK *pTask )
{      
        int i;
	unsigned long *pPara, triggerAddr;
	BRAM_DATA triggerKey;
	
	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
  	
        printf("perform_start==%.2x \n",pTask->ulParaStart);
        for(i=0;i<32;i++)
	printf("%.2x  ",TASK_PARA_BUF[pTask->u16BufferRegion][i]);
	printf("\n");
	triggerAddr = *pPara;
        printf("triggerAddr1= %x\n",triggerAddr);
	if ( Ext_funcode != GK_OPCUA_DIRECT_TASK )	
	triggerAddr = ntohl (triggerAddr);
	printf("triggerAddr2= %x\n",triggerAddr);
	triggerKey.lValue = *(++pPara);

	switch ( triggerAddr )
	{

		case START_BICC_DB:
			startBICC_DB( pTask );
			break;
		case START_DI:
			START_DI_sy(pTask);
			break;
		case STOP_DI:
			STOP_DI_sy(pTask);
			break;	
                case START_BICC_K_422:
                        START_BICC_K_422_zs(pTask);   		
		        break;
                case START_BICC_D_422:
                         START_BICC_D_422_zs(pTask);   
	                 break;
		default:
			break;
	}

	return 0;
}

int startBICC_DB( GK_OPCUA_TASK *pTask )
{
	int fd = 0;
	printf("this is startBICC_DB!!!!\n");
	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}	
	fdev_wword(fd,0x0d08,0xaaaa);
        fdev_wword(fd,0x0d06,0xaaaa);
}

int START_DI_sy( GK_OPCUA_TASK *pTask )
{
	int fd = 0;
	int num;
	printf("this is START_DI\n");
	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open start %s.", dev_file);
		return fd;
	}	
	num=fdev_wword(fd,0x0908,0xaaaa);
	printf("num=%x \n",num);
        num=fdev_wword(fd,0x0906,0xaaaa);
	printf("num=%x \n",num);	
}

int STOP_DI_sy( GK_OPCUA_TASK *pTask )
{
	int fd = 0;
	printf("this is STOP_DI!\n");
	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}	
	fdev_wword(fd,0x0908,0xaaaa);
        
}
int  START_BICC_K_422_zs( GK_OPCUA_TASK *pTask )
{
	int fd = 0;
	printf("this is startBICC_K!!!!\n");
	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}	
	fdev_wword(fd,0x0f08,0xaaaa);
        fdev_wword(fd,0x0f06,0xaaaa);
}

int  START_BICC_D_422_zs( GK_OPCUA_TASK *pTask )
{
	int fd = 0;
	printf("this is startBICC_D!!!!\n");
	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}	
	fdev_wword(fd,0x0e08,0xaaaa);
        fdev_wword(fd,0x0e06,0xaaaa);
}

