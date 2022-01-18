#define CONFIG_COMPILER_CXX_EXCEPTIONS

#include "Arduino.h"
#include "DataGlove.h"
DataGlove * dt;



#define UPDATE_RATE 100



//#define DATAGLOVE_BAUD 300
//#define DATAGLOVE_BAUD 600
//#define DATAGLOVE_BAUD 1200
//#define DATAGLOVE_BAUD 2400
//#define DATAGLOVE_BAUD 4800
//#define DATAGLOVE_BAUD 9600
//#define DATAGLOVE_BAUD 14400
//#define DATAGLOVE_BAUD 19200
//#define DATAGLOVE_BAUD 28800
//#define DATAGLOVE_BAUD 38400
#define DATAGLOVE_BAUD 57600
//#define DATAGLOVE_BAUD 115200



TaskHandle_t task_handle;
void SerialTask(void* parameter) {
    size_t buffer_size = (sizeof(Quaternion) * NUM_FINGERS * UNITS_PER_FINGER) + 4;
    char buffer[buffer_size];

    Quaternion* quats = (Quaternion*)buffer;

    buffer[0] = (buffer_size >> 24) & 0xFF;
    buffer[1] = (buffer_size >> 16) & 0xFF;
    buffer[2] = (buffer_size >> 8) & 0xFF;
    buffer[3] = (buffer_size >> 0) & 0xFF;

    while(true) {
       dt->cycle_mutex.lock();
       memcpy(buffer, dt->orientations, buffer_size);
       dt->cycle_mutex.unlock();


       for(int i = 0; i < (NUM_FINGERS * UNITS_PER_FINGER); i++) {
           Quaternion curr = quats[i];
           Serial.print(i);
           Serial.print("\t");
           Serial.print(curr.w);
           Serial.print("\t");
           Serial.print(curr.x);
           Serial.print("\t");
           Serial.print(curr.y);
           Serial.print("\t");
           Serial.println(curr.z);
       }
        
        //Serial.write(buffer, buffer_size);
        //Serial.flush();
        //Serial.println();
        vTaskDelay(UPDATE_RATE / portTICK_PERIOD_MS);
    }
}

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        //Wire.begin(-1, -1, 400000);
        //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(DATAGLOVE_BAUD);
    I2CMuxInit(I2CMUXADDR::VARIANT_DEFAULT, MUXADDR::MUX_ADDR_0);


    dt = new DataGlove();
   xTaskCreatePinnedToCore(
      SerialTask, 
      "SerialHandler", 
      100000, 
      NULL, 
      0, 
      &task_handle, 
      0);
}



void loop() {
    dt->Cycle();
    //vTaskDelay(UPDATE_RATE / portTICK_PERIOD_MS);
}