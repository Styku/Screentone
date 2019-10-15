#include "activityrecognition.h"

ActivityRecognition::ActivityRecognition()
{
}

bool ActivityRecognition::load(std::string fpath)
{
    std::cout << "Loading model... ";

    mlp = MLP::load(fpath);
    std::cout << !mlp.empty() << "\n";

    return !mlp.empty();
}

std::string ActivityRecognition::predict(NormalizedFeatures input)
{
    std::cout << "Starting prediction\n";
    return predict(prepareInput(input));
}

std::string ActivityRecognition::predict(cv::Mat input)
{
    cv::Mat results;

    mlp->predict(input, results);

    float max = .0;
    int idx = 0;

    for(int i = 0; i < results.cols; ++i)
    {
        if(results.at<float>(0, i) > max)
        {
            idx = i;
            max = results.at<float>(0, i);
        }
    }
    //TODO: temporary, read categories from file
    std::vector<std::string> categories{"reading", "social-media", "coding"};
    return categories.at(static_cast<size_t>(idx));
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
