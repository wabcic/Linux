#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "gk_opcua_header.h"
#include "gk_opcua_types.h"
#include "TaskManage.h"

#define MAXDATASIZE 960

void *opcua_instrcution_request(int *p);
void opcua_instruction_thread();

/**************schedule table*************************************************/
unsigned int 	schedule_len;
unsigned char 	hasNewSchedule;
unsigned char SCHEDULE_BUF[MAX_SCHEDULE_LEN];


/**************task parameter**************************************************/
unsigned char	TASK_PARA_BUF[PARANODE_COUNT+1][MAX_TASK_PARA_LEN];	//No. PARANODE_COUNT used for Direct Task
unsigned long	directTaskParaWP;					//Direct Task Para buf is used as a circular buffer

/**************timer task table*************************************************/
unsigned char timerTaskCount;
GK_OPCUA_TASK	TIMER_TASK_BUF[MAX_TIMER_TASK_COUNT];

/**************syn task table****************************************************/
unsigned char	synTaskCount;
GK_OPCUA_TASK	SYN_TASK_BUF[MAX_SYN_TASK_COUNT];

/**************task start/stop variable***************************************/
unsigned short	START_TASK_PARA;
unsigned char	hasStartTestTask;

unsigned char	STOP_TASK_PARA;
unsigned char	hasStopTestTask;

char FRAME_BUF[FRAME_LEN];
unsigned int	currentFB;		//ping or pong
int 	readlen;		//how many data already readed in current FRAME BUF

//direct task
unsigned char hasDirectTask;
GK_OPCUA_TASK DIRECT_TRIGGER_TASK, DIRECT_SETPARA_TASK;

int backflag=0;
extern int ret_sd; 
int DIRECT_TASK_PARA_LEN;                    
//extern char array[55];
char read_buf[FRAME_LEN];
char recv_buf[MAXDATASIZE];
int Ext_funcode;
extern int Ext_RecThread;

void saveToParaBUF( char* source,int destpos,int datalen,OPC_UINT32 code1 )
{
	unsigned int 	i;
	int j;

	switch ( code1 )
	{
		case GK_OPCUA_TASK_PARA_1:			/* 参数区1*/ 
			i = 0;
			break;
		case GK_OPCUA_TASK_PARA_2:			/* 参数区2 	*/
			i = 1;
			break;
		case GK_OPCUA_TASK_PARA_3:			/* 参数区3 	  */
			i = 2;
			break;
		case GK_OPCUA_TASK_PARA_4:			/* 参数区4 		*/
			i = 3;
			break;
		case GK_OPCUA_TASK_PARA_5:			/* 参数区5 		  */
			i = 4;
			break;
		default:
			i = 0;
			break;
	}
	
	memcpy( (void *)(&TASK_PARA_BUF[i][destpos]), source, datalen );
		
	printf("saveToParaBUF___TASK_PARA_BUF:\n");
	for(i=0;i<5;i++)
	{
		for(j=0;j<datalen;j++)
		printf("%.2x ",TASK_PARA_BUF[i][j]);
		printf("\n");
		
	}
	printf("\n");

}

void *getOneFrame( char *RX_BUF,int sd )
{
	int rx_len;
	int i,j;
	memset( RX_BUF, 0, sizeof( OPCUA_PACK_HEADER ) );

	readlen = 0;

	while ( readlen < FRAME_LEN )
	{
	
           if((rx_len=read(sd,recv_buf,FRAME_LEN))<=0){
	  	perror("recv");
	  	Ext_RecThread =1;
     	        close(sd);
		pthread_exit(1);
	   }
   
		if( rx_len > 0 )
		{
			if ( (readlen + rx_len) >= FRAME_LEN )
			{
				memcpy( FRAME_BUF+readlen, RX_BUF, (FRAME_LEN - readlen) );
				readlen += rx_len;
				break;
			}

			memcpy( FRAME_BUF+readlen, RX_BUF, rx_len );
			readlen += rx_len;
		}
	}
	
//           printf("FRAME_BUF:\n");
//           for(j=0;j<FRAME_LEN;j++)
//           printf("%.2x ",FRAME_BUF[j]);
//		printf("\n");
//	memcpy(array+28,FRAME_BUF+52,20);
	return (FRAME_BUF);
}

