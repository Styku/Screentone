#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H

#include <vector>

#include <opencv2/opencv.hpp>

namespace ar
{
    enum class TextGroupingType { Lines, Paragraphs };
    using BoundingBox = cv::Rect;

    class TextDetector
    {
    public:
        virtual ~TextDetector() = default;
        virtual std::vector<BoundingBox> detect(cv::Mat frame) const = 0;
    };

    class SimpleTextDetector : public TextDetector
    {
    public:
        SimpleTextDetector();
        SimpleTextDetector(int max_line_h, int min_line_w) : SimpleTextDetector{} { setParagraphSpacing(max_line_h, min_line_w); }

        std::vector<BoundingBox> detect(cv::Mat frame) const override;

        std::unique_ptr<TextDetector> create();

        SimpleTextDetector& setLetterSpacing(int w, int h);
        SimpleTextDetector& setParagraphSpacing(int max_line_h, int min_line_w);
        SimpleTextDetector& setGrouping(TextGroupingType grouping);
        SimpleTextDetector& setThreshold(int binarization_threshold);

    private:
        cv::Mat element_lines, element_paragraphs1, element_paragraphs2;
        int binarization_threshold = 200;
        TextGroupingType grouping = TextGroupingType::Paragraphs;
    };
}
#endif // TEXTDETECTOR_H
