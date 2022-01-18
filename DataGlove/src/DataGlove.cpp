#include "DataGlove.h"
#include <queue>
#include <exception>

#pragma region Interrupts
IInterruptListener* listener;
void CallInterrupt(FINGERTYPE type, bool first) { 
    if(listener != nullptr) {
        listener->OnInterrupt(type, first);
    }
}

void ThumbFirstInterruptFunc() { CallInterrupt(FINGERTYPE::THUMB, true); }
void ThumbSecondInterruptFunc() { CallInterrupt(FINGERTYPE::THUMB, false); }
void IndexFirstInterruptFunc() { CallInterrupt(FINGERTYPE::INDEX, true); }
void IndexSecondInterruptFunc() { CallInterrupt(FINGERTYPE::INDEX, false); }
void MiddleFirstInterruptFunc() { CallInterrupt(FINGERTYPE::MIDDLE, true); }
void MiddleSecondInterruptFunc() { CallInterrupt(FINGERTYPE::MIDDLE, false); }
void RingFirstInterruptFunc() { CallInterrupt(FINGERTYPE::RING, true); }
void RingSecondInterruptFunc() { CallInterrupt(FINGERTYPE::RING, false); }
void PinkFirstInterruptFunc() { CallInterrupt(FINGERTYPE::PINK, true); }
void PinkSecondInterruptFunc() { CallInterrupt(FINGERTYPE::PINK, false); }


#pragma endregion

#pragma region Configs
IMUCONFIG mpu_configs[] = {
{-1576, 700,    496,    52,     0,      -45     },
{-3243, 118,    1008,   12,     -77,    -7      },
{6,     2653,   1817,   -139,   62,     23      },
{2326,  -1575,  1786,   91,     -30,    1       },
{-2463, -891,   1098,   -536,   -7,     -17     },
{470,   523,    1387,   -251,   84,     -45     },
{-6,    -1834,  1661,   112,    -28,    56      },
{-3034, -1491,  1708,   102,    2,      -63     },
{-1808, 2506,   1189,   -11,    10,     -6      },
{-3290, 508,    1260,   49,     -23,    -8      },
{-485,  -14,    1392,   11,     84,     53      },
{-238,  933,    1199,   525,    -57,    13      },
{1237,  -108,   803,    57,     -46,    19      },
{-1657, -1349,  2303,   -186,   46,     53      },
{199,   -3501,  1312,   82,     60,     29      }
};

JointConfig joint_configs[] = {
    {mpu_configs[6], MUXADDR::MUX_ADDR_6, IMUADDR::ADDR_VARIANT_1, INTERRUPTPIN::PIN_D13, FINGERTYPE::THUMB, ThumbFirstInterruptFunc, 0},
    {mpu_configs[14], MUXADDR::MUX_ADDR_6, IMUADDR::ADDR_VARIANT_2, INTERRUPTPIN::PIN_D13, FINGERTYPE::THUMB, ThumbSecondInterruptFunc, 1},
    {mpu_configs[0], MUXADDR::MUX_ADDR_5, IMUADDR::ADDR_VARIANT_1, INTERRUPTPIN::PIN_D12, FINGERTYPE::INDEX, IndexFirstInterruptFunc, 2},
    {mpu_configs[1], MUXADDR::MUX_ADDR_5, IMUADDR::ADDR_VARIANT_2, INTERRUPTPIN::PIN_D14, FINGERTYPE::INDEX, IndexSecondInterruptFunc, 3},
    {mpu_configs[12], MUXADDR::MUX_ADDR_4, IMUADDR::ADDR_VARIANT_1, INTERRUPTPIN::PIN_D27, FINGERTYPE::MIDDLE, MiddleFirstInterruptFunc, 4},
    {mpu_configs[13], MUXADDR::MUX_ADDR_4, IMUADDR::ADDR_VARIANT_2, INTERRUPTPIN::PIN_D26, FINGERTYPE::MIDDLE, MiddleSecondInterruptFunc, 5},
    {mpu_configs[4], MUXADDR::MUX_ADDR_3, IMUADDR::ADDR_VARIANT_1, INTERRUPTPIN::PIN_D25, FINGERTYPE::RING, RingFirstInterruptFunc, 6},
    {mpu_configs[2], MUXADDR::MUX_ADDR_3, IMUADDR::ADDR_VARIANT_2, INTERRUPTPIN::PIN_D33, FINGERTYPE::RING, RingSecondInterruptFunc, 7},
    {mpu_configs[7], MUXADDR::MUX_ADDR_2, IMUADDR::ADDR_VARIANT_1, INTERRUPTPIN::PIN_D32, FINGERTYPE::PINK, PinkFirstInterruptFunc, 8},
    {mpu_configs[14], MUXADDR::MUX_ADDR_2, IMUADDR::ADDR_VARIANT_2, INTERRUPTPIN::PIN_D32, FINGERTYPE::PINK, PinkSecondInterruptFunc, 9}
};
#pragma endregion



