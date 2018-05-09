#include"bc.h"
#include"msg.h"
#include"pdata.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "test.h"
#include "testlib.h"

void BCDefFrame( UINT8* MsgNoArray, UINT16* MsgGapArray, int Count )
{
	int i;

	for ( i = 0; i < Count; i++ )
	{
		WriteRam( i * 4 + 0x00, 0 );
		WriteRam( i * 4 + 0x01, 0 );
		WriteRam( i * 4 + 0x02, MsgGapArray[i] );			// msg gap 
		WriteRam( i * 4 + 0x03, 0x108 + MsgNoArray[i]*38 );		// message address
	}

	WriteRam( 0x100, 0 );
	WriteRam( 0x101, 0xFFFF - Count );

	WriteRam( 0x102, 0 );
	WriteRam( 0x103, 0xFFFF - Count );					// message counter
}



void BCRun()
{
	WriteReg( 3, 0x02 );	
}

/*********************************************************************
功能: 
描述: 
参数: 
返回: 
注意: 
**********************************************************************/
void BCGetMsgBlockPos( UINT8 MsgNo, UINT16* pwWriteData,UINT16* pwReadData, UINT16* pwSize, UINT16* pwStatus )
{
	MSG_CONTROL msgControl;
	MSG_COMMAND msgCommand;
	
	UINT16 wBase;
	UINT16 wSize;
	UINT16 wWriteData;
	UINT16 wReadData;
	UINT16 wStatus;
	
	wBase = 0x108 + MsgNo * 38;

	msgControl.Controlword = ReadRam( wBase );

	msgCommand.CommandWord = ReadRam( wBase + 1 );

	wSize = msgCommand.Command.DataNum;

	wWriteData = 0;
	wReadData  = 0;
	wStatus    = 0;

	if ( msgControl.Control.RT_RT_Format )
	{
		wWriteData = wBase + 5;
		wReadData  = wBase + 5;
		wStatus    = wBase + 4;
	}
	else if ( msgControl.Control.Mode_Code_Format )
	{
		if ( msgCommand.Command.DataNum >= 0x10 )
		{
			if ( msgCommand.Command.TRFlag == MSG_RECEIVE )
			{
				if ( !msgControl.Control.Broadcast_Format )
					wStatus = wBase + 4;
				wWriteData = wBase + 2;
				wSize = 1;
			}
			else
			{
				wStatus   = wBase + 3;
				wReadData = wBase + 4;
				wSize     = 1;
			}
		}
		else
		{
			if ( !msgControl.Control.Broadcast_Format )
				wStatus = wBase + 3;
		}
	}
	else if ( msgCommand.CommandWord )
	{
		if ( !msgControl.Control.Broadcast_Format )
		{
			if ( msgCommand.Command.TRFlag == MSG_RECEIVE )
			{
				wWriteData = wBase + 2;

				if ( wSize )
					wStatus = wBase + wSize + 3;
				else
					wStatus = wBase + 32 + 3;
			}
			else
			{
				wReadData = wBase + 4;
				wStatus   = wBase + 3;
			}
		}
		else
		{
			wWriteData = wBase + 2;
		}
	}
	
	if ( pwWriteData )
		*pwWriteData = wWriteData;
	if ( pwReadData )
		*pwReadData  = wReadData;
	if ( pwSize )
		*pwSize      = wSize;
	if ( pwStatus )
		*pwStatus    = wStatus;
}

//=========================================get sysTime=====================================
void SysTimeGet(UINT8 *pbyData)
{
	if(pbyData !=NULL)
	{
		*pbyData      =(UINT8)(g_dwTimeSec>>24);
		*(pbyData+1)  =(UINT8)(g_dwTimeSec>>16);
		*(pbyData+2)  =(UINT8)(g_dwTimeSec>>8);
		*(pbyData+3)  =(UINT8) g_dwTimeSec;
		*(pbyData+4)  =(UINT8)(g_wTimeMs>>8);
		*(pbyData+5)  =(UINT8)g_wTimeMs;
		
	}
}

//==========================================================================================

void BCWriteData(UINT8 MsgNo,UINT16 *DataBuf)
{
	UINT16 wWritePos;
	UINT16 wSize;
	
	BCGetMsgBlockPos(MsgNo,&wWritePos,NULL,&wSize,NULL);	
	
	if(wSize ==0 || wSize > 32)
	{
			wSize = 32;
	}
	
//	printf("Pos = %x\n", wWritePos);

	WriteBlock(wWritePos,DataBuf,wSize);			
	
}

