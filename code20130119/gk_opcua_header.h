/*
*
*  0069项目
*  简化后的OPC UA协议数据定义文件
*
*  Created by deyi.gong on 2009-10-19.
*  Copyright 2009 gkhy.com.cn. All rights reserved.
*
*/

#ifndef GK_OPC_UA_HEADER_H
#define GK_OPC_UA_HEADER_H
#define  GK_OPC_UA_PLATFORM_X86

#include "gk_opcua_types.h"

#define		MAX_GK_OPCUA_REQUEST_LEN	32*1024	/* 请求包最大长度 */	
#define		MAX_GK_OPCUA_RESPONSE_LEN	32*1024	/* 应答包最大长度 */	
#define		MIN_GK_OPCUA_REQUEST_LEN	32		/* 请求包最小长度 */	
#define		MIN_GK_OPCUA_RESPONSE_LEN	32		/* 应答包最小长度 */	
#define		MAX_GK_OPCUA_BUFFER_LEN    2*1024               /*通信栈缓冲长度*/

#define		MAX_GK_OPCUA_RESERVE		4		/* 最大保留字节数 */	
#define		MAX_GK_OPCUA_TASK			32*4	/* 最大任务数 */
#define		MAX_GK_OPCUA_TASK_PARAM		32*2	/* 任务中参数最大长度 */
#define		MAX_GK_OPCUA_TIME_LIST		32*4	/* 最大时间列表数 */

#define		MAX_GK_OPCUA_IPADDRESS_LEN	16		/* 最大IP地址字串长度 */


