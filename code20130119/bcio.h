/*********************************************************************************************
文件：bcio.h
作者：
创建：
版本：V0.1
功能：对61580/65171的REG RAM的操作
简介：对BC操作 WriteReg---WriteRam----WriteBlock-----ReadReg----ReadRam----ReadBlock----
	  对RT操作 RTWriteReg---RTWriteRam---RTWriteBlock---RTReadReg---RTReadRam--RTReadBlock----
**********************************************************************************************/
#ifndef _BC_IO_H_
#define _BC_IO_H_

#include "types.h"






//========================write relation==============================

#define BC_BASE_REG_ADDR_W	0xf000		// write reg data
#define BC_BASE_RAM_ADDR_W      0x2000		// write ram data

#define WR_REG_ADDR_WR		0xE000	        // write reg addr

#define WR_RAM_LENGTH_WR 	0x4000		// write data length
#define WR_RAM_START_ADDR_WR 	0x6000		// write data start addr

#define WR_RAM_REG_STATUS_WR   	0xA000		// write ram 0x0000 reg 0x0001

#define WR_WRITE_STATUS       	0x8000		// write status reg
#define RD_WRITE_STATUS       	0xC000		// read startus reg

//==========================read relation=============================

#define BC_BASE_REG_ADDR_R	0x5000			// read reg data
#define BC_BASE_RAM_ADDR_R	0x7000			// read ram data

#define RD_RD_STATUS		0xB000			// read read status 0x0001 start read

#define WR_REG_ADDR_RD		0xD000			// read reg addr
#define WR_RAM_REG_STATUS_RD	 	0x9000				// read ram 0x0000 reg 0x0001 

#define WR_RAM_LENGTH_RD	 	0x3000				// read ram data length
#define WR_RAM_START_ADDR_RD		0X1A00				// read ram start addr

#define START_READ_1553B 	 	0x1F00				// start enable 1553B 0x0001
#define READ_DATA_OVER_ARM	 	0x1800				// read data over 0x0000

#endif