void jointemp(char* RX_BUF, int sd, char* source,OPC_INT32 i32len,OPC_INT32 maxlen,OPC_UINT32 code2 )
{
	unsigned int pack_len,len,saveposition;
	unsigned char 	*buf, *pDest;
        char readbuf[FRAME_LEN];
        buf=readbuf;
	int i;
	pack_len=0;
	len=0;
	saveposition=0;

	switch(code2)
	{

		case GK_OPCUA_SCHEDULE:				  /* 时间表任务*/
			pDest = (unsigned char *)SCHEDULE_BUF;
			break;
	        case GK_OPCUA_TIMER_TASK:			           /* 定时任务*/
			pDest = (unsigned char *)TIMER_TASK_BUF;
			break;
		case GK_OPCUA_SYN_TASK:				  /* 同步任务*/
			pDest = (unsigned char *)SYN_TASK_BUF;
			break;
		default:
			return;
	}
	   printf("pHeader->FunCode: %.8x \n",code2);
		
		if ( i32len > (maxlen + OPCUA_HEADER_BYTES + 4) )
			pack_len = maxlen;
		else
			pack_len = i32len - OPCUA_HEADER_BYTES - 4;

		if ( i32len> FRAME_LEN )		//注：此处不能用taskPara_len来判断
			len = FRAME_LEN - OPCUA_HEADER_BYTES - 4;
		else
			len = pack_len;

		memcpy(pDest,source+OPCUA_HEADER_BYTES+4,len);
		saveposition += len;

//	printf("pack_len=%d  saveposition=%d \n",pack_len,saveposition);
		while( saveposition < pack_len )
		{

			buf = getOneFrame(RX_BUF, sd);
			
			if ( (pack_len - saveposition) > FRAME_LEN )
				len = FRAME_LEN;
			else
				len = pack_len - saveposition;

			memcpy(pDest+saveposition,buf,len);
			saveposition += len;

		}
//	printf("len=%d saveposition=%d \n",len,saveposition);
			if(code2==GK_OPCUA_TIMER_TASK)
				{
				pDest = (unsigned char *)TIMER_TASK_BUF;
				printf("TIMER_TASK_BUF:\n");
				for(i=0;i<MAX_TIMER_TASK_COUNT;i++)
				printf("%.2x ",*pDest++);
			         printf("\n"); 
				
				}
       
}

