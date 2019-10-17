#include "activityrecognition.h"

ActivityRecognition::ActivityRecognition(std::string fpath, size_t history) : history_size{history}
{
    load(fpath);
}

bool ActivityRecognition::load(std::string fpath)
{
    mlp = MLP::load(fpath);

    return !mlp.empty();
}

std::string ActivityRecognition::predict(NormalizedFeatures input)
{
    return predict(prepareInput(input));
}

std::string ActivityRecognition::predict(cv::Mat input)
{
    cv::Mat results;

    mlp->predict(input, results);

    std::vector<float> res;
    for(int i = 0; i < results.cols; ++i)
    {
        res.push_back(results.at<float>(0,i));
    }
    softmax(res.begin(), res.end());

    std::vector ret = updatePredictions(res);
    //TODO: temporary, read categories from file
    size_t idx = static_cast<size_t>(std::distance(current_prediction.begin(),
                                                   std::max_element(current_prediction.begin(), current_prediction.end())));
    return categories.size() > idx ? categories.at(static_cast<size_t>(idx)) : "invalid category";
}

cv::Mat ActivityRecognition::prepareInput(NormalizedFeatures nf)
{
    cv::Mat_<float> ret(1, static_cast<int>(nf.size()));

    std::vector<std::string> feature_names;

    for(const auto& p : nf)
        feature_names.push_back(p.first);

    std::sort(feature_names.begin(), feature_names.end());
    for(size_t i = 0; i < feature_names.size(); ++i)
    {
        ret.at<float>(0, static_cast<int>(i)) = static_cast<float>(nf[feature_names[i]]);
    }

    return std::move(ret);
}

std::vector<float> ActivityRecognition::updatePredictions(std::vector<float> prediction)
{
    std::vector<float> ret;

    if(prediction_history.empty())
    {
        current_prediction.clear();
        for(auto p : prediction)
        {
            prediction_history.push_back(std::deque<float>{});
            current_prediction.push_back(p);
        }
    }

    for(size_t i = 0; i < prediction.size(); ++i)
    {
        prediction_history[i].push_back(prediction[i]);
        auto sz = prediction_history[i].size();
        if(sz > history_size)
        {
            current_prediction[i] = (current_prediction[i] * sz + prediction[i] - prediction_history[i].front())/sz;
            prediction_history[i].pop_front();
        }
        else
        {
            current_prediction[i] = (current_prediction[i] * (sz-1) + prediction[i])/sz;
        }
        ret.push_back(current_prediction[i]);
    }

    return ret;
}
