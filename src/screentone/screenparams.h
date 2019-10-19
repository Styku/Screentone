#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>
#include <atomic>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using Gamma = cv::Vec3d;
class ScreenParams
{

public:
    ScreenParams() = default;
    ScreenParams(std::string display) : display_id{display} {}
    ~ScreenParams() { xrandr(6600); }

    int setTemperature(int temp, unsigned int transition = 1000);

    static Gamma tempToGamma(int temp);


private:
    std::string display_id = "eDP-1-1";
    std::atomic<int> screen_temperature {6600};
    std::atomic<int> target_temperature {6600};

    void xrandr()
    {
        xrandr(screen_temperature);
    }

    void xrandr(int temp)
    {
        stringstream ss;
        Gamma gamma = tempToGamma(temp);
        ss << "xrandr --output " << display_id << " --gamma " << gamma[0] << ":" << gamma[1] << ":" << gamma[2];
        system(ss.str().c_str());
    }
};

#endif // SCREEN_H
