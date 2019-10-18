#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H

#include <vector>

#include <opencv2/opencv.hpp>

namespace ar
{
    enum class TextGroupingType { Lines, Paragraphs };

    class TextDetector
    {
    public:
        TextDetector();
        std::vector<cv::Rect> detect(cv::Mat frame) const;

        TextDetector& setLetterSpacing(int w, int h);
        TextDetector& setParagraphSpacing(int max_line_h, int min_line_w);
        TextDetector& setGrouping(TextGroupingType grouping);
        TextDetector& setThreshold(int binarization_threshold);

    private:
        int binarization_threshold = 200;
        cv::Mat element_lines, element_paragraphs1, element_paragraphs2;
        TextGroupingType grouping = TextGroupingType::Paragraphs;
    };
}
#endif // TEXTDETECTOR_H
