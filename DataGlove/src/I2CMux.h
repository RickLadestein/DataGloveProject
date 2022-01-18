#ifndef I2CMUX
#define I2CMUX

#include <I2CDev/I2Cdev.h>
#include <inttypes.h>
#include <memory>
#include "data/MuxData.h"

void I2CMuxInit(I2CMUXADDR unit_addr, MUXADDR i2c_addr);
void I2CMuxSetChannel(MUXADDR addr);
MUXADDR I2CMuxGetCurrChannel();
void I2CMuxScanChannel(MUXADDR channel);


#endif


