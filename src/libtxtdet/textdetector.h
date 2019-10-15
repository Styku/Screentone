#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H

#include <opencv2/opencv.hpp>

#include <vector>

enum class TextGrouping { Lines, Paragraphs };


class TextDetector
{
public:
    TextDetector();
    std::vector<cv::Rect> detect(cv::Mat &frame);
    TextDetector& setLetterSpacing(int w, int h);
    TextDetector& setParagraphSpacing(int max_line_h, int min_line_w);
    TextDetector& setGrouping(TextGrouping grouping);
    TextDetector& setThreshold(int threshold);

    cv::Mat frame_bin, frame_sobel;

private:
    bool groupLines;
    int threshold;
    cv::Mat element_lines, element_paragraphs1, element_paragraphs2;
    TextGrouping grouping;
    void initDefaults();

};

#endif // TEXTDETECTOR_H