void BCReadData(UINT8 MsgNo,UINT16 *DataBuf)
{

	UINT16 wReadPos;
	UINT16 wSize;
	
	BCGetMsgBlockPos(MsgNo,NULL,&wReadPos,&wSize,NULL);	//
	
	if(wSize ==0 || wSize > 32)
	{
			wSize = 32;
	}
	
	ReadBlock(wReadPos,DataBuf,wSize);			//


}


UINT16 BCGetCtrlword(LPMW_MSG Msg)
{
	MSG_CONTROL objControlword;

	objControlword.Controlword = 0x180;
	
																										
	if ( Msg->MsgFormat == MSG_TYPE_EMPTY )
	{
		objControlword.Control.Off_Line_Self_Test = 1;		
	}
	else
	{
		if ( Msg->Command2.CommandWord != 0 )		
		{
			objControlword.Control.RT_RT_Format = 1;
		}

		if ( Msg->Command1.Command.RTAddr == 31 )						//¹ã²¥Ä£Êœ
		{
			objControlword.Control.Broadcast_Format = 1;
		}

		if ( Msg->Command1.Command.SubAddr == 0 || Msg->Command1.Command.SubAddr == 31 )	//Ä£ÊœÂëÑ¡Ôñ
		{
			objControlword.Control.Mode_Code_Format = 1;
		}

		objControlword.Control.Bus_Channel = Msg->BusFlag;
	}

	return objControlword.Controlword;
}


void BCDefMsg( UINT8 MsgNo, LPMW_MSG Msg )
{
	UINT16 waZeroData[35];
	UINT16 wDataPos;
	UINT16 wControlword;
	
	wDataPos = 0x108 + MsgNo * 38;
	wControlword = BCGetCtrlword(Msg);
	
	WriteRam( wDataPos, wControlword );
	WriteRam( wDataPos + 1, Msg->Command1.CommandWord );
	WriteRam( wDataPos + 2, Msg->Command2.CommandWord );

	memset(waZeroData,0,35*sizeof(UINT16));
	WriteBlock(wDataPos+3,waZeroData,35);
}

void BC1553Binit()
{
 
	g_CollectInfodigital = 0;
	
 	normalflag = 1;

 	timecodeflag = TRUE;
 	
	timecodesendflag  = 0x05;  				// time code
// 	timecodesendflag  = 0x02;  				// time code						
 	sysstatussendflag = 0x05; 				// systeme status
 	platparasendflag  = 0x05;				// plat param	
 	datainsendflag     = 0;    				// data inject
	controlcomsendflag = 0;					// control cmd 
	stresstestsendflag = 0;					// stressflag


 	selftestsendflag= 1;  				
	synyaocesendflag=0;					
	syndatainsendflag=0;				
	ex_synyaocesendflag = 0;

	synyaoce = 0x03;
	syndatain = 0x09;

	date_gap   = 0;						// date_gap				
	datainnum  = 0;						// datainnum
											
 	frameperiodNum=0;
											 
 	g_byPeriodNum 	 = 0;										
 	g_byBigPeriodNum = 0;
	g_byPeriodTimNum = 0;
	g_byPeriodSysNum = 0;
	g_byPeriodPlaNum = 0;
 			
	platparaperiod 	= 4;			//512ms, 每帧周期128ms
	sysstatusperiod = 4;			//系统状态 512ms 每楨周期128
	timecodeperiod 	= 4;			//时间码发送周期
					
}

UINT8 BCInit(void)
{	
	UINT16 i;
	UINT8 bRet;
	
	bRet = TRUE;
	
	WriteReg(3,0x0001);			//1: start/reset: RESET
	
	for(i=0;i<SHARE_RAM_SIZE;i++)									
	{
		WriteRam(i,0);
	}
	for ( i = 0; i < SHARE_RAM_SIZE; i++ )
	{
		if ( ReadRam( i ) != 0 )
			bRet = FALSE;
	}						
	WriteReg( 7, 0x8000 );			/* 2 CR#3 ÔöÇ¿Ä£Êœ */
	WriteReg( 0, 0x0019 );			/* 3 Enable interrupts for EOM and EOF */
	WriteReg( 2, 0x0498 );			/* 4 CR#2 256_UINT16 boundry disable,interrupt status auto clear, *edge interrupt trigger */
	WriteReg( 8, 0x1160 );			/* 5 CR#4 Enables EXPANDED BC CONTROL UINT16, VALID BUSY/NO DATA,*  
						VALID MESSAGE ERROR/NO DATA, first retry alternate*  
						0x1060 -> 0x1160  */
	WriteReg( 5, 0x0000 );			/* 6 Time-Tag Counter Register */
	WriteReg( 1, 0x0160 );			/* 7 CR#1 Auto-Repeat Mode, MESSAGE GAP TIMER ENABLE */
	WriteReg( 9, 0x0800 );			/* 8 CR#5 EXPANDED CROSSING ENABLED */
	
	g_wBCCurBufPos = 0;
	g_dwBCFrames = 0;
	g_dwBCFrameCouter = 0;	

	return bRet;
}

