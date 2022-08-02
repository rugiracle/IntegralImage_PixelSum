#ifndef TESTS_H_
#define TESTS_H_
#include <vector>
#include <string>
#include "PixelSum.h"

struct query_results
{
    unsigned int pixelSum;
    double pixelAvg;
    int nonZeroCount;
    double nonZeroAverage;
};

struct TestCase
{
    std::string name;                       // name of the test case
    int x0;
    int y0;
    int x1;
    int y1;
    int xWidth;
    int yHeight;
    query_results results;                  // PixelSum computed results
    query_results given;                    // given data for correctness checking
    std::vector<unsigned char> buffer;      //just to print the search window
};

class Tests
{
public:
    std::vector<TestCase> tests;
    Tests();
    query_results compute_values(PixelSum& p_sum, int x0, int y0, int x1, int y1);
    void run();
    virtual ~Tests();
};

#endif 