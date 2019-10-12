#include "textdetector.h"

#include <opencv2/opencv.hpp>

#include <vector>

using std::vector;
using cv::Rect;
using cv::Mat;
using cv::Point;

TextDetector::TextDetector() : grouping{TextGrouping::Paragraphs}
{
    initDefaults();
}

vector<Rect> TextDetector::detect(Mat& frame)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::Sobel(frame_gray, frame_sobel, CV_8U, 1, 0);
    cv::threshold(frame_sobel, frame_bin, threshold, 255, cv::THRESH_BINARY);
    cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_CLOSE, element_lines);
    if(grouping == TextGrouping::Paragraphs)
    {
        cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_CLOSE, element_paragraphs1);
        cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_OPEN, element_paragraphs2);
    }

    vector<vector<Point>> contours, poly;
    vector<Rect> text_boxes;
    findContours(frame_bin, contours, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE, cv::CONTOURS_MATCH_I1);

    for(auto& c : contours)
    {
        vector<Point> poly;
        cv::approxPolyDP( cv::Mat(c), poly, 3, true );
        Rect bbox = boundingRect(Mat(poly));
        //TODO: expand boxes by a value based on morph kernel (faster than MORPH_DILUTE)
        cv::rectangle(frame, bbox, cv::Scalar(0, 255, 0));
        text_boxes.push_back(bbox);
    }
    return text_boxes;
}

TextDetector& TextDetector::setLetterSpacing(int w, int h)
{
    element_lines = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(w, h) );
    return *this;
}

TextDetector& TextDetector::setParagraphSpacing(int max_line_h, int min_line_w)
{
    element_paragraphs1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, max_line_h) );
    element_paragraphs2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(min_line_w, 1) );
    return *this;
}

TextDetector &TextDetector::setGrouping(TextGrouping grouping)
{
    this->grouping = grouping;
    return *this;
}

TextDetector &TextDetector::setThreshold(int threshold)
{
    this->threshold = threshold;
    return *this;
}

void TextDetector::initDefaults()
{
    setLetterSpacing(13, 3);
    setParagraphSpacing(17, 25);
}
