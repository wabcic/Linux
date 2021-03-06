#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gk_opcua_header.h"
#include "gk_opcua_types.h"
#include "types.h"
#include "msg.h"

#define	SEND_BUF_LEN	500
#define  MAX_SEND_LEN   200
char Send_BUF[MAX_SEND_LEN];
extern char uploadDI[MAX_SEND_LEN];  
extern int upDI();  
extern int backflag;
extern int array[8];

//lily
extern UINT8  g_CollectInfo[92];
extern UINT8  byadatainbuf[92];
extern UINT8  byaplatparam[92];
extern UINT8  byaprjparam1[92];
extern UINT8  byaprjparam2[92];
extern UINT8  byaprjparam3[92];

extern UINT8   datainheader[28];
extern UINT8   platparamheader[28];
extern UINT8   prjparam1header[28];
extern UINT8   prjparam2header[28];
extern UINT8   prjparam3header[28];
extern UINT8   digitalheader[28];


extern UINT8  g_CollectInfodigital;   //1553 DATA_BACK_FLAG;
extern UINT8  g_CollectInfodatain;
extern UINT8  g_CollectInfoplat;
extern UINT8  g_CollectInfoprj1;
extern UINT8  g_CollectInfoprj2;
extern UINT8  g_CollectInfoprj3;

extern int	Ext_RecThread;
typedef struct {
	OPCUA_PACK_HEADER sendHeader;
	unsigned char buf[SEND_BUF_LEN];
} SEND_TO_TM;

SEND_TO_TM send_data;

void *opcua_send_server(void *p)
{
	int sd = (int)p;
	int i;
        while(1)
	{	
		
		if(Ext_RecThread == 1)
		{
			pthread_exit(1);
		}		
	
		//lily_1553
		if(g_CollectInfodigital==1)
		{
			memcpy(g_CollectInfo,digitalheader,28);


			send(sd,(unsigned char *)g_CollectInfo,92,0);
			g_CollectInfodigital=0;

		}
		if(g_CollectInfodatain==1)
		{
			memcpy(byadatainbuf,datainheader,28);

//			printf("this is datainback.................\n");
//			for(i=0;i<64;i++)
//			{
//				printf("%x\t",byadatainbuf[i+28]);
//			}
//			printf("\n");
			
			send(sd,(unsigned char *)byadatainbuf,92,0);
			g_CollectInfodatain=0;
		}
		if(g_CollectInfoplat==1)
		{
			memcpy(byaplatparam,platparamheader,28);			
	
			send(sd,(unsigned char *)byaplatparam,92,0);
			g_CollectInfoplat=0;
		}
			
		if(g_CollectInfoprj1==1)
		{
			memcpy(byaprjparam1,prjparam1header,28);
			
//			printf("prjparam1 send PITS ........\n");
//			for(i=0;i<64;i++)
//			{
//				printf("%x\t",byaprjparam1[i+28]);				
//			}
//			printf("\n");
			send(sd,(unsigned char *)byaprjparam1,92,0);
			g_CollectInfoprj1=0;
		}
		if(g_CollectInfoprj2==1)
		{
			
			memcpy(byaprjparam2,prjparam2header,28);
						

			send(sd,(unsigned char *)byaprjparam2,92,0);
			g_CollectInfoprj2=0;
		}
		if(g_CollectInfoprj3==1)
		{
			memcpy(byaprjparam3,prjparam3header,28);		

			send(sd,(unsigned char *)byaprjparam3,92,0);
			g_CollectInfoprj3=0;
		}	
	}
}      
