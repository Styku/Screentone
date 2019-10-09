#include "textfeatures.h"

TextFeatures::TextFeatures() : initialized{false}
{

}

TextFeatures::TextFeatures(std::vector<cv::Rect> text_boxes) : text_boxes{text_boxes}
{
    initialized = extractFeatures();
}

bool TextFeatures::extractFeatures()
{
    bool ret = true;
    double area = .0, max_area = .0, total_area = .0;
    //TODO: parse for encompasing/overlapping boxes
    //TODO: add more features (area median and variance, vertical and horizontal placement, etc.)
    if(!text_boxes.empty())
    {
        for(auto& box : text_boxes)
        {
            area = box.width * box.height;
            max_area = std::max(area, max_area);
            total_area += area;
        }
        screen_coverage = total_area;
        largest_box = max_area;
    }
    else
    {
        ret = false;
    }
    return ret;
}

NormalizedFeatures TextFeatures::normalize(double screen_w, double screen_h) const
{
    NormalizedFeatures ret;
    ret["coverage"] = screen_coverage/(screen_w * screen_h);
    ret["max_area"] = largest_box/(screen_w * screen_h);
    return ret;
}
