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
    TextDetector txtd = TextDetector{}.setLetterSpacing(17, 3).setParagraphSpacing(27, 31).setGrouping(TextGrouping::Paragraphs).setThreshold(220);
    TextFeatures textfeat;
    String window = "Debug";
    namedWindow(window, WINDOW_NORMAL);
    std::future<int> currentTemp;
    std::vector<cv::Rect> boxes, boxes2;

    int mode = 0;

    Mat frame, gray;
    while(true)
    {
        screencap(frame);
        textfeat = TextFeatures(txtd.detect(frame), 1920, 1080);
        NormalizedFeatures X = textfeat.normalize();
        for(auto const& p : X)
        {
            std::cout << "(" + p.first + ", " << p.second << ") ";
        }
        std::cout << "\n";
        for(auto& box : boxes2)
        {
            cv::rectangle(frame, box, Scalar(0, 0, 255));
        }

        // Just for testing
        if(X["coverage"] > 0.3 && mode == 0)
        {
            cout << "Lowering gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentTemp = std::async(std::launch::async, &ScreenParams::setTemperature, &scr, 2800, 1000);
            mode = 1;
        }
        else if(X["coverage"] <= 0.3 && mode == 1)
        {
            cout << "Increasing gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentTemp = std::async(std::launch::async, &ScreenParams::setTemperature, &scr, 6600, 1000);
            mode = 0;
        }

        imshow(window, frame);

        if (waitKey(10) == 27)
            break;
    }
    return 0;
}
