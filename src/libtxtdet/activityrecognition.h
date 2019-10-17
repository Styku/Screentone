#ifndef ACTIVITYRECOGNITION_H
#define ACTIVITYRECOGNITION_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <unordered_map>

#include <opencv2/opencv.hpp>

#include "textfeatures.h"

using MLP=cv::ml::ANN_MLP;

class ActivityRecognition
{
public:
    ActivityRecognition(std::string fpath, size_t history=60);
    bool load(std::string fpath);
    void setCategories(std::vector<std::string> cats) {categories = cats;}
    std::string predict(cv::Mat input);
    std::string predict(NormalizedFeatures input);
    std::vector<float> getNumericalPrediction() { return current_prediction; }

    static cv::Mat prepareInput(NormalizedFeatures nf);

private:
    cv::Ptr<MLP> mlp;
    std::vector<std::deque<float>> prediction_history;
    std::vector<float> current_prediction;
    std::vector<std::string> categories;
    size_t history_size;
    std::vector<float> updatePredictions(std::vector<float> prediction);
};


#endif // ACTIVITYRECOGNITION_H
