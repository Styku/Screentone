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
    size_t discardInnerBoxes(float overlap_threshold=0.7f);

    std::vector<cv::Rect> getTextBoxes() const { return text_boxes; }

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
