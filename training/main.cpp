#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>

#include <opencv2/opencv.hpp>

#include <textdetector.h>
#include <textfeatures.h>


namespace fs=std::filesystem;

using namespace std;
using MLP=cv::ml::ANN_MLP;
enum class Mode {Help, Train, Prepare};

std::pair<Mode, cv::CommandLineParser> parseCmdArgs(int argc, char *argv[]);
void prepareDataset(std::string in, std::string out);
cv::Mat readDataset(std::string in, bool contains_header=true, char delim=';');
void train(cv::Mat input);
double parseToken(const std::string& token) noexcept;

void train(cv::Mat input)
{
    std::cout << input << std::endl;
    cv::Mat X = input.colRange(0, 14);
    cv::Mat_<float> Y = input.colRange(14, 17).clone();

    std::cout << "creating mlp\n";
    cv::Ptr<MLP> mlp = MLP::create();
    cv::Mat layers = (cv::Mat_<int>(3, 1) << X.cols, (X.cols + Y.cols)/2, Y.cols);

    std::cout << layers << std::endl;
    mlp->setLayerSizes(layers);
    mlp->setActivationFunction(MLP::ActivationFunctions::SIGMOID_SYM);
    cv::TermCriteria criteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 1e5, 1e-15);
    mlp->setTermCriteria(criteria);
    mlp->setTrainMethod(MLP::TrainingMethods::BACKPROP);

    cv::Ptr<cv::ml::TrainData> data = cv::ml::TrainData::create(X, cv::ml::SampleTypes::ROW_SAMPLE, Y);
    mlp->train(data);

    cv::Mat res;
    mlp->predict(X, res);
    std::cout << res << std::endl;
};

int main(int argc, char *argv[])
{
    std::pair<Mode, cv::CommandLineParser> cmd = parseCmdArgs(argc, argv);
    std::string input, output;
    cv::Mat data;
    switch(cmd.first)
    {
    case Mode::Help:
        cmd.second.printMessage();
        break;
    case Mode::Train:
        input = cmd.second.get<std::string>("@input");
        data = readDataset(input);
        train(data);
        //std::cout << data << endl;
        break;
    case Mode::Prepare:
        input = cmd.second.get<std::string>("@input");
        output = cmd.second.get<std::string>("output");
        std::cout << output << std::endl;
        prepareDataset(input, output);
        break;
    }
    return 0;
}

std::pair<Mode, cv::CommandLineParser> parseCmdArgs(int argc, char *argv[])
{
    const std::string keys =
        "{help h usage ? |         | print this message        }"
        "{p prepare      |         | prepare training dataset  }"
        "{o output       | out.csv | path to output file       }"
        "{@input         |         | input path                }"
        ;
    cv::CommandLineParser cmd(argc, argv, keys);
    cmd.about("Build model for screen activity detection");
    Mode m;
    if(cmd.has("help"))
    {
        m = Mode::Help;
    }
    else if(cmd.has("prepare"))
    {
        m = Mode::Prepare;
    }
    else if(!cmd.get<std::string>("@input").empty())
    {
        m = Mode::Train;
    }
    return {m, cmd};
};

void prepareDataset(std::string in, std::string out)
{
    std::ofstream output(out);
    cv::Mat img;
    TextDetector td = TextDetector().setLetterSpacing(13, 3).setParagraphSpacing(17, 29).setThreshold(210);
    TextFeatures tf;
    NormalizedFeatures nf;
    std::vector<std::string> feature_names, category_names;
    std::unordered_map<std::string, std::vector<double>> category_mapping;
    int n_samples = 0, n_categories = 0;

    for(auto& category : fs::directory_iterator(in))
    {
        n_categories++;
        category_names.push_back(category.path().filename());
        for([[maybe_unused]]const auto& input_file : fs::directory_iterator(category.path()))
            n_samples++;
    }


    for(auto& category : fs::directory_iterator(in))
    {
        std::string category_name = category.path().filename();
        std::vector<double> onehot;
        for(size_t i = 0; i < static_cast<size_t>(n_categories); ++i)
        {
            if(category_mapping.size() == i)
                onehot.push_back(1.0);
            else
                onehot.push_back(0.0);
        }
        category_mapping[category_name] = onehot;

        std::cout << "Parsing category: " << category_name << " ";

        for(const auto& input_file : fs::directory_iterator(category.path()))
        {
            std::string path = input_file.path();
            img = cv::imread(path, cv::IMREAD_COLOR);
            tf = TextFeatures(td.detect(img), img.cols, img.rows);
            tf.discardInnerBoxes();
            nf = tf.normalize();
            if(feature_names.empty())
            {
                for(const auto& p : nf)
                    feature_names.push_back(p.first);

                std::sort(feature_names.begin(), feature_names.end());
                for(auto& col : feature_names)
                    output << col << ";";
                for(auto& col : category_names)
                    output << col << ";";
                output << "\n";
            }

            for(auto& ft : feature_names)
                output << nf[ft] << ";";
            for(auto& label : category_mapping[category_name])
                output << label << ";";
            output << "\n";
            std::cout << "." << std::flush;
        }
        std::cout << std::endl;
    }
};

cv::Mat readDataset(std::string in, bool contains_header, char delim)
{
    std::vector<std::vector<float>> v;
    std::ifstream file(in);
    std::string line, value;
    std::getline(file, line);
    int n_features = static_cast<int>(std::count(line.begin(), line.end(), delim));
    if(line.back() != delim) n_features++;

    while(std::getline(file, line))
    {
        if(contains_header)
        {
            contains_header = false;
            continue;
        }
        std::vector<float> v_row;
        std::istringstream ss(line);
        while(std::getline(ss, value, delim))
        {
            cout << value << "\n";
            float val = static_cast<float>(parseToken(value));
            v_row.push_back(val);
        }
        if(n_features == -1) n_features = static_cast<int>(v_row.size());
        if(n_features != static_cast<int>(v_row.size())) return cv::Mat{};
        v.push_back(v_row);
    }
    int n_samples = static_cast<int>(v.size());
    cv::Mat_<float> ret(n_samples, n_features);

    for(int r = 0; r < n_samples; ++r)
    {
        for(int c = 0; c < n_features; ++c)
        {
            ret[r][c] = v[static_cast<size_t>(r)][static_cast<size_t>(c)];
        }
    }
    return std::move(ret);
};


double parseToken(const std::string& token) noexcept
{
    double value;
    try {
        value = std::stod(token);
    }
    catch(...) {
        value = std::nan("1");
    }
    return value;
}
