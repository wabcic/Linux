/*
*
*  0069��Ŀ
*  �򻯺��OPC UAЭ�����ݶ����ļ�
*
*  Created by deyi.gong on 2009-10-19.
*  Copyright 2009 gkhy.com.cn. All rights reserved.
*
*/

#ifndef GK_OPC_UA_HEADER_H
#define GK_OPC_UA_HEADER_H
#define  GK_OPC_UA_PLATFORM_X86

#include "gk_opcua_types.h"

#define		MAX_GK_OPCUA_REQUEST_LEN	32*1024	/* �������󳤶� */	
#define		MAX_GK_OPCUA_RESPONSE_LEN	32*1024	/* Ӧ�����󳤶� */	
#define		MIN_GK_OPCUA_REQUEST_LEN	32		/* �������С���� */	
#define		MIN_GK_OPCUA_RESPONSE_LEN	32		/* Ӧ�����С���� */	
#define		MAX_GK_OPCUA_BUFFER_LEN    2*1024               /*ͨ��ջ���峤��*/

#define		MAX_GK_OPCUA_RESERVE		4		/* ������ֽ��� */	
#define		MAX_GK_OPCUA_TASK			32*4	/* ��������� */
#define		MAX_GK_OPCUA_TASK_PARAM		32*2	/* �����в�����󳤶� */
#define		MAX_GK_OPCUA_TIME_LIST		32*4	/* ���ʱ���б��� */

#define		MAX_GK_OPCUA_IPADDRESS_LEN	16		/* ���IP��ַ�ִ����� */


