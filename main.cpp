#include <iostream>
#include <string>
#include <cstdio>
#include <cerrno>
#include <cstdlib>

#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#include <pigpiod_if2.h>
#include "ledstrip_controller.hpp"
#include "ledstrip.hpp"
#include "pir.hpp"

using namespace std;

static const unsigned GPIORed = 15;
static const unsigned GPIOGreen = 14;
static const unsigned GPIOBlue = 18;

static const unsigned GPIOPIR = 5;

int main(int argc, char * argv[]) {
    int pigpio = pigpio_start(NULL, NULL);
    if (pigpio < 0) {
        cout << "PIGPIO initialization failed" << endl;
        return 1;
    }

    LedStrip leds(pigpio, GPIORed, GPIOGreen, GPIOBlue);
    leds.setColor({144, 12, 54});
    // leds.setColor({40, 212, 154});

    cout << "red: " << leds.color().red << endl;

    PIR pir(pigpio, GPIOPIR);

    LedStripController controller;
    controller.setup(&leds, &pir);

    controller.run();

    pigpio_stop(pigpio);
    return 0;
}
