#include "textdetector.h"

using std::vector;
using cv::Mat;
using cv::Point;

namespace ar
{
    TextDetector::TextDetector()
    {
        setLetterSpacing(13, 3);
        setParagraphSpacing(17, 25);
    }

    std::vector<BoundingBox> TextDetector::detect(Mat frame) const
    {
        Mat frame_gray, frame_sobel, frame_bin;
        cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::Sobel(frame_gray, frame_sobel, CV_8U, 1, 0);
        cv::threshold(frame_sobel, frame_bin, binarization_threshold, 255, cv::THRESH_BINARY);
        cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_CLOSE, element_lines);
        if(grouping == TextGroupingType::Paragraphs)
        {
            cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_CLOSE, element_paragraphs1);
            cv::morphologyEx(frame_bin, frame_bin, cv::MORPH_OPEN, element_paragraphs2);
        }

        vector<vector<Point>> contours, poly;
        vector<BoundingBox> text_boxes;
        findContours(frame_bin, contours, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE, cv::CONTOURS_MATCH_I1);

        for(auto& c : contours)
        {
            vector<Point> poly;
            cv::approxPolyDP( cv::Mat(c), poly, 3, true );
            BoundingBox bbox = boundingRect(Mat(poly));
            //TODO: expand boxes by a value based on morph kernel (faster than MORPH_DILUTE)
            //TODO: add debug flag, do not modify frame by default
            cv::rectangle(frame, bbox, cv::Scalar(100, 0, 255));
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

    TextDetector &TextDetector::setGrouping(TextGroupingType grouping)
    {
        this->grouping = grouping;
        return *this;
    }

    TextDetector &TextDetector::setThreshold(int threshold)
    {
        this->binarization_threshold = threshold;
        return *this;
    }
}
