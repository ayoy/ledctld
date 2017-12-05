#ifndef LEDSTRIP_HPP
#define LEDSTRIP_HPP

struct Color
{
    unsigned red;
    unsigned green;
    unsigned blue;
    Color(unsigned red, unsigned green, unsigned blue);

    bool operator==(const Color &rhs) const;

    static const Color & black();
};

struct GPIOColor
{
private:
    unsigned _red;
    unsigned _green;
    unsigned _blue;
    double _brightness;
    Color _color;

    void applyBrightness();

public:

    GPIOColor(const Color &color, double brightness = 1.0);

    inline Color color() const { return this->_color; };
    inline unsigned red() const { return this->_red; };
    inline unsigned green() const { return this->_green; };
    inline unsigned blue() const { return this->_blue; };
    inline unsigned brightness() const { return this->_brightness; };

    void setBrightness(double brightness);

    static const GPIOColor & black();
};

class LedStrip
{
    int pigpio;
    unsigned gpioR;
    unsigned gpioG;
    unsigned gpioB;
    Color _color {Color::black()};
    GPIOColor gpioColor {GPIOColor(Color::black())};

    bool enabled {false};

    double brightness {0};

    void applyCurrentColor() const;
    void updatePins(const GPIOColor &color) const;

public:
    LedStrip(int pigpio, unsigned gpioR, unsigned gpioG, unsigned gpioB): 
        pigpio(pigpio), gpioR(gpioR), gpioG(gpioG), gpioB(gpioB) 
        {}

    inline Color color() const { return this->_color; }
    void setColor(const Color &color, double brightness = 1.0);

    inline Color currentColor() const { return this->gpioColor.color(); }

    inline void turnOff() { this->setColor(Color::black()); }

    void setEnabled(bool enabled);
    inline bool isEnabled() const { return this->enabled; }

    void fadeIn();
    void fadeOut();
};


#endif
