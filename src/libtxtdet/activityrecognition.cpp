#include "activityrecognition.h"

using MLP=cv::ml::ANN_MLP;

namespace ar
{
    cv::Mat prepareInput(const NormalizedFeatures& nf)
    {
        cv::Mat_<float> ret(1, static_cast<int>(nf.size()));

        std::vector<std::string> feature_names;
        for(const auto& p : nf)
            feature_names.push_back(p.first);

        std::sort(feature_names.begin(), feature_names.end());
        for(size_t i = 0; i < feature_names.size(); ++i)
        {
            ret.at<float>(0, static_cast<int>(i)) = static_cast<float>(nf.at(feature_names[i]));
        }

        return std::move(ret);
    }

    ActivityRecognition::ActivityRecognition(std::string file_path)
    {
        load(file_path);
    }

    cv::Mat ActivityRecognition::load(std::string& file_path)
    {
        mlp = MLP::load(file_path);
        return mlp->getLayerSizes();
    }

    Prediction ActivityRecognition::predict(NormalizedFeatures input)
    {
        return predict(prepareInput(input));
    }

    Prediction ActivityRecognition::getCurrentPrediction()
    {
        size_t idx = static_cast<size_t>(std::distance(prediction_current.begin(),
                                                       std::max_element(prediction_current.begin(), prediction_current.end())));
        std::string cat_name = category_names.size() > idx ? category_names.at(idx) : "";
        return {idx, cat_name};
    }

    Prediction ActivityRecognition::predict(cv::Mat input)
    {
        cv::Mat results;

        mlp->predict(input, results);

        std::vector<float> res;
        for(auto it = results.begin<float>(); it != results.end<float>(); ++it)
            res.push_back(*it);

        softmax(res.begin(), res.end());
        updatePredictions(res);

        return getCurrentPrediction();
    }

    void ActivityRecognition::updatePredictions(const std::vector<float>& prediction)
    {
        if(prediction_history.empty())
        {
            prediction_current.clear();
            for(auto p : prediction)
            {
                prediction_history.push_back(std::deque<float>{});
                prediction_current.push_back(p);
            }
        }

        for(size_t i = 0; i < prediction.size(); ++i)
        {
            prediction_history[i].push_back(prediction[i]);
            size_t sz = prediction_history[i].size();
            if(sz > history_size)
            {
                prediction_current[i] = (prediction_current[i] * sz + prediction[i] - prediction_history[i].front())/sz;
                prediction_history[i].pop_front();
            }
            else
            {
                prediction_current[i] = (prediction_current[i] * (sz-1) + prediction[i])/sz;
            }
        }
    }
}
