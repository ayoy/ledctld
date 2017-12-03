#ifndef PIR_HPP
#define PIR_HPP

#include <thread>

class PIR;

class PIRDelegate 
{
public:
    virtual void pirDidRecognizeMotion(const PIR &pir) = 0;
};

class PIR 
{

    int pigpio;
    unsigned gpio;
    bool enabled {false};
    int state {false};

    void start();
    void stop();
    void updateState();

    std::thread *worker {nullptr};

public:

    PIR(int pigpio, unsigned gpio);
    ~PIR();

    bool isEnabled() const;
    void setEnabled(bool enabled);

    PIRDelegate *delegate {nullptr};
};

#endif
