#ifndef TEXTFEATURES_H
#define TEXTFEATURES_H

#include <opencv2/opencv.hpp>

#include <vector>
#include <string>
#include <unordered_map>

#include "helpers.h"

typedef std::unordered_map<std::string, double> NormalizedFeatures;

class TextFeatures
{
public:
    TextFeatures();
    TextFeatures(std::vector<cv::Rect> text_boxes, double screen_w, double screen_h);
    bool extractFeatures();
    NormalizedFeatures normalize() const;

private:
    bool initialized;
    std::vector<cv::Rect> text_boxes;
    cv::Size screen_size;
    double screen_w, screen_h;

    // Features
    double screen_coverage, largest_box, variance, median_width, median_area;
    std::vector<int> placement_hist;


};

#endif // TEXTFEATURES_H
