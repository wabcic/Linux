/*
*
*  0069项目
*  数据类型定义
*
*  Created by deyi.gong on 2009-10-19.
*  Copyright 2009 gkhy.com.cn. All rights reserved.
*
*/

#ifndef GK_OPC_UA_TYPES_H
#define GK_OPC_UA_TYPES_H
	
	//////////////////////////////////////////////////////////////////////////
	typedef void                OPC_VOID;
	typedef unsigned char		OPC_BOOL;
	typedef	char				OPC_CHAR;
	
	typedef	char				OPC_INT8;
	typedef  short				OPC_INT16;
	typedef	long				OPC_INT32;
	
	typedef	unsigned char		OPC_UINT8;
	typedef unsigned short  	OPC_UINT16;
	typedef unsigned long		OPC_UINT32;
	
	typedef float				OPC_FLOAT32;
	
	typedef	unsigned char	BYTE;
//	typedef double				OPC_FLOAT64;
	
#define OPC_FALSE         0
#define OPC_TRUE          (!OPC_FALSE)
	
	//////////////////////////////////////////////////////////////////////////
	
#define INT8_Max			(INT8)127
#define UINT8_Min			(UINT8)0
#define UINT8_Max			(UINT8)255
#define INT16_Min			(INT16)-32768
#define INT16_Max			(INT16)32767
#define UINT16_Min			(UINT16)0
#define UINT16_Max			(UINT16)65535
#define INT32_Min			INT_MIN
#define INT32_Max			INT_MAX
#define UINT32_Min			0
#define UINT32_Max			UINT_MAX
#define FLOAT32_Min			(FLOAT32)1.175494351e-38F
#define FLOAT32_Max			(FLOAT32)3.402823466e+38F

#endif
