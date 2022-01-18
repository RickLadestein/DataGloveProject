#include "Finger.h"

Finger::Finger(FINGERTYPE type, JointConfig c1, JointConfig c2, MUXADDR channel, IInterruptListener* d_listener) {
    mux_channel = channel;
    this->first_joint = nullptr;
    this->second_joint = nullptr;
    I2CMuxSetChannel(channel);

    Serial.println("-----------------------------------------------------------------");
    Serial.print("FINGER INIT: ");
    Serial.println((int) type);

    Joint* first = new Joint(c1, d_listener);
    InitResult result = first->Init();
    joint_map.emplace((int)c1.interrupt_pin, first);
    this->first_joint = first;
    //AttatchJointInterrupt(c1, true);

    Joint* second = new Joint(c2, d_listener);
    second->Init();
    joint_map.emplace((int)c2.interrupt_pin, second);
    this->second_joint = second;
    //AttatchJointInterrupt(c2, false);
    Serial.println("-----------------------------------------------------------------");
}

Finger::Finger(FINGERTYPE type, JointConfig c1, MUXADDR channel, IInterruptListener* d_listener) {
    this->first_joint = nullptr;
    this->second_joint = nullptr;
    mux_channel = channel;
    I2CMuxSetChannel(channel);

    Serial.println("-----------------------------------------------------------------");
    Serial.print("FINGER INIT: ");  
    Joint* first = new Joint(c1, d_listener);
    InitResult result = first->Init();
    joint_map.emplace((int)c1.interrupt_pin, first);
    this->first_joint = first;
    //AttatchJointInterrupt(c1, true);
    Serial.println("-----------------------------------------------------------------");
}

void Finger::AttatchJointInterrupt(JointConfig config, bool first) {
    
    if(first) {
        interrupts[0] = config.interrupt_pin;
    } else {
        interrupts[1] = config.interrupt_pin;
    }
    int pin = (int)config.interrupt_pin;
    attachInterrupt(pin, config.InterruptFunc, RISING);
}

void Finger::Update() {
        I2CMuxSetChannel(mux_channel);
        if(first_joint != nullptr) {
            first_joint->Update();
        }

        if(second_joint != nullptr) {
            second_joint->Update();
        }
}