#ifndef LEDSTRIP_HPP
#define LEDSTRIP_HPP

struct Color
{
    unsigned red;
    unsigned green;
    unsigned blue;

    Color(unsigned red, unsigned green, unsigned blue);

    static const Color & black();
};

class LedStrip
{
    int pigpio;
    unsigned gpioR;
    unsigned gpioG;
    unsigned gpioB;
    Color _color {Color::black()};

public:
    LedStrip(int pigpio, unsigned gpioR, unsigned gpioG, unsigned gpioB): 
        pigpio(pigpio), gpioR(gpioR), gpioG(gpioG), gpioB(gpioB) 
        {}

    inline Color color() const { return this->_color; }
    void setColor(const Color &color);

    inline void turnOff() { this->setColor(Color::black()); }

    void fadeIn();
    void fadeOut();
};


#endif
