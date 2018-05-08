/*
 * (C) Copyright 2012
 *
 *	Library header file.
 *
 *
 */

#ifndef __TEST_LIB_H__
#define __TEST_LIB_H__

int fdev_open();
void fdev_close(int fd);

unsigned char fdev_rbyte(int fd, unsigned int addr);
int fdev_wbyte(int fd, unsigned int addr, unsigned char dat);
unsigned short fdev_rword(int fd, unsigned int addr);
int fdev_wword(int fd, unsigned int addr, unsigned short dat);
unsigned int fdev_rdword(int fd, unsigned int addr);
int fdev_wdword(int fd, unsigned int addr, unsigned int dat);
int fdev_reset(int fd);
#endif
