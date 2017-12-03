#include "pir.hpp"
#include <thread>
#include <chrono>
#include <pigpiod_if2.h>

using namespace std;

PIR::PIR(int pigpio, unsigned gpio): pigpio(pigpio), gpio(gpio)
{
    set_mode(this->pigpio, this->gpio, PI_INPUT);
}

PIR::~PIR() 
{
    if (this->worker) {
        delete this->worker;
        this->worker = nullptr;
    }
}

bool PIR::isEnabled() const
{
    return this->enabled;
}

void PIR::setEnabled(bool enabled)
{
    if (this->enabled != enabled) {
        this->enabled = enabled;
        if (this->enabled) {
            this->start();
        } else {
            this->stop();
        }
    }
}

void PIR::start()
{
    this->worker = new thread(&PIR::updateState, this);
}

void PIR::stop()
{
    if (this->worker) {
        delete this->worker;
        this->worker = nullptr;
    }
}

void PIR::updateState()
{
    this->state = false;

    int gpioState = 0;

    while (true) {

        gpioState = gpio_read(this->pigpio, this->gpio);
        printf("State: %d\n", gpioState);

        if (gpioState != this->state) {
            this->state = gpioState;

            if (this->state && this->delegate) {
                this->delegate->pirDidRecognizeMotion(*this);
            }
            this_thread::sleep_for(5s);
        } else {
            this_thread::sleep_for(500ms);
        }
    }
}
