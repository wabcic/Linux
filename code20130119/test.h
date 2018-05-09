/*
 *
 * (C) Copyright 2012
 *
 *	Ioctl command define.
 *
 *
 */

#ifndef __TEST_H__
#define __TEST_H__

#include <asm/ioctl.h>

#define FDEV_IOC_MAGIC 'F'

#define FDEV_IOC_RBYTE 	0x01
#define FDEV_IOC_WBYTE 	0x02
#define FDEV_IOC_RWORD 	0x03
#define FDEV_IOC_WWORD 	0x04
#define FDEV_IOC_RDWORD 0x05
#define FDEV_IOC_WDWORD 0x06
#define FDEV_IOC_R 	0x07
#define FDEV_IOC_W 	0x08
#define FDEV_IOC_RESET 	0x09
#define FDEV_IOC_BOTTOM 0x0a

#if 0
#define FDEV_IOC_RBYTE		_IOWR(FDEV_IOC_MAGIC, 0x0, void *)
#define FDEV_IOC_WBYTE		_IOWR(FDEV_IOC_MAGIC, 0x1, void *)
#define FDEV_IOC_RWORD		_IOWR(FDEV_IOC_MAGIC, 0x2, void *)
#define FDEV_IOC_WWORD		_IOWR(FDEV_IOC_MAGIC, 0x3, void *)
#define FDEV_IOC_RDWORD		_IOWR(FDEV_IOC_MAGIC, 0x4, void *)
#define FDEV_IOC_WDWORD		_IOWR(FDEV_IOC_MAGIC, 0x5, void *)
#define FDEV_IOC_R			_IOWR(FDEV_IOC_MAGIC, 0x6, void *)
#define FDEV_IOC_W			_IOWR(FDEV_IOC_MAGIC, 0x7, void *)
#define FDEV_IOC_RESET		_IOWR(FDEV_IOC_MAGIC, 0x8, void *)
#define FDEV_IOC_BOTTOM		0x9
#endif

struct fdev_byte {
	unsigned int addr;
	unsigned char dat;
};

struct fdev_word {
	unsigned int addr;
	unsigned short dat;
};

struct fdev_dword {
	unsigned int addr;
	unsigned int dat;
};

struct fdev_dat {
	unsigned int addr;
	unsigned int num;
	unsigned char *buf;
};

#endif
