#include "ledstrip.hpp"
#include <pigpiod_if2.h>
#include <iostream>

using namespace std;

static const Color & gBlack = {0,0,0};

Color::Color(unsigned red, unsigned green, unsigned blue)
{
    this->red = max(min(red, 255u), 0u);
    this->green = max(min(green, 255u), 0u);
    this->blue = max(min(blue, 255u), 0u);
}

const Color & Color::black()
{
    return gBlack;
}

void LedStrip::setColor(const Color &color)
{
    this->_color = color;
    cout << "color: " << this->_color.red << " " << this->_color.green << " " << this->_color.blue << endl;

    set_PWM_dutycycle(this->pigpio, this->gpioR, this->_color.red);
    set_PWM_dutycycle(this->pigpio, this->gpioG, this->_color.green);
    set_PWM_dutycycle(this->pigpio, this->gpioB, this->_color.blue);
}
