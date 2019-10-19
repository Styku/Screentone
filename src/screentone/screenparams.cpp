#include "screenparams.h"

int ScreenParams::setTemperature(int temp, uint transition)
{
    constexpr int STEPS = 30;
    const std::chrono::milliseconds DELAY(transition/STEPS);
    const int INCR = (temp - screen_temperature)/STEPS;
    target_temperature = temp;

    for (uint i = 0; i < STEPS; ++i)
    {
        if(temp != target_temperature) return screen_temperature;
        screen_temperature += INCR;
        xrandr();
        std::this_thread::sleep_for(DELAY);
    }

    if(temp == target_temperature)
    {
        screen_temperature = temp;
        xrandr();
    }

    return screen_temperature;
}

// Temperature to RGB conversion algorithm by Tanner Helland
// http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
Gamma ScreenParams::tempToGamma(int temp)
{
    double t = temp/100;
    double r, g, b;

    if(t <= 66)
    {
        r = 255.0;
        g = t;
        g = 99.4708025861 * log(g) - 161.1195681661;
    }
    else
    {
        r = t - 60;
        r = 329.698727446 * pow(r, -0.1332047592);
        g = t - 60;
        g = 288.1221695283 * pow(g, -0.0755148492);
    }

    if(t >= 66)
    {
        b = 255.0;
    }
    else
    {
        if(t <= 19)
        {
            b = 0.0;
        }
        else
        {
            b = t - 10;
            b = 138.5177312231 * log(b) - 305.0447927307;
        }
    }

    r = min(max(r, 0.0), 255.0)/255.0;
    g = min(max(g, 0.0), 255.0)/255.0;
    b = min(max(b, 0.0), 255.0)/255.0;
    return Gamma(r, g, b);
}
