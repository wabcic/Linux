/*********************************************************************************************************************
ÎÄŒþ£ºpdata.h
×÷Õß£º
ŽŽœšÊ±Œä£º
°æ±Ÿ£º
¹ŠÄÜ£º		1553ÍšÓÃÊýŸÝµÄ¶šÒå
Ïà¹ØÎÄŒþ£º
**********************************************************************************************************************/


#include"types.h"
#include"msg.h"
#include"bc.h"
#include<string.h>

UINT16 tc_periodNum;   							//¶šÒåÉÏ×ÜÏßÊ±ŒäÂë¹ã²¥ÖÜÆÚ
UINT16 st_periodNum;   							//¶šÒåÉÏ×ÜÏß×ÔŒì²âÖÜÆÚ
UINT16 pp_periodNum;   							//¶šÒåÉÏ×ÜÏß¹«ÓÃÊýŸÝÖÜÆÚ


//==========================time code information======================================================================
UINT32 g_dwTimeSec=0;
UINT16 g_wTimeMs=0;
UINT32 g_dwTickCout;

UINT16 g_Sysdata[3];
UINT16 g_Platdata[32];
UINT16 g_Datain[32];
UINT16 * g_Dataintmp;
UINT16  date_gap;



/*******************************************************************************************************************/

UINT8   *pPara;								// receive flag
UINT8   fd;
UINT8	timecodeflag;
UINT8	normalflag;
UINT8   datainnum;							// datainjectNUM
UINT8   datainframenum;							// datainframenum


UINT8  timecodeABflag;							//AB bus flag
UINT8  sysstatusABflag;
UINT8  platparaABflag;
UINT8  datainABflag;
UINT8  selftestABflag;
UINT8  controlcomABflag;
UINT8  stresstestABflag;			
									// BC send cmd 
UINT8  timecodesendflag;  						// time code flag 
UINT8  sysstatussendflag; 						// system status
UINT8  platparasendflag ;						// paltparam
UINT8  datainsendflag   ; 						// data inject 
UINT8  controlcomsendflag;						// control cmd   
UINT8  stresstestsendflag;

UINT8  selftestsendflag  ; 
UINT8  synyaocesendflag  ;  						
UINT8  ex_synyaocesendflag;						
UINT8  syndatainsendflag  ;  						

UINT8 timecodeperiod;
UINT8 sysstatusperiod;
UINT8 platparaperiod;



UINT8 controlcomperiod;
UINT8 datainperiod;
UINT8 dataincount;
UINT8 longtestperiod;
UINT8 vectorperiod;
UINT8 selftestperiod;

UINT16  CollectInfodect[32];

UINT8   *byadatainbufdata;
UINT8   *byaplatparamdata;
UINT8   *byaprjparam1data;
UINT8   *byaprjparam2data;
UINT8   *byaprjparam3data;
UINT8	*g_CollectInfodata; 		// get collection digital dect

UINT8   byadatainbuf[92];
UINT8   byaplatparam[92];
UINT8   byaprjparam1[92];
UINT8   byaprjparam2[92];
UINT8   byaprjparam3[92];
UINT8	g_CollectInfo[92]; 		// get collection digital dect

