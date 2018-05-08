/*
 * (C) Copyright 2012
 *
 *	Application to test arm board.
 *
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "test.h"
#include "testlib.h"

#ifdef FPAG_APP_DEBUG
#define FDEBUG(fmt, args...)	printf("APP: " fmt "\n", ##args)
#else
#define FDEBUG(fmt, args...)
#endif
#define FERR(fmt, args...)		printf("APP[ERR]: " fmt "\n", ##args)
#define  MAX_SEND_LEN   200

char *dev_file = "/dev/fpga_device";
int backflag=0;
extern int fd1;
char uploadDI[MAX_SEND_LEN]={0x00,0x00,0x12,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int upDI()
{
	/*int fd = 0;

	fd = open (dev_file, O_RDWR);
	if (fd < 0) {
		FERR("Failed to open %s.", dev_file);
		return fd;
	}
 */
		
      	// fdev_wword(fd1,0x0906,0xaaaa);
	int i,j;
	static int k=0;
        int array[8];
	for(j=0;j<2;j++)
	{	
           for(i=0;i<8;i++)
           {
            array[i]=fdev_rword(fd1, 0x0904);
           // array[i]=i;
	   }
	  if(!(array[0]||array[1]||array[2]||array[3]||array[4]||array[5]||array[6]||array[7]))
            {printf ("enter all 0\n");
           return -1;}
	  else { 
	      for(i=0;i<8;i++)              
              	printf("readDI=0x%4x\n",array[i]);

	  	  for(i=0;i<8;i++) 
	      {
                  uploadDI[k+28]=(char)array[i];
		  uploadDI[k+29]=(char)(array[i] >> 8);
		  printf("%4x ",uploadDI[k+28]);
		  k += 2;
              }
                 

           }
	  printf("\n");
       usleep(100);
            
	}
      // close(fd);

//	for(i=28;i<100;i++)
//		printf("%4x ",uploadDI[i]);
//     usleep(1000);

   
//	backflag=1;

 /* 
	int i,j;
       int array[8];
	for(j=0;j<12;j++)
	{
           for(i=0;i<8;i++)
           {
              array[i]=fdev_rword(fd, 0x0904);
	   }
	   if((array[0]||array[1]||array[2]||array[3]||array[4]||array[5]||array[6]||array[7]))
	   { 
	      for(i=0;i<8;i++)              
              printf("readDI=0x%4x\n",array[i]);
           }
	  printf("\n");
	}
       fdev_wword(fd,0x0902,0xaaaa);


*/

	


#if 0
	fdev_rbyte(fd, 0xAABB);
	fdev_wbyte(fd, 0xAACC, 55);
	fdev_rword(fd, 0xAADD);
	fdev_wword(fd, 0xAAEE, 66);
	fdev_rdword(fd, 0xBB00);
	fdev_wdword(fd, 0xBB11, 77);
#endif

	

	return 0;
}
