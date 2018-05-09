/*********************************************************************************************************************
function: bcdecal.c
×÷Õß£º
ŽŽœšÊ±Œä£º
°æ±Ÿ£º
¹ŠÄÜ£ºœ«²â¹Ü·¢ËÍµÄÏûÏ¢ÊýŸÝÐŽÈëBC ÏàÓŠµÄRAM ÏûÏ¢¿éÖÐ£¬¶Ô²à¹ÜÐÅÏ¢µÄŽŠÀí
Ïà¹ØÎÄŒþ£º
Author: wabcic

sendway:(uint8)--------------sendperiod(uint16)------------sendABflag(uint8)----------subaddr----------------
**********************************************************************************************************************/

#include"types.h"
#include"bc.h"
#include"stdio.h"
#include<string.h>

extern UINT32 g_dwTimeSec;
extern UINT16 g_wTimeMs;
extern UINT16 g_Sysdata[3];
extern UINT16 g_Platdata[32];
extern UINT16 g_Datain[32];
extern UINT16 *g_Dataintmp;

extern UINT8  *pPara;
extern UINT8  timecodeflag;
extern UINT16  date_gap;								// data_time_gap
extern UINT8  datainnum;
extern UINT8  datainframenum;							// datainjectNUM

extern UINT8  timecodeABflag;							//AB bus flag
extern UINT8  sysstatusABflag;
extern UINT8  platparaABflag;
extern UINT8  datainABflag;
extern UINT8  selftestABflag;
extern UINT8  controlcomABflag;
extern UINT8  stresstestABflag;

extern UINT8  timecodesendflag ;  						// time code flag 
extern UINT8  sysstatussendflag; 						// system status
extern UINT8  platparasendflag ;						// paltparam
extern UINT8  datainsendflag   ; 						// data inject 
extern UINT8  controlcomsendflag;						// control cmd 
extern UINT8  stresstestsendflag;						// stresstestflag

extern UINT8 timecodeperiod;
extern UINT8 sysstatusperiod;
extern UINT8 platparaperiod;
extern UINT8 controlcomperiod;
extern UINT8 datainperiod;

extern UINT16 g_byPeriodTimNum;					
extern UINT16 g_byPeriodSysNum;
extern UINT16 g_byPeriodPlaNum;

void timecodedeal(UINT8 *pTask)
{
	UINT16  sendperiod;
	UINT8	sendway;
	UINT8   lencontrol;
	UINT8	sendABflag;
	UINT8   i;
	
	pPara = pTask;								// 数据传递
	
	sendperiod=((UINT16)(*(pPara+6))<<8)+((UINT16)(*(pPara+7)));		//周期单位ms，必须是64ms（FRAME_GAP + 每帧时间）的倍数帧周期
	sendway     =*(pPara+5);
	lencontrol  =*(pPara+10);
	sendABflag  =*(pPara+8);

	printf("period: %d\t way: %x\t AB: %x\t generate: %x\n",sendperiod,sendway,sendABflag,lencontrol);
	
	if(sendABflag == 0xAB||sendABflag == 0xAA ||sendABflag ==0xBB)		//ABt总线标志位
	{
		timecodeABflag = sendABflag;
		if(timecodeABflag ==0xBB)					//  改变总线标志位
			ChangeControlword( TIME_CODE, MSG_BUS_FLAG_B);

		
		if ( (sendway == 0x00) && (sendperiod!=0) && (lencontrol ==0x00))			//初始时间值由测管设置
		{
			BCWriteData( TIME_CODE, SwapToWord(pPara+11,6));
			timecodeperiod = (sendperiod*10)/FRAME_GAP;
			g_byPeriodTimNum = timecodeperiod -1;
			printf("%x\n",timecodeperiod);
			timecodesendflag = 0x01;				//发送标志，标识周期不变发送
			timecodeflag=FALSE;
		}
		else if( (sendway == 0x01) && (sendperiod!=0) && (lencontrol ==0x00))				
		{
			g_dwTimeSec = 0x0000;
			g_wTimeMs   = 0x00;
			timecodeperiod = (sendperiod*10)/FRAME_GAP;
			g_byPeriodTimNum = timecodeperiod -1;
			timecodesendflag = 0x02;				// 发送标志,标识周期同步发送
		}
		else if((sendway == 0x02) && (lencontrol ==0x00))
		{
			BCWriteData( TIME_CODE, SwapToWord(pPara+11,6));
			timecodesendflag = 0x03;				//BC单次发送
		}
		else
		{
			timecodesendflag = 0x00;
		}
	}
}