UINT8	g_CollectInfodigital; 
UINT8   g_CollectInfodatain;
UINT8   g_CollectInfoplat;
UINT8   g_CollectInfoprj1;
UINT8   g_CollectInfoprj2;
UINT8   g_CollectInfoprj3;	


		
UINT8 datainheader[28] ={0x00,0x00,0x12,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

UINT8 platparamheader[28] =
{0x00,0x00,0x12,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

UINT8 prjparam1header[28]={0x00,0x00,0x12,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

UINT8 prjparam2header[28]=
{0x00,0x00,0x12,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

UINT8 prjparam3header[28]={0x00,0x00,0x12,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// collect backinformation: plat_param\datain\prjparam1\prjparam2\prjparam3						
UINT8 digitalheader[28]={0x00,0x00,0x12,0x59,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};



UINT8   send_KZQ_EP;		
UINT8	synyaoce;
UINT8	syndatain;
UINT8	g_byServUsedNum;						//Ò»žöÖ¡ÖÜÆÚÖÐ·þÎñÇëÇóµÄŽÎÊý
UINT8	g_byAddUsedNum;							// 32žö²åÈëÏûÏ¢µÄÎ»ÖÃÊ¹ÓÃµÄžöÊý
UINT16  frameperiodNum;							// Ö¡œáÊøÖÜÆÚŒÆÊý 

//================================================small period number=====================
UINT16 g_byPeriodNum;
UINT16 g_byPeriodTimNum;			//timecode small period
UINT16 g_byPeriodTimNum;					
UINT16 g_byPeriodSysNum;
UINT16 g_byPeriodPlaNum;

//===============================================big period number====================
UINT16 g_byBigPeriodNum;			


//=========================BC 61580 operation buffer ========================
UINT16 g_wBCCurBufPos;
UINT32 g_dwBCFrames;
UINT32 g_dwBCFrameCouter;

//=========================BC error state   =====================================
UINT16 g_byMainState=0;

//=========================BC BIT×Ö»º³åÇø==================================
UINT16 g_waBIT[6];

//=========================RT error state========================================
UINT16 g_wRTState;


UINT16 g_byaPDHUdigiSize;						// PDHUÊý×ÖÁ¿ŒÆÊý

UINT8  g_byWriteNoteIndex;						// write data index
UINT8  g_bOddPeriod;							// ÆæÅŒÖÜÆÚ±êÊ¶
UINT8  sysTimeCode[6];							// system time code

UINT16  g_wErrMsgCount;


	/*message NO | message format | A/B bus | command1 | command2 */
const MSG_DEF g_MsgBuf[14] = {
	{ EMPTY_MESSAGE,   0x01, 1,0x0001, 0},	 /* off-line self test*/
	{ PLAT_PARAM,      0x00, 1,0xF860, 0},	 /* param  plat------------subaddr 3 datanum 32  */
	{ TIME_CODE,       0x00, 1,0xF883, 0},	 /* time   code------------subaddr 4 datanum 3   */
	{ CONTROL_CMD,     0x00, 1,0x3822, 0},	 /* control cmd------------subaddr 1 datanum 2   */ 	
	{ SYS_STATE, 	   0x00, 1,0xF8C3, 0},	 /* system status----------subaddr 6 datanum 3   */
	{ DATA_INJECT, 	   0x00, 1,0x3840, 0},	 /* data inject------------subaddr 2 datanum 32  */	

	{ DIGITAL_DECT,    0x00, 1,0x3E40, 0},	 /* digital dect-----------subaddr 18 datanum 32 */
	{ DATA_INJECT_BACK,0x00, 1,0x3C60, 0},	 /* data injectback--------subaddr 3  datanum 32 */	   				 
	{ PLAT_PARAM_BACK ,0x00, 1,0x3CA0, 0},	 /* plant param back-------subaddr 5  datanum 32 */	   				 
	{ PRJ_PARAM1,      0x00, 1,0x3C20, 0},	 /* project param1 --------subaddr 1  datanum 32 */	   				 
	{ PRJ_PARAM2,      0x00, 1,0x3C40, 0},	 /* project param2 --------subaddr 2  datanum 32 */
	{ PRJ_PARAM3,	   0X00, 1,0x3C80, 0},	 /* project param3 --------subaddr 4  datanum 32 */
	{ CONTROL_CMD_B,   0x00, 1,0xF822, 0},	 /* control cmd broadcast--subaddr 1 datanum 2   */
 	{ VECTOR_BICC,     0x00, 1,0x3FF0, 0}	 /* vector ----------------mode code 32 */
};
