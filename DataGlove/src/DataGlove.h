#ifndef DATAGLOVE
#define DATAGLOVE

#include "I2CDev/helper_3dmath.h"
#include "data/InterruptListener.h"
#include "data/JointConfig.h"
#include "Finger.h"
#include <array>
#include "Util/Dictionary.h"
#include <utility>

#include <mutex>

#define NUM_FINGERS 5



class DataGlove : IInterruptListener {

public:
    DataGlove();
    ~DataGlove() = default;

    void OnInterrupt(FINGERTYPE type, bool first) override;
    INTERRUPTPIN GetMatchingInterrupt(FINGERTYPE type, bool first) override;
    void OnNewOrientationRetrieved(Quaternion quat, int index) override;
    void Cycle();

    std::mutex cycle_mutex;
    Quaternion orientations[NUM_FINGERS * UNITS_PER_FINGER];
private:
    void Init();
    Dictionary<int, Finger*> finger_map;
    std::array<Finger*, NUM_FINGERS> fingers;
};

#endif