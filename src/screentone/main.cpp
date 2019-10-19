#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <memory>

#include <textdetector.h>
#include <textfeatures.h>
#include <activityrecognition.h>

#include <opencv2/opencv.hpp>

#include "screencap.h"
#include "screenparams.h"

using namespace std;
using namespace cv;

int main()
{
    ScreenParams scr{"eDP-1-1"};
    ar::ActivityRecognition activity("model.yaml");
    activity.setCategories(std::vector<std::string>{"reading", "social-media", "coding"});

    Screencap screencap(0, 0, 1920, 1080);
    unique_ptr<ar::TextDetector> txtd = ar::SimpleTextDetector().setParagraphSpacing(27, 31).create();
    ar::TextFeatures textfeat{1920, 1080};
    String window = "Debug";
    //namedWindow(window, WINDOW_NORMAL);
    std::future<int> currentTemp;
    std::vector<cv::Rect> boxes;

    int mode = 0;

    Mat frame, gray;
    while(true)
    {
        screencap(frame);
        textfeat.extractFeatures(txtd->detect(frame));
        ar::NormalizedFeatures X = textfeat.normalize();

        ar::Prediction ac = activity.predict(X);

        if((ac.second == "coding" || ac.second == "reading") && mode == 0)
        {
            cout << "Lowering gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentTemp = std::async(std::launch::async, &ScreenParams::setTemperature, &scr, 2800, 1000);
            mode = 1;
        }
        else if(ac.second == "social-media" && mode == 1)
        {
            cout << "Increasing gamma, coverage: " << X["coverage"] << ", max. area: " << X["max_area"] << endl;
            currentTemp = std::async(std::launch::async, &ScreenParams::setTemperature, &scr, 6600, 1000);
            mode = 0;
        }

        cv::putText(frame, ac.second, cv::Point(500, 40), cv::FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 200, 50), 2);

        imshow(window, frame);

        if (waitKey(10) == 27)
            break;
    }
    return 0;
}
