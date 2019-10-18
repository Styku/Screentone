#include <QtTest>

#include <vector>
#include <algorithm>
#include "helpers.h"

class TestHelpers : public QObject
{
    Q_OBJECT

public:
    TestHelpers();
    ~TestHelpers();

private slots:
    void test_median_even();
    void test_median_odd();
    void test_median_single();
    void test_median_long();
    void test_softmax();

};

TestHelpers::TestHelpers()
{

}

TestHelpers::~TestHelpers()
{

}

void TestHelpers::test_median_even()
{
    std::vector<double> v{1, 2, 3, 4, 5, 6, 7, 8};
    std::random_shuffle(v.begin(), v.end());
    QCOMPARE(ar::median(v), 4.5);
}

void TestHelpers::test_median_odd()
{
    std::vector<double> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_shuffle(v.begin(), v.end());
    QCOMPARE(ar::median(v), 5);
}

void TestHelpers::test_median_single()
{
    std::vector<double> v{1};
    std::random_shuffle(v.begin(), v.end());
    QCOMPARE(ar::median(v), 1);
}

void TestHelpers::test_median_long()
{
    std::vector<double> v(1000000, 0);
    std::random_shuffle(v.begin(), v.end());
    QCOMPARE(ar::median(v), 0);
}

void TestHelpers::test_softmax()
{
    std::vector<double> v {120, 23.3, 43.0};
    ar::softmax(v.begin(), v.end());
    QCOMPARE(std::accumulate(v.begin(), v.end(), .0), 1.0);
}

QTEST_APPLESS_MAIN(TestHelpers)

#include "tst_helpers.moc"
