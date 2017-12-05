#include "ledstrip_controller.hpp"
#include "ledstrip.hpp"
#include "pir.hpp"
#include <thread>
#include <chrono>
#include <iostream>


LedStripController::~LedStripController()
{
    if (this->motionHandlerTask) {
        delete this->motionHandlerTask;
        this->motionHandlerTask = nullptr;
    }
}

void LedStripController::setup(LedStrip *ledStrip, PIR *pir)
{
    this->ledStrip = ledStrip;
    this->pir = pir;

    this->pir->delegate = this;
}

void LedStripController::run()
{
    std::cout << "Thread " << std::this_thread::get_id() << std::endl;

    this->pir->setEnabled(true);
    this->ledStrip->setEnabled(true);

    while (true) {
        {
            std::unique_lock<std::mutex> lock(pirMutex);

            if (this->motionHandlerTask) {
                auto task(std::move(*this->motionHandlerTask));

                std::cout << "Running task " << std::this_thread::get_id() << std::endl;
                task();
                std::cout << "Task finished " << std::this_thread::get_id() << std::endl;

                delete this->motionHandlerTask;
                this->motionHandlerTask = nullptr;
                this->pir->setEnabled(true);

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void LedStripController::pirDidStart(PIR &pir)
{
    std::cout << "PIR did start" << std::endl;
    this->ledStrip->setColor(this->ledStrip->color(), 0);
}

void LedStripController::pirDidStop(PIR &pir)
{
    std::cout << "PIR did stop" << std::endl;
}

void LedStripController::pirDidRecognizeMotion(PIR &pir)
{
    std::unique_lock<std::mutex> lock(pirMutex);

    if (!this->motionHandlerTask) {
        this->motionHandlerTask = new std::packaged_task<void()>([&]{
            std::cout << "Thread " << std::this_thread::get_id() << std::endl;
            std::cout << "Motion recognized!" << std::endl;
            this->pir->setEnabled(false);

            this->ledStrip->fadeIn();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            this->ledStrip->fadeOut();
        });
    }
}
