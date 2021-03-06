//-----------------------------------------------------------------
//--Edited by tzh at 20091223
//--TaskInfo.h
//-----------------------------------------------------------------

/***************************** Include Files *********************************/
#include "gk_opcua_header.h"
#include "TaskFun.h"

#define		OPCUA_HEADER_BYTES			28						/*OPCUA包头字节数*/
#define		OPCUA_TASK_BYTES			14						/*定时和同步任务字节数*/
#define		OPCUA_OUTTER_TASK_HEADER_BYTES		24

#define		MAX_SCHEDULE_ITEM_COUNT			200
#define		MAX_SCHEDULE_LEN			12*(MAX_SCHEDULE_ITEM_COUNT)			/*最大时表扯 */

#define		MAX_TIMER_TASK_COUNT			200						/*最大定时任务数*/
#define		MAX_TIMER_TASK_LEN			(OPCUA_TASK_BYTES)*(MAX_TIMER_TASK_COUNT)	/*最大定时任务长度 */

#define		MAX_SYN_TASK_COUNT			200						/*最大同步任务数*/
#define		MAX_SYN_TASK_LEN			(OPCUA_TASK_BYTES)*(MAX_TIMER_TASK_COUNT)	/*最大同步任务长度 */
#define		MAX_TASK_PARA_LEN			32768

#define		MAX_BUF_LEN				MAX_TIMER_TASK_LEN

#define		PARANODE_COUNT				5

#define		FRAME_LEN				960

#define		BRAM_READ_BUF_LEN		100

#define		DI_BUF_SIZE_BYTE		8196
#define		DI_BUF_SIZE				2048		// actual buf size = 2048 * 4 = 8196
#define		EACH_DI_DATA_SIZE		1024		// size of bytes = 1024 * 4 = 4096

#define		TRIGGER_ADDR_1			0x00000001
#define		SYN_TRIGGER_ADDR		0x00000002













#define		START_DI			0x020C
#define		STOP_DI				0x020E

#define		START_BICC_DB			0x0f04
#define         START_BICC_K_422                0x0f05
#define         START_BICC_D_422                0x0f06
#define         START_BICC_SYN                  0x0f07
#define         START_BICC_YC                 	0x0f08


#define		BROADCASE_TIME_1553		0x5500
#define		BROADCASE_STATUS_1553		0x5522
#define		BROADCASE_PALTPARA_1553		0x5544
#define		SEND_CONTROLCMD_1553		0x5566
#define		SEND_DATAINJECT_1553		0x5588

#define		SEND_SELFTEST_1553		0x55EE
#define		SEND_STRESSTEST_1553		0x55FF		









#define     START_SYN_SIGNAL     0x0304

//add by tzh at 20100116-1500 test TTL

//add by tzh at 20100128 test opcua_send
#define     OPCUA_SENDBACK      0x0500


#define		START_SYN_SIGNAL     0x0304


typedef union {
	unsigned long lValue;
	unsigned char ucArray[4];
} BRAM_DATA;

typedef struct _Subscribe
{
	int sd;	//socket通道
	unsigned long NodeId;	//订阅节鉏D
	unsigned long Interval; //订阅的间隔
	unsigned long Count;    //订阅次数
}Subscribe;


   //查询线程结构体
typedef struct _Monitor
{
   int sd;  //socket 通道
	unsigned long NodeId;   //查询节点ID
	//unsigned long i32SequenceNumber;
	unsigned long fLowerLimit; //查询死区下限
	unsigned long fUpperLimit; //查询死区上限
}Monitor;

typedef struct _Command
{
	int sd;
	unsigned long CommandCode;
	unsigned long NodeId;
	unsigned long Interval;
	unsigned long Count;
	unsigned long TimeStamp;
	unsigned long DataLength;
}Command;


//int NodeIDs[] = { 1, 2, 3, 4, 5 };
typedef struct _NodeInfo_
{
	unsigned long NodeId;
	unsigned long Node_BRAM_BASE_ADDR;
	unsigned long Node_BRAM_ADDR_OFFSET;
	struct _NodeInfo_* pNext;
}NodeInfo;


  /*============================================================================
	*	命令--请求包结构
	*===========================================================================*/
	
	typedef enum _GK_OPCUA_COMMANDCODE
	{
		  COMMAND_SENDDATA = 0x00001000,
		  COMMAND_JIAOSHI = 0x00001001,
		  COMMAND_EXECUTETASK = 0x00001002
	}GK_OPCUA_COMMANDCODE;
	
	typedef struct _GK_OPCUA_COMMANDREQUEST_HEADER
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32 NodeId;
//		GK_OPCUA_UINT32 NodeId;				/* 命令的前端节点号 */
		GK_OPCUA_COMMANDCODE CommandCode;
	}GK_OPCUA_COMMANDREQUEST_HEADER;
	
	typedef struct _GK_OPCUA_COMMANDTEST
	{
		GK_OPCUA_COMMANDREQUEST_HEADER CommandHeader;
//		GK_OPCUA_COMMANDCODE CommandCode;
//		GK_OPC_UINT32 NodeId;
		OPC_UINT32 Interval;
		OPC_UINT32 Count;
		OPCUA_DATETIME TimeStamp;
		OPC_UINT32 DataLength;
	}GK_OPCUA_COMMANDTEST;	