void BCSetMsgList( void )				// 初始化11个单个消息
{
	MSG msg;
	UINT16 i;	
	memset( &msg, 0, sizeof(MSG));

	for ( i = 0; i < SIG_MEG_NUM_DEF; i++ )
	{
		msg = g_MsgBuf[i].Msg;
		BCDefMsg(g_MsgBuf[i].MsgNo, &msg);
	}
	
	for ( i = 0; i < LOAD_INPUT_NUM; i++ )			// data_inject number
	{
		msg.BusFlag = MSG_BUS_FLAG_A;
		msg.Command1.Command.DataNum = 0;
		msg.Command1.Command.SubAddr = 2;
		msg.Command1.Command.RTAddr = RT_ADDR_BICC;
		msg.Command1.Command.TRFlag = 0;
		msg.Command2.CommandWord = 0;
		msg.MsgFormat = MSG_TYPE_BC_RT;	
		BCDefMsg( LOAD_INPUT_START + i, &msg );		
	}
}

//==================================================================
void BCSetMsgInFrame( int MsgIndex, UINT8 MsgNo, UINT16 MsgGap )
{
		
	WriteRam( MsgIndex * 4 + 0x03, 0x108 + MsgNo * 38 );
	
	WriteRam( MsgIndex * 4 + 0x02, MsgGap );
	
}

//=============set Frame interal time===============================

void BCSetFrameProp( UINT32 FrameTime, UINT32 Count )
{
	WriteReg( 13, (UINT16)(FrameTime));		
	g_dwBCFrames = Count;
}

//===========set Register 9 respone timeout select=================

void BCSetRespTimeOut( UINT8 TimeOutOption )
{
	WriteReg( 9, ((UINT16)(TimeOutOption) << 9) | (ReadReg(9) & 0xF9FF) );
}
 
//============set Frame auto repeat===========================

void BCSetRetryOption( UINT8 Retry, UINT8 DoubleRetry )
{
	UINT16 wBits = 0;
	if ( Retry )
	{
		wBits = 0x10;
	}

	if ( DoubleRetry )
	{
		wBits |= 0x08;
	}
	
	WriteReg( 1, (ReadReg(1) & 0xFFE7) | wBits );

}



UINT16* SwapToWord( UINT8* pbyData, UINT32 dwSize )
{	
	UINT16 wTemp;
	UINT32 i;
	UINT16 * pwTemp;
	
	pwTemp = (UINT16*)pbyData;
	
	for ( i = 0; i < dwSize/2; i++ )
	{
		wTemp = ( (UINT16)(*(pbyData+i*2)) << 8 ) + ( (UINT16)(*(pbyData+i*2+1))  );
		*(pwTemp+i) = wTemp;
	}
	return pwTemp;	

}

UINT8* SwapToByte( UINT16* pwData, UINT32 dwSize )
{
	UINT16 wTemp;
	UINT32 i;
	UINT8* pbyTemp;
	
	pbyTemp = (UINT8*)pwData;
	
	for ( i = 0; i < dwSize; i++ )
	{
		wTemp = *(pwData + i);
		*(pbyTemp+i*2) = (UINT8)( wTemp >> 8 );
		*(pbyTemp+1+i*2) = (UINT8)wTemp;
	}
	return pbyTemp;
}

