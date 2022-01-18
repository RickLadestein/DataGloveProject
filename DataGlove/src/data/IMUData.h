#ifndef IMUDATA
#define IMUDATA

#include "inttypes.h"

enum class IMUADDR {
    ADDR_VARIANT_1 = 0x68,
    ADDR_VARIANT_2 = 0x69
};

//The imu config
struct IMUCONFIG {
    int16_t accel_offset_x;
    int16_t accel_offset_y;
    int16_t accel_offset_z;
    int16_t gyro_offset_x;
    int16_t gyro_offset_y;
    int16_t gyro_offset_z;
};

#endif