#ifndef INTERRUPTLISTENER
#define INTERRUPTLISTENER
#include <utility>


enum FINGERTYPE {
    THUMB = 0,
    INDEX = 1,
    MIDDLE = 2,
    RING = 3,
    PINK = 4
};

enum class INTERRUPTPIN {
    INVALID = -1,
    PIN_D2 = 2,
    PIN_D4 = 4,
    PIN_D5 = 5,
    PIN_D12 = 12,
    PIN_D13 = 13,
    PIN_D14 = 14,
    PIN_D15 = 15,
    PIN_D18 = 18,
    PIN_D19 = 19,
    PIN_D25 = 25,
    PIN_D26 = 26,
    PIN_D27 = 27,
    PIN_D32 = 32,
    PIN_D33 = 33,
    PIN_D34 = 34,
    PIN_D35 = 35
    
};

typedef std::pair<FINGERTYPE, INTERRUPTPIN> InterruptArgs;


class IInterruptListener {
    public:
    virtual void OnInterrupt(FINGERTYPE type, bool first);
    virtual INTERRUPTPIN GetMatchingInterrupt(FINGERTYPE type, bool first);
    virtual void OnNewOrientationRetrieved(Quaternion quat, int index);
};
#endif