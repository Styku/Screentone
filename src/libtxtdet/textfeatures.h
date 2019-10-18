#ifndef TEXTFEATURES_H
#define TEXTFEATURES_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include <opencv2/opencv.hpp>

#include "helpers.h"

namespace ar
{
    using NormalizedFeatures = std::unordered_map<std::string, double>;
    using Pixel = unsigned;

    class TextFeatures
    {
    public:
        TextFeatures(Pixel screen_w, Pixel screen_h) : screen_w{screen_w}, screen_h{screen_h} {}
        TextFeatures(const std::vector<cv::Rect> &text_boxes, Pixel screen_w, Pixel screen_h);
        TextFeatures(std::vector<cv::Rect> &&text_boxes, Pixel screen_w, Pixel screen_h) noexcept;
        TextFeatures() = default;

        bool extractFeatures(const std::vector<cv::Rect> &text_boxes) noexcept;
        bool extractFeatures(std::vector<cv::Rect> &&text_boxes) noexcept;
        NormalizedFeatures normalize() const;
        std::vector<cv::Rect> discardInnerBoxes(std::vector<cv::Rect> &text_boxes, float overlap_threshold=0.7f);

    private:
        Pixel screen_w = 1920;
        Pixel screen_h = 1080;

        // Features
        double screen_coverage{}, largest_box{}, variance{}, median_width{}, median_area{};
        std::vector<int> placement_hist{};


    };
}

#endif // TEXTFEATURES_H
