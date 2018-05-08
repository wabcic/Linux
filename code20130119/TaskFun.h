int setPara( GK_OPCUA_TASK *pTask );
int performTask( GK_OPCUA_TASK *pTask );
int startDO( GK_OPCUA_TASK *pTask );
int startDI( GK_OPCUA_TASK *pTask );
int stopDI( GK_OPCUA_TASK *pTask );
int stopPulseEN();
void initDI();

int setTime( GK_OPCUA_TASK *pTask );
int getTime( GK_OPCUA_TASK *pTask );

void distributeTask( unsigned long, GK_OPCUA_TASK * );

int startStepMotor( GK_OPCUA_TASK *pTask );
	
int startSynSignal( GK_OPCUA_TASK *pTask );

int setDO_N_Para( unsigned long addrPara, GK_OPCUA_TASK *pTask );
int setTimePara( GK_OPCUA_TASK *pTask );
int directPulseOut( GK_OPCUA_TASK *pTask );
int doConcurrentTask( GK_OPCUA_TASK *pTask );

int startSPI( GK_OPCUA_TASK *pTask );
int setCoderConfigPara( GK_OPCUA_TASK *pTask );
int setSwirConfigPara( GK_OPCUA_TASK *pTask );
int setVnirConfigPara( GK_OPCUA_TASK *pTask );
int doCoderConfig( GK_OPCUA_TASK *pTask );
int doSwirConfig( GK_OPCUA_TASK *pTask );
int doVnirConfig( GK_OPCUA_TASK *pTask );
void CoderCtrlMsgInit(void);