void BCCreatFrame( void )
{  
	UINT8 i;
	
	UINT8	 byaMsgNo[MSG_NUM_FRAME];
	UINT16   waMsgGap[MSG_NUM_FRAME];
	UINT8	 sysTimeCode[6];				
	for ( i = 0; i < PRE_EMPTY_MESSAGE_NUM; i++ )
	{
		byaMsgNo[i]   = EMPTY_MESSAGE;
		waMsgGap[i]   = EMPTY_MSG_GAP;
	}
	for ( i = EMPTY_MESSAGE_START; i < MSG_NUM_FRAME; i++ )
	{
		byaMsgNo[i] = EMPTY_MESSAGE;
		waMsgGap[i] = EMPTY_MSG_GAP;
	}	
	BCDefFrame( byaMsgNo, waMsgGap, MSG_NUM_FRAME );			// period 0
										// empty message initial
	SysTimeGet( sysTimeCode );						// get timecode 						
	
	if(timecodesendflag  == 0x04)
	{
		BCWriteData( TIME_CODE, SwapToWord(sysTimeCode, 6) );			// time code 

		BCSetMsgInFrame( SYS_MESSAGE_START+6, TIME_CODE, DATA_TRANS_GAP );	// time message to stack
		
		memset(g_Sysdata,0,sizeof(g_Sysdata));

		BCWriteData(SYS_STATE, g_Sysdata);
			
		BCSetMsgInFrame( SYS_MESSAGE_START+1, SYS_STATE, DATA_TRANS_GAP );	// plant to plant_frame 
	
		memset(g_Platdata,0,sizeof(g_Platdata));		
										
		BCWriteData( PLAT_PARAM, g_Platdata);
	
		BCSetMsgInFrame( SYS_MESSAGE_START+2, PLAT_PARAM, DATA_TRANS_GAP );	// plant to plant_frame	
	}
	BCSetFrameProp( FRAME_GAP, 0 );						// frame gap = 64ms 	

	BCSetRespTimeOut( BC_RESP_TIME_18US );					// (0 18us, 1 22us, 2 50us, 3 130us)

	BCSetRetryOption( 1, FALSE );						// Frame atuo repeat(0 non-retry, 1 retry) 

	g_byServUsedNum = 0;
	g_byAddUsedNum = 0;

	g_byPeriodNum  = 0;	
	g_byBigPeriodNum = 0;
	g_byPeriodTimNum = 0;
	g_byPeriodSysNum = 0;
	g_byPeriodPlaNum = 0;
	
}  


/*********************************************************************
功能: 修改某个消息的控制字，主要是修改A/B总线
描述: 
参数: byMsgNo 消息号; byBusFlag A/B总线
返回: 
注意: 
**********************************************************************/ 
void ChangeControlword( UINT8 byMsgNo, UINT8 byBusFlag )
{
	UINT16 wDataPos;
	MSG_CONTROL objControlword;

	wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
	objControlword.Controlword = ReadRam( wDataPos );
	objControlword.Control.Bus_Channel = byBusFlag;
	WriteRam( wDataPos, objControlword.Controlword );	
}

/*********************************************************************
功能: 服务请求消息的数据存
描述: 
参数: 
返回: 
注意: 
**********************************************************************/ 