void constructDirectTask( unsigned char *pBuffer )
{
	int i;
        GK_OPCUA_REQUESTHEADER *pHeader;
	GK_OPCUA_OUTTERTASK_HEADER *pOuttaskHeader;
	//OPCUA_PACK_HEADER* pHeader = (OPCUA_PACK_HEADER*)pBuffer;
	pHeader = (GK_OPCUA_REQUESTHEADER*)pBuffer;
	pOuttaskHeader = (GK_OPCUA_OUTTERTASK_HEADER *)(pBuffer+OPCUA_HEADER_BYTES);
    
        pHeader->FunCode=ntohl(pHeader->FunCode);                          //32bits
        pHeader->error=ntohl(pHeader->error);                              //32bits
        pHeader->excode=ntohl(pHeader->excode);                           //32bits
        pHeader->i32Length=ntohl(pHeader->i32Length);                     //32bits
        pHeader->NodeSource=ntohl(pHeader->NodeSource);                   //32bits
        pHeader->Timestamp.dwLowDateTime=ntohl(pHeader->Timestamp.dwLowDateTime);                     //32bits
        pHeader->Timestamp.dwHighDateTime=ntohl(pHeader->Timestamp.dwHighDateTime);                     //32bits

        pOuttaskHeader->ExecuteType=ntohs(pOuttaskHeader->ExecuteType);      //16bits
        pOuttaskHeader->SynID=ntohs(pOuttaskHeader->SynID);                  //16bits
        pOuttaskHeader->TriggerAddr=ntohl(pOuttaskHeader->TriggerAddr);      //32bits
        pOuttaskHeader->TriggerKey=ntohl(pOuttaskHeader->TriggerKey);        //32bits
        pOuttaskHeader->AddrParam=ntohl(pOuttaskHeader->AddrParam);          //32bits
        pOuttaskHeader->ParamLen=ntohl(pOuttaskHeader->ParamLen);            //32bits
        pOuttaskHeader->Reserved=ntohl(pOuttaskHeader->Reserved);            //32bits

	*((unsigned long *)&(TASK_PARA_BUF[PARANODE_COUNT][0])) = pOuttaskHeader->AddrParam;
	//*((unsigned long *)&(TASK_PARA_BUF[PARANODE_COUNT][4])) = pHeader->i32Length-OPCUA_HEADER_BYTES-OPCUA_OUTTER_TASK_HEADER_BYTES;
      

	memcpy( (void *)&(TASK_PARA_BUF[PARANODE_COUNT][4]), 
		pBuffer+OPCUA_HEADER_BYTES+OPCUA_OUTTER_TASK_HEADER_BYTES, 
		pHeader->i32Length-OPCUA_HEADER_BYTES-OPCUA_OUTTER_TASK_HEADER_BYTES);
	
	memcpy( (void *)&(TASK_PARA_BUF[PARANODE_COUNT][4+pHeader->i32Length-OPCUA_HEADER_BYTES-OPCUA_OUTTER_TASK_HEADER_BYTES]),
		(void *)&(pOuttaskHeader->TriggerAddr), 8);

	DIRECT_SETPARA_TASK.u16BufferRegion = PARANODE_COUNT;
	DIRECT_SETPARA_TASK.ulParaStart = 0;
	DIRECT_SETPARA_TASK.ulParaLen = pHeader->i32Length - OPCUA_HEADER_BYTES - OPCUA_OUTTER_TASK_HEADER_BYTES + 4;
        //DIRECT_SETPARA_TASK.ulParaLen = pHeader->i32Length - OPCUA_HEADER_BYTES - OPCUA_OUTTER_TASK_HEADER_BYTES + 8;
        printf("zs_want_len==%.4x ",DIRECT_SETPARA_TASK.ulParaLen);
        DIRECT_TASK_PARA_LEN = DIRECT_SETPARA_TASK.ulParaLen;
	printf("DIRECT_TASK_PARA_LEN==%.4x ",DIRECT_TASK_PARA_LEN);
	DIRECT_TRIGGER_TASK.i8SynFlag = pOuttaskHeader->SynID;
	DIRECT_TRIGGER_TASK.u16BufferRegion = PARANODE_COUNT;
//	DIRECT_TRIGGER_TASK.ulParaStart = DIRECT_SETPARA_TASK.ulParaLen+4;
       DIRECT_TRIGGER_TASK.ulParaStart = DIRECT_SETPARA_TASK.ulParaLen;
       printf("zs_want_start==%.4x ",DIRECT_TRIGGER_TASK.ulParaStart);
 //       DIRECT_TRIGGER_TASK.ulParaStart = 0;
	DIRECT_TRIGGER_TASK.ulParaLen = 8;
  //      DIRECT_TRIGGER_TASK.ulParaLen =4;

	printf("TASK_PARA_BUF=\n");
	for(i=0;i<32;i++)
	printf("%.2x  ",TASK_PARA_BUF[PARANODE_COUNT][i]);
	printf("\n");
	printf("DIRECT_SETPARA_TASK.u16BufferRegion=%d \n",DIRECT_SETPARA_TASK.u16BufferRegion);
	printf("DIRECT_SETPARA_TASK.ulParaLen=%ld \n",DIRECT_SETPARA_TASK.ulParaLen);
//	pPara = (unsigned long *)&(TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]);
	printf("TASK_PARA_BUF[pTask->u16BufferRegion][pTask->ulParaStart]=\n");
	for(i=DIRECT_SETPARA_TASK.ulParaStart;i<DIRECT_SETPARA_TASK.ulParaLen;i++)
	printf("%.2x ",TASK_PARA_BUF[DIRECT_SETPARA_TASK.u16BufferRegion][i]);
	printf("\n");
}

