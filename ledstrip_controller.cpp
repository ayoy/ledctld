#include "ledstrip_controller.hpp"
#include "ledstrip.hpp"
#include "pir.hpp"

void LedStripController::setup(LedStrip *ledStrip, PIR *pir)
{
    this->ledStrip = ledStrip;
    this->pir = pir;

    this->pir->delegate = this;
}

void LedStripController::pirDidRecognizeMotion(const PIR &pir)
{
    if (pir.isEnabled()) {
        printf("Motion recognized!\n");

        this->ledStrip->fadeIn();
    }
}