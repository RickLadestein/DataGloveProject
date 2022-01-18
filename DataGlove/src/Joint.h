#ifndef JOINT
#define JOINT

#include "I2CDev/helper_3dmath.h"
/*


#include "I2CDev/I2Cdev.h"
//#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
//#include "I2CDev/MPU6050.h"

#include "I2CDev/MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
*/

#include "data/JointConfig.h"
#include "data/InterruptListener.h"
#include <array>
#include <inttypes.h>
#include "I2CMux.h"
#include <string>
#include <utility>

typedef std::pair<std::string, bool> InitResult;

class MPU6050;

class Joint {
    public:
        Quaternion orientation;
        JointConfig config;
        Joint(JointConfig _config, IInterruptListener* listener);
        ~Joint();
        InitResult Init();
        Quaternion GetOrientation();
        void SetOrientation(float nw, float nx, float ny, float nz);
        void Update();

    private:
        IInterruptListener* dataglovelistener;
        MPU6050* sensor;
        bool dmpReady = false;  // set true if DMP init was successful
        uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        uint16_t fifoCount;     // count of all bytes currently in FIFO
        uint8_t fifoBuffer[64]; // FIFO storage buffer
};

#endif