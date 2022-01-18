#include "I2CMux.h"

MUXADDR current_addr;
I2CMUXADDR own_addr;

void I2CMuxInit(I2CMUXADDR unit_addr, MUXADDR i2c_addr) {
    current_addr = i2c_addr;
    own_addr = unit_addr;
}
void I2CMuxSetChannel(MUXADDR addr) {
    Wire.beginTransmission((int)own_addr);
    Wire.write((int)addr);
    Wire.endTransmission();
}
MUXADDR I2CMuxGetCurrChannel() {
    return current_addr;
}

void I2CMuxScanChannel(MUXADDR channel) {
    I2CMuxSetChannel(channel);
    Serial.print("I2C mux: Seaching channel [");
    Serial.print((int)channel);
    Serial.println("] for available I2C devices");
    for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == (uint8_t)own_addr) {
            Serial.println("Found TCA9548A multiplexer");
        } else {
            Wire.beginTransmission(addr);
            if (!Wire.endTransmission()) {
                Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
            }
        }
      }
}