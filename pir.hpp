#ifndef PIR_HPP
#define PIR_HPP

#include <thread>
#include <mutex>
#include <atomic>

class PIR;

class PIRDelegate 
{
public:
    virtual void pirDidStart(PIR &pir) = 0;
    virtual void pirDidStop(PIR &pir) = 0;
    virtual void pirDidRecognizeMotion(PIR &pir) = 0;
};

class PIR 
{
    int pigpio;
    unsigned gpio;
    bool state {false};

    std::mutex enabledMutex;

    std::thread *worker {nullptr};
    std::atomic<bool>stopWorker {false};

    std::atomic<bool>enabled {false};

    void start();
    void stop();
    void updateState();

public:

    PIR(int pigpio, unsigned gpio);
    ~PIR();

    bool isEnabled();
    void setEnabled(bool enabled);

    PIRDelegate *delegate {nullptr};
};

#endif
