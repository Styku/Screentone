#include <QtTest>

#include <opencv2/opencv.hpp>
#include <vector>

#include "textfeatures.h"

using cv::Rect;
using std::vector;

class FeatureExtraction : public QObject
{
    Q_OBJECT

public:
    FeatureExtraction();
    ~FeatureExtraction();

private:
    ar::TextFeatures tf;
    ar::NormalizedFeatures nf;
    static constexpr double screen_area = 640*480;

private slots:
    void initTestCase();
    void test_coverage();
    void test_max_area();
    void test_median_area();
    void test_median_width();
    void test_placement_histogram();
    void cleanupTestCase();
};

FeatureExtraction::FeatureExtraction()
{

}

FeatureExtraction::~FeatureExtraction()
{

}

void FeatureExtraction::initTestCase()
{
    vector<Rect> v{Rect(0, 0, 200, 200), Rect(250, 250, 50, 20), Rect(50, 50, 50, 50), Rect(620, 0, 20, 20)};
    tf = ar::TextFeatures{std::move(v), 640, 480};
    nf = tf.normalize();
}

void FeatureExtraction::test_coverage()
{
    QCOMPARE(nf["coverage"], (200*200 + 50*20 + 50*50 + 20*20)/screen_area);
}

void FeatureExtraction::test_max_area()
{
    QCOMPARE(nf["max_area"], (200*200)/screen_area);
}

void FeatureExtraction::test_median_area()
{
    QCOMPARE(nf["median_area"], ((50*50 + 50*20)/2)/screen_area);
}

void FeatureExtraction::test_median_width()
{
    QCOMPARE(nf["median_width"], 50/640.0);
}

void FeatureExtraction::test_placement_histogram()
{
    QCOMPARE(nf["placement_0"], 2);
    QCOMPARE(nf["placement_3"], 1);
    QCOMPARE(nf["placement_9"], 1);
}

void FeatureExtraction::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(FeatureExtraction)

#include "tst_featureextraction.moc"