void sysstatusdeal( UINT8 *p) 
{
		UINT16 sendperiod;
		UINT8 sendway;
		UINT8 sendABflag;
		UINT8 lencontrol;
		UINT8 i;
		pPara = p;

		sendperiod=((UINT16)(*(pPara+6))<<8)+((UINT16)(*(pPara+7)));				//发送包内周期为2字节
		sendway=*(pPara+5);
		sendABflag=*(pPara+8);
		lencontrol = *(pPara+10);

		printf("period: %d\t way: %x\t AB: %x\t generate: %x\n",sendperiod,sendway,sendABflag,lencontrol);

		if(sendABflag==0xAB||sendABflag==0xAA||sendABflag==0xBB)	//AB总线选择标志
		{
			sysstatusABflag=sendABflag;

			if(sysstatusABflag ==0xBB)					//  改变总线标志位
				ChangeControlword( SYS_STATE, MSG_BUS_FLAG_B);
			


			if(sendway==0x00 && (sendperiod!=0x0000) && lencontrol ==0x00)
			{
				sysstatusperiod=(sendperiod*10)/FRAME_GAP;
				g_byPeriodSysNum = sysstatusperiod-1;
				printf("%x\n",sysstatusperiod);
				BCWriteData(SYS_STATE,SwapToWord((pPara+11),6));
				sysstatussendflag=0x01;				//发送标志，标识周期不变发送
			}
			else if(sendway==0x01 && (sendperiod!=0x0000) && lencontrol ==0x00)
			{
				sysstatusperiod=(sendperiod*10)/FRAME_GAP;
				g_byPeriodSysNum = sysstatusperiod-1;
				
			
				memset(g_Sysdata,0,sizeof(g_Sysdata));
				sysstatussendflag=0x02;				//发送标志，标识周期变发送
			}
			else if(sendway==0x02 && lencontrol ==0x00)
			{	
				BCWriteData(SYS_STATE,SwapToWord((pPara+11),6));	
				sysstatussendflag=0x03;				// 仅发送一次
			}
			else 
			{
				sysstatussendflag=0x00;
			}
		}
}
void platparadeal( UINT8 *p)  						 	 //平台参数
{
		UINT16 sendperiod;
		UINT8 sendway;
		UINT8 sendABflag;
		UINT8 lencontrol;
		UINT8 i;
		pPara = p;

		sendperiod=((UINT16)(*(pPara+6))<<8)+((UINT16)(*(pPara+7)));				//发送包内周期为2字节
		sendway=*(pPara+5);
		sendABflag=*(pPara+8);
		lencontrol = *(pPara+10);

		printf("period: %d\t way: %x\t AB: %x\t generate: %x\n",sendperiod,sendway,sendABflag,lencontrol);

		if(sendABflag==0xAB||sendABflag==0xAA||sendABflag==0xBB)		//AB总线选择标志
		{
			platparaABflag=sendABflag;
			if(platparaABflag ==0xBB)					//  改变总线标志位
				ChangeControlword( PLAT_PARAM, MSG_BUS_FLAG_B);
		
			if(sendway==0x00 && (sendperiod!=0x0000) && lencontrol ==0x00)
			{
				platparaperiod = (sendperiod*10)/ (FRAME_GAP);
				g_byPeriodPlaNum = platparaperiod-1;
				BCWriteData( PLAT_PARAM, SwapToWord((pPara+11),64) );
				platparasendflag=0x01;					//发送标志，标识周期发送
			}
			else if(sendway==0x01 && (sendperiod!=0x0000) && lencontrol ==0x00)
			{
				platparaperiod = (sendperiod*10)/ (FRAME_GAP);
				g_byPeriodPlaNum = platparaperiod-1;
				
				memset(g_Platdata,0,sizeof(g_Platdata));
		//		BCWriteData( PLAT_PARAM, g_Platdata);
				platparasendflag=0x02;					//发送标志，标识单次发送
			}
			else if(sendway==0x02 && lencontrol ==0x00)
			{	
				BCWriteData(PLAT_PARAM,SwapToWord((pPara+11),64));
				
				platparasendflag=0x03;				// 仅发送一次
			}
			else 
			{
				sysstatussendflag=0x00;
			}
		}
}
void dataindeal( UINT8 *p) 							// datain
{
		UINT8 sendABflag;
		UINT8 lencontrol;
		UINT8 i;
		pPara = p;
		
		UINT16 wWritePos;
		
		sendABflag=*(pPara+5);
		lencontrol =*(pPara+7);
		datainnum = lencontrol;		
	
		printf("AB: %x\t lencontrol: %x\n",sendABflag,lencontrol);		

		if(sendABflag==0xAB||sendABflag==0xAA||sendABflag==0xBB)	// AB总线选择标志
		{
			datainABflag=sendABflag;
			if(datainABflag ==0xBB)					//  改变总线标志位
				ChangeControlword( DATA_INJECT, MSG_BUS_FLAG_B);			
		
			if(lencontrol == 0x00)					// normal send
			{	
				g_Dataintmp = (UINT16 *)SwapToWord((pPara+8),64);
//				for(i=0;i<32;i++)
//				{
//					printf("%x\t",g_Dataintmp[i]);
//				}
//				printf("\n");
			
//				wWritePos = 0x108 + DATA_INJECT * 38+2;
//				WriteBlock(wWritePos,g_Dataintmp,32);

				BCWriteData( DATA_INJECT, g_Dataintmp);
				datainsendflag=0x01;				
			}
			else							// abnormal send
			{
				datainsendflag=0x00;
			}
		}
}

