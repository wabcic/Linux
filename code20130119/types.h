/***************************************************************************************************
文件：			types.h
作者：			
创建：			
版本：			Version 0.1
功能：			常量数据类型定义
相关文件：	
**************************************************************************************************/

	#ifndef _TYPES_H
	#define _TYPES_H
	
/****************************************************************************************************
*****************************************************************************************************	
**************************************数据类型*******************************************************
*****************************************************************************************************
*****************************************************************************************************/
	typedef void               	 	VOID;
	typedef unsigned char			BOOL;
	
	typedef	char				INT8;
	typedef short				INT16;
	typedef	int				INT32;
	typedef long 				INT64;
	
	typedef	unsigned char			UINT8;
	typedef unsigned short  		UINT16;
	typedef unsigned int 			UINT32;


	typedef float				FLOAT32;
	

	
	#define OPC_FALSE         		0
	#define OPC_TRUE          		(!OPC_FALSE)

//=================================================================================================
//*****************************************数据极限***********************************************
//=================================================================================================	

	#define INT8_MIN				(INT8)-128
	#define INT8_Max				(INT8)127
	#define UINT8_Min				(UINT8)0
	#define UINT8_Max				(UINT8)255
	#define INT16_Min				(INT16)-32768
	#define INT16_Max				(INT16)32767
	#define UINT16_Min				(UINT16)0
	#define UINT16_Max				(UINT16)65535
	#define INT32_Min				INT_MIN
	#define INT32_Max				INT_MAX
	#define UINT32_Min				0
	#define UINT32_Max				UINT_MAX

	#define FLOAT32_Min				(FLOAT32)1.175494351e-38F
	#define FLOAT32_Max				(FLOAT32)3.402823466e+38F

	#ifndef  FALSE
	#define  FALSE   0
	#endif

	#ifndef  TRUE
	#define  TRUE   1
	#endif

	#ifndef  NULL
	#define  NULL   0
	#endif

	#endif
