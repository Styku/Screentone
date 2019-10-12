#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>

#include <opencv2/opencv.hpp>

#include <textdetector.h>
#include <textfeatures.h>

namespace fs=std::filesystem;

using namespace std;

enum class Mode {Help, Train, Prepare};

std::pair<Mode, cv::CommandLineParser> parseCmdArgs(int argc, char *argv[]);
void prepareDataset(std::string in, std::string out);

int main(int argc, char *argv[])
{
    std::pair<Mode, cv::CommandLineParser> cmd = parseCmdArgs(argc, argv);

    switch(cmd.first)
    {
    case Mode::Help:
        cmd.second.printMessage();
        break;
    case Mode::Train:
        break;
    case Mode::Prepare:
        std::string input, output;
        input = cmd.second.get<std::string>("@input");
        output = cmd.second.get<std::string>("output");
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
    std::vector<std::string> feature_names;
    for(auto& category : fs::directory_iterator(in))
    {
        std::string category_name = category.path().filename();

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
                std::for_each(feature_names.begin(), feature_names.end(),
                              [&output](std::string& s)
                                {
                                     output << s << ";";
                                });
                output << "category\n";
            }

            std::for_each(feature_names.begin(), feature_names.end(),
                          [&output, &nf](std::string& s)
                            {
                                 output << nf[s] << ";";
                            });
            output << category_name << "\n";
        }
    }
};
