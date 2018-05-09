#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "test.h"
#include "testlib.h"

#include "types.h"
#include"bcio.h"

#ifdef FPAG_APP_DEBUG
#define FDEBUG(fmt, args...)	printf("APP: " fmt "\n", ##args)
#else
#define FDEBUG(fmt, args...)
#endif
#define FERR(fmt, args...)		printf("APP[ERR]: " fmt "\n", ##args)

char *dev_file = "/dev/fpga_device";
extern unhandledTTs_1;
extern int fd;

#define watie_BC  for(i=0;i<10;i++)

//======================================BC==========================================================

void WriteReg(UINT16 reg , UINT16 value)
{
	
	int wr_status;
	int i=0;
	wr_status = fdev_rword(fd,RD_WRITE_STATUS);					// read write status 
	while(wr_status !=0x0010 && i<1400)
	{	
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;	
	}
	i=0;
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);						// write wire-status reg
//	fdev_wword(fd,(0x2000+reg),value);	
	watie_BC;	
	fdev_wword(fd,WR_REG_ADDR_WR,(UINT8)reg);					// write reg-addr
	watie_BC;	
	fdev_wword(fd,BC_BASE_REG_ADDR_W,value);					// write reg-data
	watie_BC;	
	fdev_wword(fd,WR_RAM_REG_STATUS_WR,0x0001);					// write reg 0x0001 ram 0x0000
	watie_BC;
	fdev_wword(fd,WR_WRITE_STATUS,0x0001);						// write reg over
											// close /dev/fpga_device
}

//====================write ram ========================================

void WriteRam(UINT16 offset, UINT16 value)
{
	int wr_status;
	int i=0;
	wr_status = fdev_rword(fd,RD_WRITE_STATUS);					// read write status
	
	while(wr_status != 0x0010 && i<1400 )
//	while(wr_status != 0x0010)	
	{	
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;
	}
	i=0;
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);						// write write-status reg
	watie_BC;
	fdev_wword(fd,WR_RAM_START_ADDR_WR,offset);					// write ram start addr
	watie_BC;	
	fdev_wword(fd,BC_BASE_RAM_ADDR_W,value);					// write ram data
	watie_BC;
	fdev_wword(fd,WR_RAM_LENGTH_WR,0x0001);						// write ram data length
	watie_BC;
	fdev_wword(fd,WR_RAM_REG_STATUS_WR,0x0000);					// write reg 0x0001 ram 0x0000
	watie_BC;	
	fdev_wword(fd,WR_WRITE_STATUS,0x0001);						// write reg over
}


//===============================================================

void WriteBlock(UINT16 offset,UINT16 *address,UINT16 count)
{
	
	int wr_status;
	int i=0;
	wr_status = fdev_rword(fd,RD_WRITE_STATUS);						// read write status
	
	while(wr_status != 0x0010 && i<1400)
//	while(wr_status != 0x0010)
	{
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;
	}
	i=0;
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);							// write write-status reg
	watie_BC;
	fdev_wword(fd,WR_RAM_START_ADDR_WR,offset);						// write ram start addr	
	watie_BC;
	for(i=0;i<count;i++)

		fdev_wword(fd,BC_BASE_RAM_ADDR_W+i,address[i]);					// write ram data

	fdev_wword(fd,WR_RAM_LENGTH_WR,count);							// write ram data length
	watie_BC;
	fdev_wword(fd,WR_RAM_REG_STATUS_WR,0x0000);						// write reg 0x0001 ram 0x0000
	watie_BC;	
	fdev_wword(fd,WR_WRITE_STATUS,0x0001);							// write reg over

}

//================BC read operating===============================

