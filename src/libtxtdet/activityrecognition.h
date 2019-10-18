#ifndef ACTIVITYRECOGNITION_H
#define ACTIVITYRECOGNITION_H

#include <string>
#include <vector>
#include <algorithm>
#include <deque>

#include <opencv2/opencv.hpp>

#include "textfeatures.h"

namespace ar {

    using Prediction = std::pair<unsigned, std::string>;

    cv::Mat prepareInput(const NormalizedFeatures &nf);

    class ActivityRecognition
    {
    public:
        ActivityRecognition(std::string file_path);
        ActivityRecognition() = default;

        cv::Mat load(std::string& file_path);
        void setCategories(const std::vector<std::string>& category_names) { this->category_names = category_names; }
        const std::vector<std::string>& getCategories() const { return category_names; }
        const std::vector<float>& getPredictionProbability() const { return prediction_current; }

        Prediction predict(cv::Mat input);
        Prediction predict(NormalizedFeatures input);
        Prediction getCurrentPrediction();

    private:
        cv::Ptr<cv::ml::ANN_MLP> mlp;
        std::vector<std::deque<float>> prediction_history;
        std::vector<float> prediction_current;
        std::vector<std::string> category_names;
        size_t history_size = 60;
        void updatePredictions(const std::vector<float> &prediction);
    };

}

#endif // ACTIVITYRECOGNITION_H
