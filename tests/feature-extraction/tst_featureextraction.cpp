#include <QtTest>

#include <opencv2/opencv.hpp>
#include <vector>

#include "textfeatures.h"

class FeatureExtraction : public QObject
{
    Q_OBJECT

public:
    FeatureExtraction();
    ~FeatureExtraction();

private slots:
    void test_case1();

};

FeatureExtraction::FeatureExtraction()
{

}

FeatureExtraction::~FeatureExtraction()
{

}

void FeatureExtraction::test_case1()
{

}

QTEST_APPLESS_MAIN(FeatureExtraction)

#include "tst_featureextraction.moc"