#define		MAX_TIMELIST_LEN		128

	//功能定义
	typedef enum _GK_OPCUA_FUNC_CODE
	{
			/* 任务管理   0x1000 --- 0x11FF */
			GK_OPCUA_CALL					= 0x1000,		/* 预置任务 */
			GK_OPCUA_CLEARTASKLIST			= 0x1001,		/* 清空任务列表 */
			GK_OPCUA_SETTIMELIST			= 0x1002,		/* 设置主控前端触发时间表 */
			GK_OPCUA_SETTIME				= 0x1003,		/* 校时 */
// 			//任务种类
// 			GK_OPCUA_STARTTEST       = 0xA001,//开始测试
// 			GK_OPCUA_STOPTEST        = 0xA002,//结束测试
// 			GK_OPCUA_SYNCHRONIZATIONTRIGGER = 0xA003,//同步触发
// 			GK_OPCUA_TIMETRIGGER           = 0xA004,//定时触发
// 			GK_OPCUA_PARAMNODE           = 0xA005,//参数存储区

			/* 时间表 */
			GK_OPCUA_SCHEDULE = 0x1111,

			/* “启动测试”直接任务 */
			GK_OPCUA_START_TASK = 0x2121,

			GK_OPCUA_STOP_TASK = 0x2323,

			/* 定时任务 */
			GK_OPCUA_TIMER_TASK	= 0x3131,

			/* 同步任务 */
			GK_OPCUA_SYN_TASK	= 0x3232,
			
			/* perform task directly */
			GK_OPCUA_DIRECT_TASK = 0x1500,
			
			/*命令*/
			GK_OPCUA_COMMAND				= 0x1100,		/*命令*/
			
			/* 查询		  0x1200 --- 0x12FF */
			GK_OPCUA_QUERY					= 0x1200,		/* 查询 */
			/* 任务参数 */
			GK_OPCUA_TASK_PARA_1 = 0x4141,				/* PARA NODE 1 */
			GK_OPCUA_TASK_PARA_2 = 0x4242,				/* PARA NODE 2 */
			GK_OPCUA_TASK_PARA_3 = 0x4343,				/* PARA NODE 3 */
			GK_OPCUA_TASK_PARA_4 = 0x4444,				/* PARA NODE 4 */
			GK_OPCUA_TASK_PARA_5 = 0x4545,				/* PARA NODE 5 */

			//发送参数任务（内部任务）
			GK_OPCUA_TASK_SEND_PARAMETER = 0x5001,
			//发送触发任务（内部任务）
			GK_OPCUA_TASK_TRIGGER = 0x5002,
			
			/* 订阅		  0x1300 --- 0x13FF */
			GK_OPCUA_CREATESUBSCRIPTION		= 0x1300,		/* 新建订阅 */
			GK_OPCUA_DELETESUBSCRIPTIONS	= 0x1301,		/* 删除订阅 */
			GK_OPCUA_SUBSCRIPTIONCONTENT = 0x1302,		/*订阅返回内容*/
			
			/* 监视		  0x1400 --- 0x14FF */
			GK_OPCUA_CREATEMONITOREDITEMS	= 0x1400,		/* 新建监视 */
			GK_OPCUA_SETTRIGGERING			= 0x1401,		/* 启动监视 */
			GK_OPCUA_DELETEMONITOREDITEMS	= 0x1402,		/* 删除监视 */
			GK_OPCUA_MONITORCONTENT = 0x1403 /*监视返回内容*/
	}GK_OPCUA_FUNC_CODE;

	typedef enum _GK_OPCUA_ERROR
	{
		GK_OPCUA_NOERROR =0x0,                 /*没有错误*/
		GK_OPCUA_NOTFOUNDNODEID =0x1000,/*没有发现客户端请求的节点*/
		GK_OPCUA_OTHERERROR = 0x2000         /*其他错误*/
	}GK_OPCUA_ERROR;
	/****************************************************************************/	
	/*							通信栈层数据定义								*/
	/****************************************************************************/	

	/* 时间定义 */
	typedef struct _OPCUA_DATETIME 
	{
		OPC_UINT32 dwLowDateTime;
		OPC_UINT32 dwHighDateTime;
	}OPCUA_DATETIME;	
	/************************************************************************/
	/* 包头定义：把原来的请求头和响应头合并到一起。      */
	/************************************************************************/
	typedef struct _OPCUA_PACK_HEADER
	{
		OPC_UINT32		FunCode; /* 请求功能码 */
		OPC_UINT32          error; /*错误代码*/
		OPC_UINT32				excode; /*附加码(暂用于标识发送的请求)*/
		OPC_INT32 		   	    i32Length;		/* 整个包的长度(包括包头和数据) */
		OPC_UINT32				NodeSource;		/* 发送节点ID */
		OPCUA_DATETIME      Timestamp;		/* 通信始发时间戳 */
	} OPCUA_PACK_HEADER;
	/*============================================================================
	* 公用请求包定义
	*===========================================================================*/
	typedef OPCUA_PACK_HEADER GK_OPCUA_REQUESTHEADER;
	typedef OPCUA_PACK_HEADER GK_OPCUA_RESPONSEHEADER;

	/*============================================================================
	* 外部任务包头定义
	*===========================================================================*/
	typedef struct _GK_OPCUA_OUTTERTASK_HEADER
	{
		OPC_UINT16  ExecuteType;//外部任务类型
		OPC_UINT16  SynID;          //同步ID
		OPC_UINT32 TriggerAddr; //触发地址
		OPC_UINT32 TriggerKey;      //触发钥匙
		OPC_UINT32 AddrParam;   //参数地址
		OPC_UINT32 ParamLen;     //参数长度
		OPC_UINT32 Reserved;     //保留
	}GK_OPCUA_OUTTERTASK_HEADER;
	
	#define GK_OPCUA_REQUESTHEADER_LEN sizeof(GK_OPCUA_REQUESTHEADER)	

	/*============================================================================
	* 公用应答包定义
	*===========================================================================*/
	#define GK_OPCUA_RESPONSEHEADER_LEN sizeof(GK_OPCUA_RESPONSEHEADER)

	/*============================================================================
	* 订阅通知消息定义
	*===========================================================================*/
	typedef struct _GK_OPCUA_NOTIFICATIONMSG
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32				i32SequenceNumber;	/* 序列号 */
		OPCUA_DATETIME			PublishTime;		/*　触发时间 */
		OPC_INT32				i32MsgLength;		/* 消息长度 */
	//	OPC_CHAR				*pMsgData;			/* 消息内容 */	
	}GK_OPCUA_NOTIFICATIONMSG;

	/****************************************************************************/	
	/*							应用层数据定义									*/
	/****************************************************************************/	
	/*触发条件*/
	typedef enum _GK_OPCUA_TRIGGERINGCONDITION
	{
		OpcUa_Triggering_Synchronization = 0,	/* 同步触发 */
		OpcUa_Triggering_immediately	 = 1,	/* 立即触发 */
		OpcUa_Triggering_monitor		 = 2,	/* monitor触发 */
		OpcUa_Triggering_Timer           =3   //定时触发
	}GK_OPCUA_TRIGGERINGCONDITION;


	/*============================================================================
	*  任务数据结构
	*===========================================================================*/