UINT16 ReadReg(UINT16 reg)
{
	
	int wr_status,rd_status;
	UINT16 regdata;
	int i=0;
	wr_status = fdev_rword(fd,RD_WRITE_STATUS);						// read write status
	while(wr_status != 0x0010 && i<1400)
	{
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;
	}
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);							// write write-status reg
	watie_BC;
	fdev_wword(fd,WR_REG_ADDR_RD,(UINT8)reg);						// write reg-addr
	watie_BC;
	fdev_wword(fd,WR_RAM_REG_STATUS_RD,0x0001);						// write reg 0x0001 ram 0x0000
	watie_BC;
	fdev_wword(fd,START_READ_1553B,0x0001);							// start read 1553B
	watie_BC;
	rd_status = fdev_rword(fd,RD_RD_STATUS);						// read status start
	
	i=0;
	while(rd_status != 0x0001 && i<1200)
	{
		rd_status = fdev_rword(fd,RD_RD_STATUS);
		i++;
	}
	i=0;
	regdata = fdev_rword(fd,BC_BASE_REG_ADDR_R);						// read data to regdata
	watie_BC;
	fdev_wword(fd,READ_DATA_OVER_ARM,0x0000);
	watie_BC;
	fdev_wword(fd,WR_WRITE_STATUS,0x0010);

	return regdata;	
}

UINT16 ReadRam(UINT16 offset)
{
	
	int wr_status,rd_status;
	UINT16 Ramdata;
	int i=0;
	wr_status = fdev_rword(fd,RD_WRITE_STATUS);						// read status
	while(wr_status != 0x0010 && i<1400)
	{
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;
	}
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);							// write write-status reg
	watie_BC;
	fdev_wword(fd,WR_RAM_LENGTH_RD,0x0001);							// read ram data length
	watie_BC;
	fdev_wword(fd,WR_RAM_START_ADDR_RD,offset);						// read ram address
	watie_BC;
	fdev_wword(fd,WR_RAM_REG_STATUS_RD,0x0000);						// read ram 0x0000 reg 0x0001
	watie_BC;
	fdev_wword(fd,START_READ_1553B,0x0000);							// read start 1553B
	watie_BC;
	rd_status = fdev_rword(fd,RD_RD_STATUS);						// read status start
	
	i=0;
	while(rd_status != 0x0001 && i<1200)
	{
		rd_status = fdev_rword(fd,RD_RD_STATUS);
		i++;
	}
	i=0;
	Ramdata = fdev_rword(fd,BC_BASE_RAM_ADDR_R);					// read data to regdata
	watie_BC;
	fdev_wword(fd,READ_DATA_OVER_ARM,0x0000);					// read data over		
	watie_BC;
	fdev_wword(fd,WR_WRITE_STATUS,0x0010);						// write status to 

	return Ramdata;

}

void ReadBlock(UINT16 Offset, UINT16* Address, UINT16 Count)
{
	int i=0;
	int wr_status,rd_status;


	wr_status = fdev_rword(fd,RD_WRITE_STATUS);			// read status 	
	while(wr_status != 0x0010 && i<1400)
	{
		wr_status = fdev_rword(fd,RD_WRITE_STATUS);
		i++;
	}
	fdev_wword(fd,WR_WRITE_STATUS,0x0020);				// write write-status reg
	watie_BC;
	fdev_wword(fd,WR_RAM_LENGTH_RD,Count);				// read ram data length
	watie_BC;
	fdev_wword(fd,WR_RAM_START_ADDR_RD,Offset);			// read ram address
	watie_BC;
	fdev_wword(fd,WR_RAM_REG_STATUS_RD,0x0000);			// read ram 0x0000 reg 0x0001
	watie_BC;
	fdev_wword(fd,START_READ_1553B,0x0000);				// read start 1553B
	
	rd_status = fdev_rword(fd,RD_RD_STATUS);			// read status start
	i=0;
	while(rd_status != 0x0001 && i<1200)
	{
		rd_status = fdev_rword(fd,RD_RD_STATUS);
		i++;
	}
	
	for(i=0;i<Count;i++)
	{
		Address[i]= fdev_rword(fd,BC_BASE_RAM_ADDR_R+i);
	}
	watie_BC;
	fdev_wword(fd,READ_DATA_OVER_ARM,0x0000);			// read data over		
	watie_BC;
	fdev_wword(fd,WR_WRITE_STATUS,0x0010);				// write status to STATUS REG
}