std::queue<InterruptArgs> queue;
std::queue<std::pair<FINGERTYPE, bool>> search_queue;


DataGlove::DataGlove() {
    size_t total_arr_size = NUM_FINGERS * UNITS_PER_FINGER;
    for(size_t i = 0; i < total_arr_size; i++) {
        orientations[i] = Quaternion();
    }
    Init();
}

void DataGlove::Init() {
    listener = this;
    I2CMuxInit(I2CMUXADDR::VARIANT_DEFAULT, MUXADDR::MUX_ADDR_0);
    for(int i = 0; i < fingers.size(); i++) {
        fingers[i] = nullptr;
    }

    Finger* curr_finger;

    curr_finger = new Finger(FINGERTYPE::THUMB, joint_configs[0], MUXADDR::MUX_ADDR_6, this);
    finger_map.emplace((int)FINGERTYPE::THUMB, curr_finger);
    fingers[0] = curr_finger;
//
    curr_finger = new Finger(FINGERTYPE::INDEX, joint_configs[2], joint_configs[3], MUXADDR::MUX_ADDR_5, this);
    finger_map.emplace((int)FINGERTYPE::INDEX, curr_finger);
    fingers[1] = curr_finger;
//
//
    curr_finger = new Finger(FINGERTYPE::MIDDLE, joint_configs[4], joint_configs[5], MUXADDR::MUX_ADDR_4, this);
    finger_map.emplace((int)FINGERTYPE::MIDDLE, curr_finger);
    fingers[2] = curr_finger;
//    //
    curr_finger = new Finger(FINGERTYPE::RING, joint_configs[6], joint_configs[7], MUXADDR::MUX_ADDR_3, this);
    finger_map.emplace((int)FINGERTYPE::RING, curr_finger);
    fingers[3] = curr_finger;
//    
    curr_finger = new Finger(FINGERTYPE::PINK, joint_configs[8], MUXADDR::MUX_ADDR_2, this);
    finger_map.emplace((int)FINGERTYPE::PINK, curr_finger);
    fingers[4] = curr_finger;
}

void DataGlove::Cycle() {
    for(int i = 0 ; i < NUM_FINGERS; i++) {
        Finger* curr = fingers[i];
        if(curr != nullptr) {
            curr->Update();
        }
    }
    return;
}

void DataGlove::OnInterrupt(FINGERTYPE type, bool first) {
    //search_queue.push(std::pair<FINGERTYPE, bool>(type, first));
}

INTERRUPTPIN DataGlove::GetMatchingInterrupt(FINGERTYPE type, bool first) {
    Finger* result;
    bool found = finger_map.try_get_value((int)type, result);
    if(found) {
        if(first) {
            return result->interrupts[0];
        }
        return result->interrupts[1];
    } else {
        return INTERRUPTPIN::INVALID;
    }
}

void DataGlove::OnNewOrientationRetrieved(Quaternion quat, int index) {
    cycle_mutex.lock();
    orientations[index] = quat;
    cycle_mutex.unlock();
}