#ifndef ACTIVITYRECOGNITION_H
#define ACTIVITYRECOGNITION_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "textfeatures.h"

using MLP=cv::ml::ANN_MLP;

class ActivityRecognition
{
public:
    ActivityRecognition();
    bool load(std::string fpath);
    std::string predict(cv::Mat input);
    std::string predict(NormalizedFeatures input);

    static cv::Mat prepareInput(NormalizedFeatures nf);

private:
    cv::Ptr<MLP> mlp;
};


#endif // ACTIVITYRECOGNITION_H
