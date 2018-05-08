/*
 * (C) Copyright 2012
 *
 *	Library to test board.
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "test.h"

#ifdef FPAG_LIB_DEBUG
#define FDEBUG(fmt, args...)	printf("LIB: " fmt "\n", ##args)
#else
#define FDEBUG(fmt, args...)
#endif
#define FERR(fmt, args...)		printf("LIB[ERR]: " fmt "\n", ##args)


char *dev_name = "/dev/fpga_device";

int fdev_open()
{
	int fd;

	FDEBUG("fdev_open()...");

	fd	= open(dev_name, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_name);
		return -1;
	}

	return fd;
}

void fdev_close(int fd)
{
	FDEBUG("fdev_close()...");

	close(fd);
}

extern unsigned char fdev_rbyte(int fd, unsigned int addr)
{
	int ret = 0;
	struct fdev_byte fb;

	fb.addr = addr;

	ret = ioctl(fd, FDEV_IOC_RBYTE, (void *)&fb);
	if (ret < 0) {
		return ret;
	}

	return fb.dat;
}

int fdev_wbyte(int fd, unsigned int addr, unsigned char dat)
{
	int ret = 0;
	struct fdev_byte fb;

	fb.addr = addr;
	fb.dat = dat;

//	printf("zb test for lib in fuc (fdev_wbyte).\n");
	ret = ioctl(fd, FDEV_IOC_WBYTE, (void *)&fb);
	printf("zb test for lib in fuc (fdev_wbyte).\n");
	if (ret < 0) {
		return ret;
	}

	return 0;
}

unsigned short fdev_rword(int fd, unsigned int addr)
{
	int ret = 0;
	struct fdev_word fw;

	fw.addr = addr;
//	printf("test for lib in fun (fdev_rword).\n");
	ret = ioctl(fd, FDEV_IOC_RWORD, (void *)&fw);
//	printf("test for lib in fun (fdev_rword).\n");
	if (ret < 0) {
		return ret;
	}

	return fw.dat;
}

int fdev_wword(int fd, unsigned int addr, unsigned short dat)
{
	int ret = 0;
	struct fdev_word fw;

	fw.addr = addr;
	fw.dat = dat;
//	printf("test for lib in fun (fdev_wword).\n");
	ret = ioctl(fd, FDEV_IOC_WWORD, (void *)&fw);
//	printf("test for lib in fun (fdev_wword).\n");
	if (ret < 0) {
		return ret;
	}

	return 0;
}

unsigned int fdev_rdword(int fd, unsigned int addr)
{
	int ret = 0;
	struct fdev_dword fdw;

	fdw.addr = addr;
	 
	ret = ioctl(fd, FDEV_IOC_RDWORD, (void *)&fdw);
	if (ret < 0) {
		return ret;
	}

	return fdw.dat;
}

int fdev_wdword(int fd, unsigned int addr, unsigned int dat)
{
	int ret = 0;
	struct fdev_dword fdw;

	fdw.addr = addr;
	fdw.dat = dat;

	ret = ioctl(fd, FDEV_IOC_WDWORD, (void *)&fdw);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int fdev_reset(int fd)
{
	return ioctl(fd, FDEV_IOC_RESET, NULL);
}

#if 0
int fdev_r(int fd, unsigned char start, unsigned int size, unsigned char *buf)
{
	return 0;
}

int fdev_w(int fd, unsigned char start, unsigned int size, unsigned char *buf)
{
	return 0;
}
#endif
