#include "ledstrip.hpp"
#include <pigpiod_if2.h>
#include <iostream>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>

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
    if (abs(brightness - this->_brightness) > std::numeric_limits<double>::epsilon()) {
        this->_brightness = brightness;
        this->applyBrightness();
    }
}

void GPIOColor::applyBrightness() 
{
    double redLuminance = this->_color.red * Y_RED;
    double greenLuminance = this->_color.green * Y_GREEN;
    double blueLuminance = this->_color.blue * Y_BLUE;

    this->_red = int(this->_brightness * redLuminance / Y_RED);
    this->_green = int(this->_brightness * greenLuminance / Y_GREEN);
    this->_blue = int(this->_brightness * blueLuminance / Y_BLUE);
    // printf("GPIOColor r: %d g: %d b: %d, br: %.2f\n", this->_red, this->_green, this->_blue, this->_brightness);
}

//

void LedStrip::setColor(const Color &color, double brightness)
{
    this->_color = color;
    this->gpioColor = GPIOColor(color, brightness);
    this->updatePins();
    cout << "color: " << this->_color.red << " " << this->_color.green << " " << this->_color.blue << endl;
}

void LedStrip::fadeIn()
{
    double brightness = 0;
    auto step = 0.05;
    while (brightness <= 1.0) {
        this->gpioColor.setBrightness(brightness);
        this->updatePins();

        brightness += step;
        printf("fadeIn r: %d g: %d b: %d, br: %.2f\n", this->gpioColor.red(), this->gpioColor.green(), this->gpioColor.blue(), brightness);
        this_thread::sleep_for(25ms);
    }
    this->gpioColor.setBrightness(1.0);
    this->updatePins();
    printf("fadeIn r: %d g: %d b: %d, br: %.2f\n", this->gpioColor.red(), this->gpioColor.green(), this->gpioColor.blue(), brightness);
}

void LedStrip::fadeOut()
{
    double brightness = 1.0;
    auto step = 0.05;
    while (brightness >= 0.0) {
        this->gpioColor.setBrightness(brightness);
        this->updatePins();

        brightness -= step;
        printf("fadeOut r: %d g: %d b: %d, br: %.2f\n", this->gpioColor.red(), this->gpioColor.green(), this->gpioColor.blue(), brightness);
        this_thread::sleep_for(25ms);
    }
    this->gpioColor.setBrightness(0.0);
    this->updatePins();
    printf("fadeOut r: %d g: %d b: %d, br: %.2f\n", this->gpioColor.red(), this->gpioColor.green(), this->gpioColor.blue(), brightness);
}

void LedStrip::updatePins() const
{
    set_PWM_dutycycle(this->pigpio, this->gpioR, this->gpioColor.red());
    set_PWM_dutycycle(this->pigpio, this->gpioG, this->gpioColor.green());
    set_PWM_dutycycle(this->pigpio, this->gpioB, this->gpioColor.blue());    
}
