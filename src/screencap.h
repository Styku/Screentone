#ifndef SCREENCAP_H
#define SCREENCAP_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <opencv2/opencv.hpp>

using cv::Mat;

//Courtesy of https://stackoverflow.com/questions/24988164/c-fast-screenshots-in-linux-for-use-with-opencv/39781697

class Screencap
{
public:
    Screencap(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        init = true;
    }

    void operator() (Mat& cvImg)
    {
        if(init == true)
            init = false;
        else
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, static_cast<unsigned int>(width), static_cast<unsigned int>(height), AllPlanes, ZPixmap);

        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~Screencap()
    {
        if(init == false)
            XDestroyImage(img);

        XCloseDisplay(display);
    }

    int x, y;
    int width, height;

private:
    Display* display;
    Window root;
    XImage* img;
    bool init;
};

#endif // SCREENCAP_H
