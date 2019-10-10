#include "screenparams.h"

ScreenParams::ScreenParams(std::string display_id) : display_id{display_id}, gamma{1.0, 1.0, 1.0}
{

}

ScreenParams::Gamma ScreenParams::set(unsigned int temp, unsigned int transition)
{
    std::lock_guard<std::mutex> lock(ScreenParams::execution);
    char cmd[100];
    unsigned int steps = 30;
    Gamma new_gamma = tempToGamma(temp);
    unsigned int delay_per_step = transition/steps;
    if(gamma != new_gamma)
    {
        Gamma step = computeStep(new_gamma, steps);
        for(unsigned int i = 0; i < steps; ++i)
        {
            gamma += step;
            sprintf(cmd, "xrandr --output %s --gamma %f:%f:%f", display_id.c_str(), gamma.r, gamma.g, gamma.b);
            system(cmd);
            cout << cmd << endl;
            cout << "[Thread " << std::this_thread::get_id() << "] Waiting for " << delay_per_step << "ms" << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_step));
        }
    }
    return gamma;
}

// Temperature to RGB conversion algorithm by Tanner Helland
// http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
ScreenParams::Gamma ScreenParams::tempToGamma(unsigned int temp)
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