void *opcua_instrcution_request(int *p)
{
	int sd = (int)p;
	int n= 0;
	int iRead = 0;
	OPC_INT32 taskPara_len;
        int i,j;
	char *recv_buff;
        char readbuf[FRAME_LEN];
        recv_buff=readbuf;	
	GK_OPCUA_REQUESTHEADER* pHeader;
	
	currentFB = 0;
	readlen = 0;
	Ext_RecThread =0;	
	directTaskParaWP = 0;
        memset(recv_buff,0,sizeof(OPCUA_PACK_HEADER));
        readlen=0;
        while( 1 ) 
	{

	  if ( (recv_buff = getOneFrame(recv_buf,sd)) == NULL )
	   {
	        usleep(10000);
		continue;
	   }

	   pHeader = (GK_OPCUA_REQUESTHEADER*)recv_buff;
	   pHeader->FunCode=ntohl(pHeader->FunCode);                          //32bits
	   printf("pHeader->FunCode: %.8x \n",pHeader->FunCode);
	   Ext_funcode = pHeader->FunCode;
           switch( pHeader->FunCode )
		{
		/*case GK_OPCUA_QUERY://					= 0x1200, // 查询 
		{
				////xil_printf( "success parse pack,in GK_OPCUA_CREATESUBSCRIPTION, now return...\r\n" );
			
			GK_OPCUA_QUERYREQUEST* pQuery = (GK_OPCUA_QUERYREQUEST*)recv_buf;

			NodeInfo* pNode = GetNodeInfoByID( pQuery->NodeId );
			 
			if( pNode == NULL )
			{
				opcua_return_NodeNotFound( sd, recv_buf );
			}
			else
			{
				DoQueryWork( sd, recv_buf );
			}

//			    	FUN_ACK_QUERY(Buffer->NodeID);
			break;
		}
			case GK_OPCUA_CREATESUBSCRIPTION:
			{
//				////xil_printf( "success parse pack,in GK_OPCUA_CREATESUBSCRIPTION, now return...\r\n" );
				GK_OPCUA_CREATESUBSCRIPTIONRESPONSE* pSubscribe = ( GK_OPCUA_CREATESUBSCRIPTIONRESPONSE*)recv_buf;
				
				NodeInfo* pNode = GetNodeInfoByID( pSubscribe->NodeId );
				
				if( pNode == NULL )
				{
//					////xil_printf("GK_OPCUA_CREATESUBSCRIPTION:searching NodeId %d failed,NodeId doesnot exist",pSubscribe->NodeId);
					opcua_return_NodeNotFound( sd, recv_buf );
				}
				else
				{
					DoSubscribeWork( sd, recv_buf );
				}rx_len

				break;
			}
			case GK_OPCUA_CREATEMONITOREDITEMS:
			{
//				////xil_printf( "success parse pack,in GK_OPCUA_CREATEMONITOREDITEMS, now return...\r\n" );

				GK_OPCUA_CREATEMONITOREDITEMSREQUEST* pMonitor = (GK_OPCUA_CREATEMONITOREDITEMSREQUEST*)recv_buf;
				
				NodeInfo* pNode = GetNodeInfoByID( pMonitor->NodeId );
				if( pNode == NULL )
				{
//					////xil_printf("GK_OPCUA_CREATEMONITOREDITEMS:ssearch nodeid %d failed, NodeId doesnot exist",pMonitor->NodeId );
					opcu
	pHeader = (GK_OPCUA_REQUESTHEADER*)pBuffer;
	pOuttaskHeader = (GK_OPCa_return_NodeNotFound( sd, recv_buf )TIMER_TASK_BUF;
				}
				else
				{
					DoMonitorWork( sd, recv_buf );
					//opcua_return_message(sd,*Buffer,MAX_GK_OPCUA_NODE_DATA_LEN,0);
				}
			   	break;
			}
		    	case GK_OPCUA_COMMAND:
			{
//				////xil_printf( "Success parse pack, in GK_OPCUA_COMMANDTEST, now return ...\r\n" );
//				////xil_printf( "GK_OPCUA_COMMANDTEST:pNode->Base_Addr: 0x%x\r\n", g_NodeHeader->Node_BRAM_BASE_ADDR );
				GK_OPCUA_COMMANDTEST* pCommand = (GK_OPCUA_COMMANDTEST*)recv_buf;
				
				NodeInfo* pNode = GetNodeInfoByID( pCommand->CommandHeader.NodeId );
//				////xil_printf( "pNode->Base_Addr: 0x%x\r\n", pNode->Node_BRAM_BASE_ADDR );
				if( pNode == NULL )
				{
//					////xil_printf( "GK_OPCUA_COMMAND: search nodeid %d failed, NodeId doesnot exist", pCommand->CommandHeader.NodeId );
					opcua_return_NodeNotFound( sd, recv_buf );
				}
				else
				{
//					////xil_printf( "GK_OPCUA_COMMAND: search NodeId %d success\r\n", pCommand->CommandHeader.NodeId );
//					////xil_printf( "GK_OPCUA_COMMAND: Ready to judge pCommand->CommandHeader.CommandCode\r\n");
					switch( pCommand->CommandHeader.CommandCode )
					{
						case COMMAND_SENDDATA:
						{
//							////xil_printf( "GK_OPCUA_COMMAND: succeed convincing the instruction COMMAND_SENDDATA, now enter DoCommandwork() ... \r\n");
							DoCommandwork(sd, recv_buf );  //这里需要将⑼的葑装好											
//							DoCommand_SendData( sd, recv_buf );
							break;
						}
//				   		case COMMAND_JIAOSHI:
//                				{
//							DoCommand_Jiaoshi( sd, recv_buf );
//							break;
//					   	}
					}
				}
	          		break;					 
			}  */
			case GK_OPCUA_SCHEDULE:				// = 0x1111			//  时间表 
			{
				printf("this is schedule in!\n");
				pHeader->i32Length=ntohl(pHeader->i32Length);              //32bits				
				if ( pHeader->i32Length > (MAX_SCHEDULE_LEN + OPCUA_HEADER_BYTES + 4) )
					schedule_len = MAX_SCHEDULE_LEN;
				else
					schedule_len = pHeader->i32Length-OPCUA_HEADER_BYTES;
				printf("schedule_len=%4x \n",schedule_len);
				
				jointemp(read_buf,sd, recv_buff, pHeader->i32Length ,MAX_SCHEDULE_LEN,pHeader->FunCode );
				hasNewSchedule = 0xAA;
				printf("SCHEDULE_BUF:\n");
				for(i=0;i<schedule_len ;i++)
				printf("%.2x ",SCHEDULE_BUF[i]);
				printf("\n");
				printf("this is schedule out!\n");
				break;
			}		
			case GK_OPCUA_START_TASK:			// = 0x2121	
			{
				START_TASK_PARA = (unsigned short)(recv_buf[OPCUA_HEADER_BYTES]);
				hasStartTestTask = 0xAA;
				break;
			}		
			case GK_OPCUA_STOP_TASK:			// = 0x2323	
			{
				STOP_TASK_PARA = (unsigned char)(recv_buf[OPCUA_HEADER_BYTES]);
				hasStopTestTask = 0xAA;
				//inTesting = 0;

				//sem_post( p_timer_sem );		DIRECT_TRIGGER_TASK.u16TaskID = GK_OPCUA_TASK_TRIGGER;//如果MainTask正处于等待SEM状态，叫袽ainTask				
				break;
			}		
			case GK_OPCUA_TASK_PARA_1:			// = 0x4141			// 参数区1 
			case GK_OPCUA_TASK_PARA_2:			// 参数区2 
			case GK_OPCUA_TASK_PARA_3:			//参数区3 
			case GK_OPCUA_TASK_PARA_4:			// 参? 
			case GK_OPCUA_TASK_PARA_5:			// 参数区5 
			{
				int	saveposition = 0;
				int	len = 0;
				char 	*buf;
				pHeader->i32Length=ntohl(pHeader->i32Length);                     //32bits
		
				if ( pHeader->i32Length > (MAX_TASK_PARA_LEN + OPCUA_HEADER_BYTES) )
					taskPara_len = MAX_TASK_PARA_LEN;
				else
					taskPara_len = pHeader->i32Length - OPCUA_HEADER_BYTES;
		
				if ( pHeader->i32Length > FRAME_LEN )		//注：此处不能用taskPara_len来判断
					len = FRAME_LEN - OPCUA_HEADER_BYTES;
				else
					len = taskPara_len;
		
				saveToParaBUF(recv_buff+OPCUA_HEADER_BYTES, saveposition, len, pHeader->FunCode);

				saveposition += len;
				
				while( saveposition < taskPara_len )
				{
					buf = getOneFrame(read_buf, sd);
					
					if ( (taskPara_len - saveposition) > FRAME_LEN )
						len = FRAME_LEN;
					else
						len = taskPara_len - saveposition;
					
					saveToParaBUF(buf, saveposition, len, pHeader->FunCode);
					saveposition += len;
				}
				backflag=1;
				break;
			}    
			case GK_OPCUA_TIMER_TASK:			// = 0x3131			//  定时任务
			{
				printf("this is timer_task in!\n");
 				pHeader->i32Length=ntohl(pHeader->i32Length);                     //32bits				
				if ( pHeader->i32Length > (MAX_TIMER_TASK_LEN + OPCUA_HEADER_BYTES + 4) )
					timerTaskCount=MAX_TIMER_TASK_COUNT;
		
				else
					timerTaskCount = ((pHeader->i32Length - OPCUA_HEADER_BYTES - 4) / OPCUA_TASK_BYTES );
//	  			 printf("pHeader->FunCode: %.8x \n",pHeader->FunCode);
			        printf("%d ",timerTaskCount);
				jointemp(read_buf, sd, recv_buff, pHeader->i32Length, MAX_TIMER_TASK_LEN, pHeader->FunCode );
				for(i=0;i< pHeader->i32Length ;i++)
				printf("%.2x ",TIMER_TASK_BUF[i]);
				
				printf("this is timer_task out!\n");
				break;
			}		
			case GK_OPCUA_SYN_TASK:				// = 0x3232			//    同步任务
			{
				printf("this is syn_task in!\n");
//				////xil_printf( "Success parse pack, in GK_OPCUA_SYN_TASK, now return ...\r\n" );
				if ( pHeader->i32Length > (MAX_SYN_TASK_LEN + OPCUA_HEADER_BYTES + 4) )
					synTaskCount = MAX_SYN_TASK_COUNT;
				else
					synTaskCount = ((pHeader->i32Length - OPCUA_HEADER_BYTES - 4) / OPCUA_TASK_BYTES);
						
				jointemp(read_buf, sd, recv_buf, pHeader->i32Length, MAX_SYN_TASK_LEN, pHeader->FunCode );
				printf("this is syn_task out!\n");				
				break;		
			}
//			case GK_OPCUA_DIRECT_TASK:		// 0x5151
//				memcpy( &DIRECT_TASK, recv_buf+OPCUA_HEADER_BYTES, OPCUA_TASK_BYTES );
//				writeDirectTaskPara( &DIRECT_TASK, recv_buf+OPCUA_HEADER_BYTES+OPCUA_TASK_BYTES, TASK_PARA_BUF[PARANODE_COUNT] );
//				hasDirectTask = 0xAA;
//				break;	                             
			default:
                                printf("ok_0_direct!\n");
				constructDirectTask( recv_buff );
				
				hasDirectTask = 0xAA;
				
                                printf("ok_1\n");
				
				break;
	              }	
	}

}