#define		MAX_TIMELIST_LEN		128

	//���ܶ���
	typedef enum _GK_OPCUA_FUNC_CODE
	{
			/* �������   0x1000 --- 0x11FF */
			GK_OPCUA_CALL					= 0x1000,		/* Ԥ������ */
			GK_OPCUA_CLEARTASKLIST			= 0x1001,		/* ��������б� */
			GK_OPCUA_SETTIMELIST			= 0x1002,		/* ��������ǰ�˴���ʱ��� */
			GK_OPCUA_SETTIME				= 0x1003,		/* Уʱ */
// 			//��������
// 			GK_OPCUA_STARTTEST       = 0xA001,//��ʼ����
// 			GK_OPCUA_STOPTEST        = 0xA002,//��������
// 			GK_OPCUA_SYNCHRONIZATIONTRIGGER = 0xA003,//ͬ������
// 			GK_OPCUA_TIMETRIGGER           = 0xA004,//��ʱ����
// 			GK_OPCUA_PARAMNODE           = 0xA005,//�����洢��

			/* ʱ��� */
			GK_OPCUA_SCHEDULE = 0x1111,

			/* ���������ԡ�ֱ������ */
			GK_OPCUA_START_TASK = 0x2121,

			GK_OPCUA_STOP_TASK = 0x2323,

			/* ��ʱ���� */
			GK_OPCUA_TIMER_TASK	= 0x3131,

			/* ͬ������ */
			GK_OPCUA_SYN_TASK	= 0x3232,
			
			/* perform task directly */
			GK_OPCUA_DIRECT_TASK = 0x1500,
			
			/*����*/
			GK_OPCUA_COMMAND				= 0x1100,		/*����*/
			
			/* ��ѯ		  0x1200 --- 0x12FF */
			GK_OPCUA_QUERY					= 0x1200,		/* ��ѯ */
			/* ������� */
			GK_OPCUA_TASK_PARA_1 = 0x4141,				/* PARA NODE 1 */
			GK_OPCUA_TASK_PARA_2 = 0x4242,				/* PARA NODE 2 */
			GK_OPCUA_TASK_PARA_3 = 0x4343,				/* PARA NODE 3 */
			GK_OPCUA_TASK_PARA_4 = 0x4444,				/* PARA NODE 4 */
			GK_OPCUA_TASK_PARA_5 = 0x4545,				/* PARA NODE 5 */

			//���Ͳ��������ڲ�����
			GK_OPCUA_TASK_SEND_PARAMETER = 0x5001,
			//���ʹ��������ڲ�����
			GK_OPCUA_TASK_TRIGGER = 0x5002,
			
			/* ����		  0x1300 --- 0x13FF */
			GK_OPCUA_CREATESUBSCRIPTION		= 0x1300,		/* �½����� */
			GK_OPCUA_DELETESUBSCRIPTIONS	= 0x1301,		/* ɾ������ */
			GK_OPCUA_SUBSCRIPTIONCONTENT = 0x1302,		/*���ķ�������*/
			
			/* ����		  0x1400 --- 0x14FF */
			GK_OPCUA_CREATEMONITOREDITEMS	= 0x1400,		/* �½����� */
			GK_OPCUA_SETTRIGGERING			= 0x1401,		/* �������� */
			GK_OPCUA_DELETEMONITOREDITEMS	= 0x1402,		/* ɾ������ */
			GK_OPCUA_MONITORCONTENT = 0x1403 /*���ӷ�������*/
	}GK_OPCUA_FUNC_CODE;

	typedef enum _GK_OPCUA_ERROR
	{
		GK_OPCUA_NOERROR =0x0,                 /*û�д���*/
		GK_OPCUA_NOTFOUNDNODEID =0x1000,/*û�з��ֿͻ�������Ľڵ�*/
		GK_OPCUA_OTHERERROR = 0x2000         /*��������*/
	}GK_OPCUA_ERROR;
	/****************************************************************************/	
	/*							ͨ��ջ�����ݶ���								*/
	/****************************************************************************/	

	/* ʱ�䶨�� */
	typedef struct _OPCUA_DATETIME 
	{
		OPC_UINT32 dwLowDateTime;
		OPC_UINT32 dwHighDateTime;
	}OPCUA_DATETIME;	
	/************************************************************************/
	/* ��ͷ���壺��ԭ��������ͷ����Ӧͷ�ϲ���һ��      */
	/************************************************************************/
	typedef struct _OPCUA_PACK_HEADER
	{
		OPC_UINT32		FunCode; /* �������� */
		OPC_UINT32          error; /*�������*/
		OPC_UINT32				excode; /*������(�����ڱ�ʶ���͵�����)*/
		OPC_INT32 		   	    i32Length;		/* �������ĳ���(������ͷ������) */
		OPC_UINT32				NodeSource;		/* ���ͽڵ�ID */
		OPCUA_DATETIME      Timestamp;		/* ͨ��ʼ��ʱ��� */
	} OPCUA_PACK_HEADER;
	/*============================================================================
	* �������������
	*===========================================================================*/
	typedef OPCUA_PACK_HEADER GK_OPCUA_REQUESTHEADER;
	typedef OPCUA_PACK_HEADER GK_OPCUA_RESPONSEHEADER;

	/*============================================================================
	* �ⲿ�����ͷ����
	*===========================================================================*/
	typedef struct _GK_OPCUA_OUTTERTASK_HEADER
	{
		OPC_UINT16  ExecuteType;//�ⲿ��������
		OPC_UINT16  SynID;          //ͬ��ID
		OPC_UINT32 TriggerAddr; //������ַ
		OPC_UINT32 TriggerKey;      //����Կ��
		OPC_UINT32 AddrParam;   //������ַ
		OPC_UINT32 ParamLen;     //��������
		OPC_UINT32 Reserved;     //����
	}GK_OPCUA_OUTTERTASK_HEADER;
	
	#define GK_OPCUA_REQUESTHEADER_LEN sizeof(GK_OPCUA_REQUESTHEADER)	

	/*============================================================================
	* ����Ӧ�������
	*===========================================================================*/
	#define GK_OPCUA_RESPONSEHEADER_LEN sizeof(GK_OPCUA_RESPONSEHEADER)

	/*============================================================================
	* ����֪ͨ��Ϣ����
	*===========================================================================*/
	typedef struct _GK_OPCUA_NOTIFICATIONMSG
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32				i32SequenceNumber;	/* ���к� */
		OPCUA_DATETIME			PublishTime;		/*������ʱ�� */
		OPC_INT32				i32MsgLength;		/* ��Ϣ���� */
	//	OPC_CHAR				*pMsgData;			/* ��Ϣ���� */	
	}GK_OPCUA_NOTIFICATIONMSG;

	/****************************************************************************/	
	/*							Ӧ�ò����ݶ���									*/
	/****************************************************************************/	
	/*��������*/
	typedef enum _GK_OPCUA_TRIGGERINGCONDITION
	{
		OpcUa_Triggering_Synchronization = 0,	/* ͬ������ */
		OpcUa_Triggering_immediately	 = 1,	/* �������� */
		OpcUa_Triggering_monitor		 = 2,	/* monitor���� */
		OpcUa_Triggering_Timer           =3   //��ʱ����
	}GK_OPCUA_TRIGGERINGCONDITION;


	/*============================================================================
	*  �������ݽṹ
	*===========================================================================*/
