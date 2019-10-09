#ifndef TEXTFEATURES_H
#define TEXTFEATURES_H

#include <opencv2/opencv.hpp>

#include <vector>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, double> NormalizedFeatures;

class TextFeatures
{
public:
    TextFeatures();
    TextFeatures(std::vector<cv::Rect> text_boxes);
    bool extractFeatures();
    NormalizedFeatures normalize(double screen_w, double screen_h) const;

private:
    bool initialized;
    std::vector<cv::Rect> text_boxes;
    cv::Size screen_size;

    // Features
    double screen_coverage;
    double largest_box;
};

#endif // TEXTFEATURES_H
