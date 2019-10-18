#include "textfeatures.h"

namespace ar
{
    TextFeatures::TextFeatures(const std::vector<cv::Rect> &text_boxes, Pixel screen_w, Pixel screen_h) : TextFeatures{screen_w, screen_h}
    {
        extractFeatures(text_boxes);
    }

    TextFeatures::TextFeatures(std::vector<cv::Rect> &&text_boxes, Pixel screen_w, Pixel screen_h) noexcept : TextFeatures{screen_w, screen_h}
    {
        extractFeatures(text_boxes);
    }

    bool TextFeatures::extractFeatures(const std::vector<cv::Rect> &text_boxes) noexcept
    {
        return extractFeatures(std::vector<cv::Rect>{text_boxes});
    }

    bool TextFeatures::extractFeatures(std::vector<cv::Rect> &&text_boxes) noexcept
    {
        size_t bins = 10;
        double area = .0, max_area = .0, total_area = .0;
        placement_hist = std::vector<int>(bins, 0);
        const unsigned hist_step = screen_w/bins;
        std::vector<double> areas, widths;
        if(!text_boxes.empty())
        {
            for(auto& box : discardInnerBoxes(text_boxes))
            {
                area = box.width * box.height;
                areas.push_back(area);
                widths.push_back(box.width);
                max_area = std::max(area, max_area);
                total_area += area;
                //TODO: weigh placement histogram by area
                placement_hist[static_cast<unsigned>(box.x)/hist_step]++;

            }
            screen_coverage = total_area;
            largest_box = max_area;
            median_area = median(areas);
            median_width = median(widths);
            return true;
        }
        return false;
    }

    NormalizedFeatures TextFeatures::normalize() const
    {
        NormalizedFeatures ret;
        double screen_area = (screen_w * screen_h);
        ret["coverage"] = screen_coverage/screen_area;
        ret["max_area"] = largest_box/screen_area;
        for(size_t i = 0; i < placement_hist.size(); ++i)
            ret["placement_" + std::to_string(i)] = placement_hist[i];
        ret["median_area"] = median_area/screen_area;
        ret["median_width"] = median_width/screen_w;

        return ret;
    }

    std::vector<cv::Rect> TextFeatures::discardInnerBoxes(std::vector<cv::Rect>& text_boxes, float overlap_threshold)
    {
        //TODO: optimize
        std::unordered_map<size_t, bool> to_discard;
        std::sort(text_boxes.begin(), text_boxes.end(), [](cv::Rect a, cv::Rect b) { return a.x < b.x; });
        cv::Rect& a = text_boxes.at(0), b = text_boxes.at(0);
        float a_area, b_area, overlap_area, a_y2, a_x2, b_y2;

        for(size_t i = 0; i < text_boxes.size(); ++i)
        {
            a = text_boxes.at(i);
            a_area = a.width * a.height;
            a_y2 = a.y + a.height;
            a_x2 = a.x + a.width;
            for(size_t j = i + 1; j < text_boxes.size(); ++j)
            {
                b = text_boxes.at(j);
                b_y2 = b.y + b.height;
                if(b.x >= a_x2)
                    break;
                else if(b_y2 <= a.y || b.y >= a_y2)
                    continue;
                else // b corner within a, compute overlap
                {
                    b_area = b.width * b.height;
                    if(a.y < b.y)
                        overlap_area = (a_x2 - b.x) * (a_y2 - b.y);
                    else
                        overlap_area = (a_x2 - b.x) * (a.y - b_y2);
                    if(a_area > b_area)
                    {
                        if(overlap_area > b_area * overlap_threshold)
                            to_discard[j] = true;
                    }
                    else if(overlap_area > a_area * overlap_threshold)
                    {
                        to_discard[j] = true;
                        break;
                    }
                }
            }
        }
        std::vector<cv::Rect> filtered;
        for(size_t i = 0; i < text_boxes.size(); ++i)
        {
            if(to_discard.find(i) == to_discard.end())
                filtered.push_back(text_boxes.at(i));
        }
        text_boxes = filtered;
        return filtered;
    }
}
