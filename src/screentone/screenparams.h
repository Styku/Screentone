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
#include <opencv2/opencv.hpp>

using namespace std;

class ScreenParams
{
public:
    ScreenParams(std::string display_id="eDP-1-1");

    int setTemperature(int temp, unsigned int transition = 1000);

    static cv::Vec3d tempToGamma(int temp);


private:
    std::string display_id;
    std::atomic<int> screen_temperature;
    std::atomic<int> target_temperature;

    void xrandr()
    {
        char cmd[100];
        cv::Vec3d gamma = tempToGamma(screen_temperature);
        sprintf(cmd, "xrandr --output %s --gamma %f:%f:%f", display_id.c_str(), gamma[0], gamma[1], gamma[2]);
        system(cmd);
    }
};

#endif // SCREEN_H
