#include "xbasic_types.h"
#include "xstatus.h"


XStatus GpioIntrExample(XIntc* IntcInstancePtr,
                        XGpio* InstancePtr,
                        Xuint16 DeviceId,
                        Xuint16 IntrId,
                        Xuint16 IntrMask,
                        Xuint32 *DataRead);
