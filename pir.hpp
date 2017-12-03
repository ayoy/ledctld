#ifndef PIR_HPP
#define PIR_HPP

#include <thread>

class PIR {

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

    void(*didRecognizeMotion)(const PIR &) = nullptr;
};

#endif
