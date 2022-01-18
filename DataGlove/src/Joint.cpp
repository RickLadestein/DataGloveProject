#include "Joint.h"

#include "I2CDev/I2Cdev.h"
#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
//#include "I2CDev/MPU6050.h"

#include "I2CDev/MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include <sstream>

Joint::Joint(JointConfig _config, IInterruptListener* listener) {
    this->dataglovelistener = listener;
    this->config = _config;
    this->orientation = Quaternion();
}

Joint::~Joint() {
    delete this->sensor;
}

InitResult Joint::Init() {
    Serial.println("ïnitializing joint");
    InitResult rs = {std::string(""), true};
    std::stringstream ss;

    sensor = new MPU6050((uint8_t)config.i2c_addr);

    sensor->initialize();
    if(!sensor->testConnection()) {
        ss << "Test connection failed: could not connect to sensor";
        rs = {ss.str(), false};
        Serial.println(ss.str().c_str());
        return rs;
    }

    devStatus = sensor->dmpInitialize();
    sensor->setXGyroOffset(config.config.gyro_offset_x);
    sensor->setYGyroOffset(config.config.gyro_offset_y);
    sensor->setZGyroOffset(config.config.gyro_offset_z);
    sensor->setZAccelOffset(config.config.accel_offset_z);

    if (devStatus == 0) {
        //DMP was initialised
        sensor->setDMPEnabled(true);
        mpuIntStatus = sensor->getIntStatus();
        dmpReady = true;
        packetSize = sensor->dmpGetFIFOPacketSize();
        Serial.println("IMU INIT SUCCES");
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        ss << "Joint Status: INIT ERROR (code " << (int)devStatus << ")";
        rs = {ss.str(), false};
        Serial.println(ss.str().c_str());
    }
    return rs;
}

void Joint::SetOrientation(float nw, float nx, float ny, float nz) {
    orientation = Quaternion(nw, nx, ny, nz);
}

void Joint::Update() {
    mpuIntStatus = sensor->getIntStatus();

    // get current FIFO count
    fifoCount = sensor->getFIFOCount();
    
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        sensor->resetFIFO();
    }
    
    if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) {
            fifoCount = sensor->getFIFOCount();
        } 
        // read a packet from FIFO
        sensor->getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        sensor->dmpGetQuaternion(&orientation, fifoBuffer);


        this->dataglovelistener->OnNewOrientationRetrieved(orientation, this->config.orientation_idx);
    }
    return;
}


