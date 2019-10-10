#include <iostream>
#include <string>
#include <vector>
#include <future>

#include <opencv2/opencv.hpp>
#include "screencap.h"
#include "textdetector.h"
#include "textfeatures.h"
#include "screenparams.h"

using namespace std;
using namespace cv;

int main()
{
    ScreenParams scr{"DP-1-1"};
    Screencap screencap(0, 0, 1920, 1080);
    TextDetector txtd = TextDetector{}.setLetterSpacing(17, 3).setParagraphSpacing(27, 25).setGrouping(TextGrouping::Lines);
    TextFeatures textfeat;
    String window = "Debug";
    namedWindow(window, WINDOW_NORMAL);
    std::shared_future<ScreenParams::Gamma> currentGamma;

    int mode = 0;

    Mat frame, gray;
    while(true)
    {
        screencap(frame);
        textfeat = txtd.detect(frame);
        NormalizedFeatures X = textfeat.normalize(1920, 1080);

        // Just for testing
        if(X["coverage"] > 0.3 && mode == 0)
        {
            cout << "Lowering gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentGamma = std::async(std::launch::async, &ScreenParams::set, &scr, 3200, 10000);
            //scr.set(3200, 1000);
            mode = 1;
        }
        else if(X["coverage"] <= 0.3 && mode == 1)
        {
            cout << "Increasing gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentGamma = std::async(std::launch::async, &ScreenParams::set, &scr, 6600, 10000);
            //scr.set(6600, 1000);
            mode = 0;
        }

        imshow(window, frame);

        if (waitKey(10) == 27)
            break;
    }
    return 0;
}
