#include "textfeatures.h"

TextFeatures::TextFeatures() : initialized{false}
{

}

TextFeatures::TextFeatures(std::vector<cv::Rect> text_boxes, double screen_w, double screen_h) : text_boxes{text_boxes}, screen_w{screen_w}, screen_h{screen_h}
{
    initialized = extractFeatures();
}

bool TextFeatures::extractFeatures()
{
    bool ret = true;
    size_t bins = 10;
    double area = .0, max_area = .0, total_area = .0;
    placement_hist = std::vector<int>(bins, 0);
    const int hist_step = static_cast<int>(screen_w/bins);
    //TODO: parse for encompasing/overlapping boxes
    //TODO: add more features (area median and variance, vertical and horizontal placement, etc.)
    std::vector<double> areas, widths;
    if(!text_boxes.empty())
    {
        for(auto& box : text_boxes)
        {
            area = box.width * box.height;
            areas.push_back(area);
            widths.push_back(box.width);
            max_area = std::max(area, max_area);
            total_area += area;
            //TODO: weigh placement histogram by area
            placement_hist.at(static_cast<size_t>(box.x/hist_step))++;

        }
        screen_coverage = total_area;
        largest_box = max_area;

        median_area = median(areas);

        //TODO: check WTF happens here
        median_width = median(widths);
    }
    else
    {
        ret = false;
    }
    return ret;
}

NormalizedFeatures TextFeatures::normalize() const
{
    NormalizedFeatures ret;
    double screen_area = (screen_w * screen_h);
    ret["coverage"] = screen_coverage/screen_area;
    ret["max_area"] = largest_box/screen_area;
    for(size_t i = 0; i < placement_hist.size(); ++i)
        ret["placement_" + std::to_string(i)] = placement_hist[i];
//    ret["median_area"] = median_area/screen_area;
//    ret["median_width"] = median_width/screen_w;

    return ret;
}
