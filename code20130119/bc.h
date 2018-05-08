/*********************************************************************************************************************
document:   bc.h
×÷Õß£º
ŽŽœšÊ±Œä£º
°æ±Ÿ£º
¹ŠÄÜ£º		BC¹«¹²ÐÅÏ¢µÄ¶šÒå
Author    wabcic
**********************************************************************************************************************/

#ifndef _BC_H_
#define _BC_H_

//=======================Message define================================================================================
//¿ÕÏûÏ¢

#define		EMPTY_MESSAGE					0

//BICC =====param=============================================================
#define 	PLAT_PARAM					1
#define  	TIME_CODE					2
#define 	CONTROL_CMD					3
#define 	SYS_STATE					4	
#define		DATA_INJECT					5	

#define		DIGITAL_DECT					6 
#define		DATA_INJECT_BACK				7
#define		PLAT_PARAM_BACK					8
#define		PRJ_PARAM1					9
#define		PRJ_PARAM2					10
#define       	PRJ_PARAM3					11	
#define         CONTROL_CMD_B	 				12
#define 	VECTOR_BICC					13


//=====================================================================================================================
#define MSG_TRANSMIT						0x01
#define MSG_RECEIVE						0x00


//================================ Frame gap 640*100us=64ms =========================================================

#define FRAME_GAP    						1280
#define BEGINPERIOD						4

//=====================================================================================================================
#define RT_ADDR_BICC 						0x07
//============================message   start=============================================================================
#define LOAD_INPUT_NUM						20
#define LOAD_INPUT_START					15


//=====================================================================================================================

#define MW_MAX_CARD_COUNT					8

#define MW_1553_TYPE_BC						0x01
#define MW_1553_TYPE_RT						0x02
#define MW_1553_TYPE_MT						0x04

#define MSG_TYPE_EMPTY						0x01 
#define MSG_TYPE_BC_RT						0x02 
#define MSG_TYPE_RT_BC						0x03 
#define MSG_TYPE_RT_RT						0x04 
#define MSG_TYPE_MODE_NODATA					0x05 
#define MSG_TYPE_TX_MODE_WITHDATA				0x06 
#define MSG_TYPE_RX_MODE_WITHDATA				0x07 
#define MSG_TYPE_BCST						0x08 
#define MSG_TYPE_RT_BCST					0x09 
#define MSG_TYPE_BCST_MODE_NODATA				0x0A 
#define MSG_TYPE_BCST_MODE_WITHDATA				0x0B 
#define MSG_TYPE_NOP						0x0C 

#define MSG_BUS_FLAG_A						0x01
#define MSG_BUS_FLAG_B						0x00

#define MSG_TRANSMIT						0x01
#define MSG_RECEIVE						0x00

#define BC_RESP_TIME_18US					0x00
#define BC_RESP_TIME_22US					0x01
#define BC_RESP_TIME_50US					0x02
#define BC_RESP_TIME_130US					0x03

#define MW_RT_STATUS_SVCREQ					0x0100
#define MW_RT_STATUS_BUSY					0x0008
#define MW_RT_STATUS_SSFLG					0x0004
#define MW_RT_STATUS_DYNCBUS					0x0002
#define MW_RT_STATUS_RTFLAG					0x0001


////////////////////////////////////////////// ÏûÏ¢ÅÅÁÐ/////////////////////////////////////////////////////////////////
#define	PRE_EMPTY_MESSAGE_NUM					2
#define	VECTOR_WORD_NUM						6

#define	EMPTY_MESSAGE_START					2
#define UPLOAD_MESSAGE_POS  					8
#define SYS_MESSAGE_START 					3

#define ADD_MESSAGE_START 					11
#define ADD_MESSAGE_NUM   					32

#define	MSG_NUM_FRAME						40

/////////////////////////////////////¿ÕÏûÏ¢µÄÊ±ŒäŒäžô/////////////////////////////////////////////////////////////////////

#define		VECTOR_WORD_GAP					400
#define		EMPTY_MSG_GAP					800
#define		DATA_TRANS_GAP					800

//=============================EOM & EOF  BC END AND END OF MESSAGE======================================================
#define BC_INT_MASTER_FLAG					0x8000
#define BC_INT_EOM_FLAG						0x0001
#define BC_INT_EOF_FLAG						0x0008

#define MW_BC_MIN_FRAME_TIME					800	
#define MSG_DATA_SIZE						38


#define RT_NO_RESPONSE  					(1<<9)
#define SERVER_REQ						0x0100
//====================================RT adress=========================================================================

#define RT_ADD_KJG						5

//=====================================Signal Message Num================================================================
#define SIG_MEG_NUM_DEF						14

//==================================服务请求方式   frame start============================================================
#define SEQ_REQ_NUM						5
#define SEQ_REQ_START						10

#define SER_REQ_NUM						32
//==============================================stress test  frame start ====================================================
#define STRESS_START						20
//=====================================================RAM size===============================================================
#define SHARE_RAM_SIZE 						4096

//==========================================collect data length=====================================================================
#define COLLECT_LEN_DATAIN              64
#define COLLECT_LEN_PLATPARAM           64
#define COLLECT_LEN_PRJPARAM1           64
#define COLLECT_LEN_PRJPARAM2           64
#define COLLECT_LEN_PRJPARAM3		64

#endif		/*_BC_H_*/
