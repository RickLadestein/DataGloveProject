#ifndef JOINTCONFIG
#define JOINTCONFIG

#include "InterruptListener.h"
#include "IMUData.h"
#include "MuxData.h"

typedef void (*voidFuncPtr)(void);

struct JointConfig {
    IMUCONFIG config; //The IMU config for configuring imu offsets
    MUXADDR channel; // The channel that the IMU is on in the mux
    IMUADDR i2c_addr; // The physical i2c adress of the IMU
    INTERRUPTPIN interrupt_pin; // The pin number of the interrupt associated with the Joint
    FINGERTYPE finger;
    voidFuncPtr InterruptFunc;
    int orientation_idx;
};
#endif