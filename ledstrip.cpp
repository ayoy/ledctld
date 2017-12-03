#include "ledstrip.hpp"
#include <pigpiod_if2.h>
#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

static const Color & gBlack = {0,0,0};

Color::Color(unsigned red, unsigned green, unsigned blue):
    red(max(min(red, 255u), 0u)),
    green(max(min(green, 255u), 0u)),
    blue(max(min(blue, 255u), 0u))
{
}

bool Color::operator==(const Color &rhs) const 
{
    return this->red == rhs.red && this->green == rhs.green && this->blue == rhs.blue;
}

const Color & Color::black()
{
    return gBlack;
}

//

// values as per https://stackoverflow.com/a/596243/969818

static const double Y_RED = 0.299;
static const double Y_GREEN = 0.587;
static const double Y_BLUE = 0.114;

GPIOColor::GPIOColor(const Color &color, double brightness) : 
    _color(color), 
    _brightness(brightness)
{
    applyBrightness();
}

void GPIOColor::setBrightness(double brightness)
{
    brightness = max(min(brightness, 1.0), 0.0);
    if (abs(brightness - this->_brightness) < std::numeric_limits<double>::epsilon()) {
        this->_brightness = brightness;
        this->applyBrightness();
    }
}

void GPIOColor::applyBrightness() 
{
    double redLuminance = this->_color.red * Y_RED;
    double greenLuminance = this->_color.green * Y_GREEN;
    double blueLuminance = this->_color.blue * Y_BLUE;

    this->_red = this->_brightness * redLuminance / Y_RED;
    this->_green = this->_brightness * greenLuminance / Y_GREEN;
    this->_blue = this->_brightness * blueLuminance / Y_BLUE;
}

//

void LedStrip::setColor(const Color &color, double brightness)
{
    this->_color = color;
    this->gpioColor = GPIOColor(color, brightness);
    cout << "color: " << this->_color.red << " " << this->_color.green << " " << this->_color.blue << endl;

    set_PWM_dutycycle(this->pigpio, this->gpioR, this->_color.red);
    set_PWM_dutycycle(this->pigpio, this->gpioG, this->_color.green);
    set_PWM_dutycycle(this->pigpio, this->gpioB, this->_color.blue);
}

void LedStrip::fadeIn()
{

}

void LedStrip::fadeOut()
{

}