void savebackdata( UINT8 byMsgNo )
{
	UINT8 i;
	UINT16  wDataPos;
	MSG_COMMAND msgCommand;
	UINT16 g_collectinfodataintmp[32];
	UINT16 g_collectinfoplattmp[32];
	UINT16 g_collectinfoprj1tmp[32];
	UINT16 g_collectinfoprj2tmp[32];
	UINT16 g_collectinfoprj3tmp[32];
	
	switch (byMsgNo)
	{
		case DATA_INJECT_BACK:
		{
//			BCReadData(DATA_INJECT_BACK,g_collectinfodataintmp);
			wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
			msgCommand.CommandWord = ReadRam( wDataPos + 1 );
			
			ReadBlock( wDataPos+4, (UINT16 *)g_collectinfodataintmp,32 );
			if(msgCommand.Command.SubAddr == 3)			
			{	
				
				byadatainbufdata = (UINT8 *)SwapToByte(g_collectinfodataintmp,32); 
			
				memcpy((void *)(byadatainbuf+28),(void *)byadatainbufdata,64);
	
			//	printf("data_inject_back\n");
			//	for(i=0;i<64;i++)
			//	{
			//		printf("%x\t",byadatainbufdata[i]);				
			//	}
			//	printf("\n");
				
			//	memset(byadatainbufdata,0,64);
			//	memset(g_collectinfodataintmp,0,64);
				g_CollectInfodatain =1;				
				
			}
			break;
		}
		case PLAT_PARAM_BACK:
		{
	//		BCReadData(PLAT_PARAM_BACK,g_collectinfoplattmp);				
			wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
			msgCommand.CommandWord = ReadRam( wDataPos + 1 );
		
			ReadBlock( wDataPos+4, (UINT16 *)g_collectinfoplattmp,32 );
			if(msgCommand.Command.SubAddr == 5)
			{
				byaplatparamdata = (UINT8 *)SwapToByte(g_collectinfoplattmp,32); 

				memcpy((void *)(byaplatparam+28),(void *)byaplatparamdata,64);
				
			//	memset(byaplatparamdata,0,64);
			//	memset(g_collectinfoplattmp,0,64);
				g_CollectInfoplat =1;				

			}
			break;
		}
		case PRJ_PARAM1:
		{
//			BCReadData(PRJ_PARAM1,g_collectinfoprj1tmp);
			wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
			msgCommand.CommandWord = ReadRam( wDataPos + 1 );
			
			ReadBlock( wDataPos+4, (UINT16 *)g_collectinfoprj1tmp,32 );
			if(msgCommand.Command.SubAddr == 1)
			{
				byaprjparam1data = (UINT8 *)SwapToByte(g_collectinfoprj1tmp,32); 
			
				memcpy((void *)(byaprjparam1+28),(void *)byaprjparam1data,64);

			//	printf("prjparam1 back ........\n");
			//	for(i=0;i<32;i++)
			//	{
			//		printf("%x\t",g_collectinfoprj1tmp[i]);				
			//	}
			//	printf("\n");			
				
				//memset(byaprjparam1data,0,64);
				//memset(g_collectinfoprj1tmp,0,64);
				g_CollectInfoprj1 =1;					
			}
			break;
		}
		case PRJ_PARAM2:
		{
//			BCReadData(PRJ_PARAM2,g_collectinfoprj2tmp);
			wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
			msgCommand.CommandWord = ReadRam( wDataPos + 1 );			
			
			ReadBlock( wDataPos+4, (UINT16 *)g_collectinfoprj2tmp,32 );
			if(msgCommand.Command.SubAddr == 2)
			{				
				byaprjparam2data = (UINT8 *)SwapToByte(g_collectinfoprj2tmp,32); 

				memcpy((void *)(byaprjparam2+28),(void *)byaprjparam2data,64);

			//	memset(byaprjparam2data,0,64);
			//	memset(g_collectinfoprj2tmp,0,64);
				
				g_CollectInfoprj2 =1;	
			}
			break;
		}
		case PRJ_PARAM3:
		{
//			BCReadData(PRJ_PARAM3,g_collectinfoprj3tmp);
			wDataPos = 0x108 + byMsgNo * MSG_DATA_SIZE;
			msgCommand.CommandWord = ReadRam( wDataPos + 1 );
		
			ReadBlock( wDataPos+4, (UINT16 *)g_collectinfoprj3tmp,32 );
			if(msgCommand.Command.SubAddr == 4)
			{
				byaprjparam3data = (UINT8 *)SwapToByte(g_collectinfoprj3tmp,32);
 
				memcpy((void *)(byaprjparam3+28),(void *)byaprjparam3data,64);

				//memset(byaprjparam3data,0,64);
				//memset(g_collectinfoprj3tmp,0,64);
				
				g_CollectInfoprj3 =1;
			}
			break;
		}	
		default: 
			break;			
	}
}

UINT8 BCGetMsgNo( UINT8* pbyMsgNo, UINT16* pwBlockStatus )
{
	UINT16 wNewPos;
	UINT16 wMsgAddr;
	UINT8 bRet;
	
	bRet = FALSE;

										/* ERROR check */
	if ( pbyMsgNo != NULL || pwBlockStatus != NULL )
	{
										/*if stack pointer >message common ===reset */
		wNewPos = ReadRam( 0x100 );
		if ( wNewPos >= (0xFFFF - ReadRam( 0x103 )) * 4 )										
		{			
			wNewPos = 0;
		} 		
			
		if ( g_wBCCurBufPos != wNewPos )
		{
										/* get current block status */
			*pwBlockStatus = ReadRam( g_wBCCurBufPos + 0 );
										/* EOM check & all check */
			if ( *pwBlockStatus & 0x8000 )
			{
										/* get message address */
				wMsgAddr = ReadRam( g_wBCCurBufPos + 3);	
				*pbyMsgNo = (wMsgAddr - 0x108) / 38;		/* count message number */
				g_wBCCurBufPos += 4;
										/* update next message pointer */
				bRet = TRUE;
			}					
		}
										/* EOM enable interrupt  */
		if ( g_wBCCurBufPos >= (0xFFFF-ReadRam( 0x103 )) * 4 )
		{			
			g_wBCCurBufPos = 0;
		}		
	}	
	return bRet;	
}