#pragma pack(1)
	typedef struct _GK_OPCUA_TASK{
		unsigned char   byTaskNO;//������
		unsigned short u16TaskID;   //����ID
		unsigned char	i8SynFlag;
		unsigned short u16BufferRegion;//�����洢��������
		unsigned long ulParaStart;//�����������ʼֽ
		unsigned long ulParaLen; //�������������
	} GK_OPCUA_TASK;
#pragma pack()
	/*============================================================================
	* ʱ���б����ݽṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_TIMELIST{
		OPCUA_DATETIME executeTime;
		OPC_UINT32 ulTaskNO;//ʱ��������
	} GK_OPCUA_TIMELIST;


	/*============================================================================
	* ��ʼ��������
	*===========================================================================*/
	typedef struct _GK_OPCUA_STARTTASKREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT16                           ui16StartTask;
	} GK_OPCUA_STARTTASKREQUEST;
	/*============================================================================
	* ������������
	*===========================================================================*/
	typedef struct _GK_OPCUA_STOPTASKREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT8                           ui8StopTask;
	} GK_OPCUA_STOPTASKREQUEST;
	/*============================================================================
	* Ԥ����������
	*===========================================================================*/
	typedef struct _GK_OPCUA_CALLREQUEST
	{
		GK_OPCUA_REQUESTHEADER  RequestHeader;
		//OPC_UINT32            uiNodeID;                    //
		OPC_INT32				i32TaskCount;				/* ��Ч�������� */
		GK_OPCUA_TASK		pTask[MAX_GK_OPCUA_TASK];	/* �����б� */
	}GK_OPCUA_TASKREQUEST;//GK_OPCUA_CALLREQUEST
	/*============================================================================
	* ʱ���б�����
	*===========================================================================*/
	typedef struct _GK_OPCUA_TIMELISTREQUEST{
		GK_OPCUA_REQUESTHEADER  RequestHeader;
		OPC_UINT32        ui32TimeListCount;//ʱ�������
		GK_OPCUA_TIMELIST pTimeList[MAX_TIMELIST_LEN];//ʱ���б�
	} GK_OPCUA_TIMELISTREQUEST;

	/*============================================================================
	* Ԥ������Ӧ��
	*===========================================================================*/
	typedef struct _GK_OPCUA_CALLRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_INT32         i32TaskCount;			/* ���ص��������� */
	}GK_OPCUA_CALLRESPONSE;

	/*============================================================================
	*	��������б�--������ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_CLEARTASKLISTREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;				/* ����ǰ�˽ڵ�� */
	}GK_OPCUA_CLEARTASKLISTREQUEST;

	/*============================================================================
	* ��������б�--Ӧ����ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_CLEARTASKLISTRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32	NodeId;				/* ����ǰ�˽ڵ�� */
	}GK_OPCUA_CLEARTASKLISTRESPONSE;

	/*============================================================================
	*	��������ǰ�˴���ʱ���,����ʱ������ͬ���ź�--������ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMELISTREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32			NodeId;				/* ����ǰ�˽ڵ�� */
		OPC_INT32			i32TimeCount;		/* �б�Ԫ���� */
		OPCUA_DATETIME  pTime[MAX_GK_OPCUA_TIME_LIST];	/* ����ʱ���б� */
	}GK_OPCUA_SETTIMELISTREQUEST;

	/*============================================================================
	* ��������ǰ�˴���ʱ���--Ӧ����ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTASKTIMELISTRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
	}GK_OPCUA_SETTASKTIMELISTRESPONSE;

	/*============================================================================
	* Уʱ--������ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMEREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32			NodeId;				/* ����ǰ�˽ڵ�� */
		OPCUA_DATETIME	CurrentTime;		/* ��ǰʱ�� */
	}GK_OPCUA_SETTIMEREQUEST;

	/*============================================================================
	* Уʱ--Ӧ�����?	*===========================================================================*/
	typedef struct _GK_OPCUA_SETTIMERESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32					NodeId;		/* ����ǰ�˽ڵ�?*/
		OPCUA_DATETIME         time;		/* ��ǰʱ�� */
	}GK_OPCUA_SETTIMERESPONSE;


	/*============================================================================
	*	��ѯ--������ṹ
	*===========================================================================*/
	typedef struct _GK_OPCUA_QUERYREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;				/* ��ѯ��ǰ�˽ڵ�� */
	}GK_OPCUA_QUERYREQUEST;

	/*============================================================================
	* ��ѯ--Ӧ�����?
	*===========================================================================*/
	typedef struct _GK_OPCUA_QUERYRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32	NodeId;				/* ��ѯ��ǰ�˽ڵ�� */
	}GK_OPCUA_QUERYRESPONSE;
	
	/*============================================================================
	*����--����ṹ
	*============================================================================*/
	typedef struct _GK_OPCUA_COMMANDREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32 NodeId; //��ѯ�ڵ�
		OPC_UINT32 CommandCode; 
	}GK_OPCUA_COMMANDREQUEST;
	
	/*==========================================================================
	* 	����--���ؽṹ
	*===========================================================================*/
	typedef GK_OPCUA_COMMANDREQUEST GK_OPCUA_COMMANDRESPONSE;

	/*============================================================================
	* �½�����--������ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATESUBSCRIPTIONREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32PublishingInterval;	/* ���ļ�� */
		OPC_UINT32		i32LifetimeCount;		/* ���Ĵ��� */
	}GK_OPCUA_CREATESUBSCRIPTIONREQUEST;

	/*============================================================================
	* �½�����--Ӧ����ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATESUBSCRIPTIONRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER ResponseHeader;
		OPC_UINT32		NodeId;							/* ǰ�˽ڵ�� */
		OPC_UINT32		i32SubscriptionId;				/* ���صĶ����� */
		OPC_UINT32		i32RevisedPublishingInterval;	/* ���ļ�� */
		OPC_UINT32      i32RevisedLifetimeCount;		/* ���Ĵ��� */
	}GK_OPCUA_CREATESUBSCRIPTIONRESPONSE;

	/*============================================================================
	*  ɾ������--�������?	*===========================================================================*/
	typedef struct GK_OPCUA_DELETESUBSCRIPTIONSREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32	NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32	i32NoOfSubscriptionIds;	/* ������ */
	}GK_OPCUA_DELETESUBSCRIPTIONSREQUEST;

	/*============================================================================
	* ɾ������--Ӧ����ṹ
	*===========================================================================*/
	typedef struct GK_OpcUa_DeleteSubscriptionsResponse
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32NoOfSubscriptionIds;	/* ������ */
	}GK_OpcUa_DeleteSubscriptionsResponse;


	/*============================================================================
	* �½�����--������ṹ (ֵ�仯��״̬�仯���¼�)
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATEMONITOREDITEMSREQUEST
	{
		GK_OPCUA_REQUESTHEADER	RequestHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_FLOAT32		fLowerLimit;			/* �������� */
		OPC_FLOAT32		fUpperLimit;			/* �������� */
	}GK_OPCUA_CREATEMONITOREDITEMSREQUEST;

	/*============================================================================
	* �½�����--Ӧ����ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_CREATEMONITOREDITEMSRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER             ResponseHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_FLOAT32		fLowerLimit;			/* �������� */
		OPC_FLOAT32		fUpperLimit;			/* �������� */
	}GK_OPCUA_CREATEMONITOREDITEMSRESPONSE;

	/*============================================================================
	* ��������--������ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_SETTRIGGERINGREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32TriggeringItemId;	/* ���ؼ��ӱ�� */
	}GK_OPCUA_SETTRIGGERINGREQUEST;

	/*============================================================================
	* ��������--Ӧ����
	*===========================================================================*/
	typedef struct GK_OPCUA_SETTRIGGERINGRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32TriggeringItemId;	/* ���ӱ�� */
	}GK_OPCUA_SETTRIGGERINGRESPONSE;

	/*============================================================================
	* ɾ������--������ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_DELETEMONITOREDITEMSREQUEST
	{
		GK_OPCUA_REQUESTHEADER RequestHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32TriggeringItemId;	/* ���ӱ�� */
	}GK_OPCUA_DELETEMONITOREDITEMSREQUEST;

	/*============================================================================
	* ɾ������--Ӧ����ṹ
	*===========================================================================*/
	typedef struct GK_OPCUA_DELETEMONITOREDITEMSRESPONSE
	{
		GK_OPCUA_RESPONSEHEADER  ResponseHeader;
		OPC_UINT32		NodeId;					/* ǰ�˽ڵ�� */
		OPC_UINT32		i32TriggeringItemId;	/* ���ӱ�� */
	}GK_OPCUA_DELETEMONITOREDITEMSRESPONSE;


#endif
