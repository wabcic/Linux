/*********************************************************************
文件: msg.h
作者: 
创建: 
版本: 
功能: 1553 常用的数据结构定义
相关: 
更新: 
**********************************************************************/
#ifndef _MSG_H__
#define _MSG_H__

#include "types.h"
#include "bc.h"
typedef union																												//命令字的定义
{
	UINT16 CommandWord;
	struct
	{
		unsigned DataNum	:5;
		unsigned SubAddr	:5;
		unsigned TRFlag		:1;
		unsigned RTAddr		:5;
	} Command;
} MSG_COMMAND, *P_MSG_COMMAND, *LP_MSG_COMMAND;

typedef union																												//控制字的定义
{
	UINT16 Controlword;
	
	struct
	{
		unsigned	RT_RT_Format         : 1;
		unsigned	Broadcast_Format     : 1;
		unsigned	Mode_Code_Format     : 1;
		unsigned	                     : 2;
		unsigned	Mask_Bcst_Bit        : 1;
		unsigned	Off_Line_Self_Test   : 1;
		unsigned	Bus_Channel          : 1;
		unsigned	Retry_Enable         : 1;
		unsigned	                     : 7;
	} Control;
} MSG_CONTROL, *P_MSG_CONTROL, *LP_MSG_CONTROL;

typedef struct
{
	UINT8 MsgFormat;
	UINT8 BusFlag;
	MSG_COMMAND Command1;
	MSG_COMMAND Command2;
} MSG, *PMW_MSG, *LPMW_MSG;

typedef struct
{
	UINT8 MsgNo;
	MSG Msg;
} MSG_DEF;


typedef struct
{
	UINT8 byadatainbuf[92];
	UINT8 byaplatparam[92];
	UINT8 byaprjparam1[92];
	UINT8 byaprjparam2[92];
	UINT8 byaprjparam3[92];
} Collectinfo;

#endif /* _MSG_H__ */