#pragma pack(1)
	typedef struct _GK_OPCUA_TASK{
		unsigned char   byTaskNO;//任务编号
		unsigned short u16TaskID;   //任务ID
		unsigned char	i8SynFlag;
		unsigned short u16BufferRegion;//参数存储缓冲区号
		unsigned long ulParaStart;//本任务参数开始纸
		unsigned long ulParaLen; //本任务参数长度
	} GK_OPCUA_TASK;
#pragma pack()
	/*============================================================================
	* 时间列表数据结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_TIMELIST{
		OPCUA_DATETIME executeTime;
		OPC_UINT32 ulTaskNO;//时间表任务号
	} GK_OPCUA_TIMELIST;


	/*============================================================================
	* 开始任务请求
	*===========================================================================*/
	typedef struct _GK_OPCUA_STARTTASKREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT16                           ui16StartTask;
	} GK_OPCUA_STARTTASKREQUEST;
	/*============================================================================
	* 结束任务请求
	*===========================================================================*/
	typedef struct _GK_OPCUA_STOPTASKREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT8                           ui8StopTask;
	} GK_OPCUA_STOPTASKREQUEST;
	/*============================================================================
	* 预置任务请求
	*===========================================================================*/
	typedef struct _GK_OPCUA_CALLREQUEST
	{
		GK_OPCUA_REQUESTHEADER  RequestHeader;
		//OPC_UINT32            uiNodeID;                    //
		OPC_INT32				i32TaskCount;				/* 有效任务总数 */
		GK_OPCUA_TASK		pTask[MAX_GK_OPCUA_TASK];	/* 任务列表 */
	}GK_OPCUA_TASKREQUEST;//GK_OPCUA_CALLREQUEST
	/*============================================================================
	* 时间列表请求
	*===========================================================================*/
	typedef struct _GK_OPCUA_TIMELISTREQUEST{
		GK_OPCUA_REQUESTHEADER  RequestHeader;
		OPC_UINT32        ui32TimeListCount;//时间表数量
		GK_OPCUA_TIMELIST pTimeList[MAX_TIMELIST_LEN];//时间列表
	} GK_OPCUA_TIMELISTREQUEST;

	/*============================================================================
	* 预置任务应答
	*===========================================================================*/
	typedef struct _GK_OPCUA_CALLRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_INT32         i32TaskCount;			/* 返回的任务总数 */
	}GK_OPCUA_CALLRESPONSE;

	/*============================================================================
	*	清空任务列表--请求包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_CLEARTASKLISTREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;				/* 主控前端节点号 */
	}GK_OPCUA_CLEARTASKLISTREQUEST;

	/*============================================================================
	* 清空任务列表--应答包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_CLEARTASKLISTRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32	NodeId;				/* 主控前端节点号 */
	}GK_OPCUA_CLEARTASKLISTRESPONSE;

	/*============================================================================
	*	设置主控前端触发时间表,并按时间表产生同步信号--请求包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMELISTREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32			NodeId;				/* 主控前端节点号 */
		OPC_INT32			i32TimeCount;		/* 列表元刈苁 */
		OPCUA_DATETIME  pTime[MAX_GK_OPCUA_TIME_LIST];	/* 触发时间列表 */
	}GK_OPCUA_SETTIMELISTREQUEST;

	/*============================================================================
	* 设置主控前端触发时间表--应答包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTASKTIMELISTRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
	}GK_OPCUA_SETTASKTIMELISTRESPONSE;

	/*============================================================================
	* 校时--请求包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMEREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32			NodeId;				/* 主控前端节点号 */
		OPCUA_DATETIME	CurrentTime;		/* 当前时间 */
	}GK_OPCUA_SETTIMEREQUEST;

	/*============================================================================
	* 校时--应答包结?	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMERESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32					NodeId;		/* 主控前端节点?*/
		OPCUA_DATETIME         time;		/* 当前时间 */
	}GK_OPCUA_SETTIMERESPONSE;


	/*============================================================================
	*	查询--请求包结构
	*===========================================================================*/
	typedef struct _GK_OPCUA_QUERYREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;				/* 查询的前端节点号 */
	}GK_OPCUA_QUERYREQUEST;

	/*============================================================================
	* 查询--应答包结?
	*===========================================================================*/
	typedef struct _GK_OPCUA_QUERYRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32	NodeId;				/* 查询的前端节点号 */
	}GK_OPCUA_QUERYRESPONSE;
	
	/*============================================================================
	*命令--请求结构
	*============================================================================*/
	typedef struct _GK_OPCUA_COMMANDREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32 NodeId; //查询节点
		OPC_UINT32 CommandCode; 
	}GK_OPCUA_COMMANDREQUEST;
	
	/*==========================================================================
	* 	命令--返回结构
	*===========================================================================*/
	typedef GK_OPCUA_COMMANDREQUEST GK_OPCUA_COMMANDRESPONSE;

	/*============================================================================
	* 新建订阅--请求包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATESUBSCRIPTIONREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32PublishingInterval;	/* 订阅间隔 */
		OPC_UINT32		i32LifetimeCount;		/* 订阅次数 */
	}GK_OPCUA_CREATESUBSCRIPTIONREQUEST;

	/*============================================================================
	* 新建订阅--应答包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATESUBSCRIPTIONRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32		NodeId;							/* 前端节点号 */
		OPC_UINT32		i32SubscriptionId;				/* 返回的订单号 */
		OPC_UINT32		i32RevisedPublishingInterval;	/* 订阅间隔 */
		OPC_UINT32      i32RevisedLifetimeCount;		/* 订阅次数 */
	}GK_OPCUA_CREATESUBSCRIPTIONRESPONSE;

	/*============================================================================
	*  删除订阅--请求包结?	*===========================================================================*/
	typedef struct GK_OPCUA_DELETESUBSCRIPTIONSREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;					/* 前端节点号 */
		OPC_UINT32	i32NoOfSubscriptionIds;	/* 订单号 */
	}GK_OPCUA_DELETESUBSCRIPTIONSREQUEST;

	/*============================================================================
	* 删除订阅--应答包结构
	*===========================================================================*/
	typedef struct GK_OpcUa_DeleteSubscriptionsResponse
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32NoOfSubscriptionIds;	/* 订单号 */
	}GK_OpcUa_DeleteSubscriptionsResponse;


	/*============================================================================
	* 新建监视--请求包结构 (值变化、状态变化、事件)
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATEMONITOREDITEMSREQUEST
	{
		GK_OPCUA_REQUESTHEADER	RequestHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_FLOAT32		fLowerLimit;			/* 死区下限 */
		OPC_FLOAT32		fUpperLimit;			/* 死区上限 */
	}GK_OPCUA_CREATEMONITOREDITEMSREQUEST;

	/*============================================================================
	* 新建监视--应答包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATEMONITOREDITEMSRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER             ResponseHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_FLOAT32		fLowerLimit;			/* 死区下限 */
		OPC_FLOAT32		fUpperLimit;			/* 死区上限 */
	}GK_OPCUA_CREATEMONITOREDITEMSRESPONSE;

	/*============================================================================
	* 启动监视--请求包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_SETTRIGGERINGREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32TriggeringItemId;	/* 返回监视编号 */
	}GK_OPCUA_SETTRIGGERINGREQUEST;

	/*============================================================================
	* 启动监视--应鸢峁
	*===========================================================================*/
	typedef struct GK_OPCUA_SETTRIGGERINGRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32TriggeringItemId;	/* 监视编号 */
	}GK_OPCUA_SETTRIGGERINGRESPONSE;

	/*============================================================================
	* 删除监视--请求包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_DELETEMONITOREDITEMSREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32TriggeringItemId;	/* 监视编号 */
	}GK_OPCUA_DELETEMONITOREDITEMSREQUEST;

	/*============================================================================
	* 删除监视--应答包结构
	*===========================================================================*/
	typedef struct GK_OPCUA_DELETEMONITOREDITEMSRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* 前端节点号 */
		OPC_UINT32		i32TriggeringItemId;	/* 监视编号 */
	}GK_OPCUA_DELETEMONITOREDITEMSRESPONSE;


#endif
