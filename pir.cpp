#include "pir.hpp"
#include <chrono>
#include <iostream>
#include <future>
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

bool PIR::isEnabled()
{
    bool value = false;
    {
        std::lock_guard<std::mutex> guard(this->enabledMutex);
        value = this->enabled.load();
    }
    return value;
}

void PIR::setEnabled(bool enabled)
{
    std::lock_guard<std::mutex> guard(this->enabledMutex);
    std::cout << "setEnabled: " << enabled << std::endl;
    if (this->enabled.load() != enabled) {
        this->enabled.store(enabled);
        if (this->enabled) {
            this->start();
        } else {
            this->stop();
        }
    }
}


void PIR::start()
{
    std::cout << "start" << std::endl;
    if (this->worker) {
        delete this->worker;
    }
    this->worker = new std::thread(&PIR::updateState, this);
    if (this->delegate) {
        this->delegate->pirDidStart(*this);
    }
}

void PIR::stop()
{
    if (this->worker) {
        std::cout << "stop" << std::endl;
        this->stopWorker.store(true);
        this->worker->join();
        delete this->worker;
        std::cout << "thread stopped and deleted" << std::endl;
        this->worker = nullptr;
        if (this->delegate) {
            this->delegate->pirDidStop(*this);
        }
    }
}

void PIR::updateState()
{
    this->state = false;
    this->stopWorker = false;

    int gpioState = 0;

    while (gpioState == 0 && !this->stopWorker.load()) {

        gpioState = gpio_read(this->pigpio, this->gpio);
        std::cout << "State: " << gpioState << std::endl;

        if (gpioState != this->state) {
            this->state = gpioState;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }


    if (this->state && this->delegate) {
        std::cout << "Thread " << std::this_thread::get_id() << std::endl;
        this->delegate->pirDidRecognizeMotion(*this);
        std::cout << "PIR thread has finished" << std::endl;
    }
}
