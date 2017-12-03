#ifndef LEDSTRIPCONTROLLER_HPP
#define LEDSTRIPCONTROLLER_HPP

#include "pir.hpp"

class LedStrip;


class LedStripController: PIRDelegate
{
    LedStrip *ledStrip {nullptr};
    PIR *pir {nullptr};

public:
    void setup(LedStrip *ledStrip, PIR *pir);
    void pirDidRecognizeMotion(const PIR &pir);
};

#endif