void controlcmddeal( UINT8 *p) 
{
		
		UINT8 lencontrol;
		UINT8 sendABflag;
		pPara = p;

		sendABflag =  *(pPara+5);
		lencontrol =  *(pPara+7);		

		printf("AB: %x\t generate: %x\n",sendABflag,lencontrol);
		if(sendABflag==0xAB||sendABflag==0xAA||sendABflag==0xBB)		//AB总线选择标志
		{
			controlcomABflag=sendABflag;
			if(controlcomABflag ==0xBB)					//  改变总线标志位
				ChangeControlword( CONTROL_CMD, MSG_BUS_FLAG_B);				

			if(lencontrol ==0x00)
			{
											//发送标志，突发式正常发送
				BCWriteData( CONTROL_CMD, SwapToWord((pPara+8),4));
				controlcomsendflag=0x01;
			}
			else 								// 异常不发送
			{
				controlcomsendflag=0x00;
			}				
		}			
}


void stresstest(UINT8 *p)
{
	
	UINT8 sendABflag;
	UINT8 timegapcase;
	UINT8 i;
	pPara = p;
	UINT16 *stressdatatmp;
	
	sendABflag = *(pPara+5);
	timegapcase  = *(pPara+7);				// date_gap				
	datainframenum  = *(pPara+8);				// message num 
	
	printf("AB: %x\t DATA_GAP: %x\t messagenum: %x\n",sendABflag,date_gap,datainframenum);
	
	if(sendABflag==0xAB||sendABflag==0xAA||sendABflag==0xBB)
	{
		stresstestABflag = sendABflag;
		if(controlcomABflag ==0xBB)					//  改变总线标志位
		{			
			for(i=0;i<datainframenum;i++)	
				ChangeControlword( LOAD_INPUT_START+i, MSG_BUS_FLAG_B);
		}
		
		stressdatatmp  = (UINT16 *)SwapToWord((pPara+9),64);
		BCWriteData(LOAD_INPUT_START,stressdatatmp);
		memcpy(g_Datain,stressdatatmp,64);
		
		switch(timegapcase)
		{
			case 0x00:	
				date_gap = 800;
				stresstestsendflag = 0x01;
				break;		
			case 0x01:
				date_gap = 32000;
				stresstestsendflag = 0x01;
				break;
			case 0x02:
				date_gap = 30000;
				stresstestsendflag = 0x01;
				break;
			default:
				stresstestsendflag = 0x00;
				break;
		}
		
	}
}
