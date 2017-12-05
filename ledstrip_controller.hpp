#ifndef LEDSTRIPCONTROLLER_HPP
#define LEDSTRIPCONTROLLER_HPP

#include <mutex>
#include <future>
#include "pir.hpp"

class LedStrip;

class LedStripController: PIRDelegate
{
    LedStrip *ledStrip {nullptr};
    PIR *pir {nullptr};

    std::mutex pirMutex;
    std::packaged_task<void()> *motionHandlerTask {nullptr};

public:
    ~LedStripController();
    void setup(LedStrip *ledStrip, PIR *pir);

    void run();

    void pirDidStart(PIR &pir);
    void pirDidStop(PIR &pir);
    void pirDidRecognizeMotion(PIR &pir);
};

#endif
