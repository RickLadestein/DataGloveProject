#ifndef FINGER
#define FINGER

#include "Joint.h"
#include "I2CMux.h"
#include "data/InterruptListener.h"
#include "data/JointConfig.h"
#include "I2CDev/helper_3dmath.h"
#include <array>
#include "Util/Dictionary.h"

#define UNITS_PER_FINGER 2

class Finger {
    public:
    Finger(FINGERTYPE type, JointConfig c1, JointConfig c2, MUXADDR channel, IInterruptListener* d_listener);
    Finger(FINGERTYPE type, JointConfig c1, MUXADDR channel, IInterruptListener* d_listener);
    void Update();
    void GetOrientations(Quaternion* quat);
    void AttatchJointInterrupt(JointConfig config, bool first);

    std::array<INTERRUPTPIN, UNITS_PER_FINGER> interrupts;
    private:
    Dictionary<int, Joint*> joint_map;
    Joint* first_joint;
    Joint* second_joint;
    MUXADDR mux_channel;
};

#endif