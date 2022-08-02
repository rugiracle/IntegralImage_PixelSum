#include "Tests.h"
#include <chrono>
#include"Helpers.h"
#include <iostream>
#include<vector>
#include<iomanip>


Tests::Tests()
{
    int xWidth, yHeight;                                            // size of the input buffer
    std::vector<unsigned char> buffer;
    int x0, y0, x1, y1;                                             // search window 's location
    query_results given, results{ 0, 0.0, 0, 0.0 };;                //pre computed search window results
    xWidth = 7;
    yHeight = 6;
    buffer =
    {
        0, 2, 3, 4, 5, 6, 7,
        0, 1, 2, 3, 4, 5, 7,
        0, 1, 1, 1, 1, 0, 7,
        0, 2, 2, 2, 2, 0, 7,
        0, 3, 3, 3, 3, 1, 7,
        0, 0, 0, 0, 0, 0, 7
    };
    std::cout << "input buffer:[" << yHeight << "x" << xWidth << "]" << std::endl;
    printMatrix(buffer, xWidth, yHeight);
    x0 = 1, y0 = 1;
    x1 = 3, y1 = 2;
    given = { 9, 1.5, 6, 1.5 };                             // calculated values for the new search window

    tests.push_back({ "PixelSum data structure and operation testing...", x0, y0, x1, y1, xWidth, yHeight, results, given, buffer });

    //PixelSum copy constructor testing
    std::vector<unsigned char> buffer_ =                    // new buffer
    {
        3, 1,
        0, 3
    };
    int xWidth_ = 2, yHeight_ = 2;
    x0 = 0, y0 = 0;
    x1 = 1, y1 = 1;
    unsigned char* buff = new unsigned char[buffer_.size()];
    std::copy(buffer_.begin(), buffer_.end(), buff);
    PixelSum pixelSum1(buff, xWidth_, yHeight_);
    delete[] buff;
    PixelSum pixelSum1Copy(pixelSum1);
    given = compute_values(pixelSum1Copy, x0, y0, x1, y1);  //compute values from a copy of pixel sum
    
    tests.push_back({ "PixelSum copy constructor testing..." , x0, y0, x1, y1,xWidth_, yHeight_,results, given, buffer_ });

    //PixelSum assign operator testing....
    xWidth_ = 2, yHeight_ = 2;                          //new buffer size
    buffer_ =                    // new buffer
    {
        7, 8,
        9, 10
    };
    unsigned char* buffAssign = new unsigned char[buffer_.size()];
    std::copy(buffer_.begin(), buffer_.end(), buffAssign);
    PixelSum pixelSum2(buffAssign, xWidth_, yHeight_);    // pixelSumAssigned
    delete[] buffAssign;
    PixelSum Assigned(pixelSum1Copy);
    Assigned = pixelSum2;                           //assign pixelSum to  pixelSumAssigned
    given = compute_values(Assigned, x0, y0, x1, y1); //evaluate the search window on pixelSumAssigned: results should be the same
    tests.push_back({ "PixelSum assign operator testing...." , x0, y0, x1, y1, xWidth_, yHeight_,results, given , buffer_ });

    ////Updated buffer and further queries
    buffer_ =                    // new buffer
    {
        7, 8, 0, 0,
        4, 5, 6, 1,
        2, 5, 6, 1
    };
    xWidth_ = 4, yHeight_ = 3;

    x0 = 1, y0 = 1;
    x1 = 2, y1 = 2;
    unsigned char val = 9;
    unsigned char* buffUpdate = new unsigned char[buffer_.size()];
    std::copy(buffer_.begin(), buffer_.end(), buffUpdate);
    PixelSum pixelSumBuffUpdate(buffUpdate, xWidth_, yHeight_);
    given = compute_values(pixelSumBuffUpdate, x0, y0, x1, y1);         // compute search window results before buffer update, as true values
    updateWindow(buffUpdate, xWidth_, yHeight_, x0, y0, x1, y1, val);   // edit the input buffer[search window] and compute the new values
    
    tests.push_back({ "Updated buffer and further queries...." , x0, y0, x1, y1, xWidth_, yHeight_,results, given, buffer_ });
    delete[] buffUpdate;

    // Swaping test: unordered search window [x1<X0 & y1<y0]
    x0 = 4, y0 = 4;
    x1 = 2, y1 = 3;
    given = { 15, 2.5, 6, 2.5 };   
    tests.push_back({ "Swaping test: unordered search window [x1<X0 & y1<y0]...." , x0, y0, x1, y1,xWidth, yHeight, results, given, buffer });

    //Search window extending outside the buffer :
    x0 = 4, y0 = 0;
    x1 = 9, y1 = 3;
    given = { 51, 2.12, 10, 5.10 };    // calculated values for the new search window   
    tests.push_back({ "Search window extending outside the buffer...." , x0, y0, x1, y1, xWidth, yHeight,results, given , buffer });

    //Search windows containing only zero testing...
    x0 = 0, y0 = 5;
    x1 = 5, y1 = 5;
    given = { 0, 0.0, 0, 0.0 };    // calculated values for the new search window    
    tests.push_back({ "Search windows containing only zero testing...." , x0, y0, x1, y1, xWidth, yHeight,results, given , buffer });

    //Search window completely outside of the buffer
    x0 = 9, y0 = 8;
    x1 = 12, y1 = 10;    
    tests.push_back({ "Search window completely outside of the buffer...." , x0, y0, x1, y1, xWidth, yHeight,results, given , buffer });

    // Maximum buffer, maximum search window
    std::vector<unsigned char> maxBuffer(MAX_SIZE, 255);
    x0 = 0, y0 = 0;
    x1 = MAX_WIDTH - 1, y1 = MAX_HEIGHT - 1;
    xWidth = MAX_WIDTH;
    yHeight = MAX_HEIGHT;
    given = { 4278190080, MAX_VALUE, MAX_SIZE, MAX_VALUE };    // calculated values for the new search window
    tests.push_back({ "Maximum buffer[4096x4096], with maximum value...." , x0, y0, x1, y1,xWidth, yHeight, results, given ,maxBuffer });
   
    // // Testing too big buffer i.e.: size > MAX_SIZE ----> this should produce an error
    int OverMax_num = 100;
    xWidth = MAX_WIDTH + OverMax_num;
    yHeight = MAX_HEIGHT + OverMax_num;
    std::vector<unsigned char> TooBigBuffer(xWidth* yHeight, 255);

    x1 = MAX_WIDTH + OverMax_num, y1 = MAX_HEIGHT + OverMax_num;
     tests.push_back({ "Testing too big buffer i.e.: size > MAX_SIZE.... \n->intended to fail...." , x0, y0, x1, y1,xWidth, yHeight, results, given,TooBigBuffer });   
}


void Tests::run()
{
    int numTests = tests.size();
    bool status = 0;
    int Passed = 0;
    for (int i = 0; i < numTests; i++)
    {
        try {
                unsigned char* buffer_in = new unsigned char[tests[i].buffer.size()];
                std::copy(tests[i].buffer.begin(), tests[i].buffer.end(), buffer_in);
                PixelSum pixelSum(buffer_in, tests[i].xWidth, tests[i].yHeight);         // create PixelSum
                delete[] buffer_in;
                tests[i].results = { 0, 0.0, 0, 0.0 };
                tests[i].results = compute_values(pixelSum, tests[i].x0, tests[i].y0, tests[i].x1, tests[i].y1);  //calculated values for the new search window
                //check functions outputs correctness        
                status = (tests[i].given.pixelSum == tests[i].results.pixelSum) && (tests[i].given.nonZeroCount == tests[i].results.nonZeroCount) && compare(tests[i].given.pixelAvg, tests[i].results.pixelAvg) && compare(tests[i].given.nonZeroAverage, tests[i].results.nonZeroAverage);
                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Test Case[" << i + 1 << "]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                std::cout << "                                         *" << (status > 0 ? "Passed" : "Failed") << "*" << std::endl; //print test status: Passed or failed
                std::cout << tests[i].name << std::endl;
                std::cout << "Search window: (x0=" << tests[i].x0 << " y0=" << tests[i].y0 << " x1=" << tests[i].x1 << " y1=" << tests[i].y1 <<")"<< std::endl;//print search window
                if (tests[i].xWidth < 20 || tests[i].yHeight < 20) //just to avoid printing big arrays
                {
                    printSearchWindow(tests[i].buffer, tests[i].x0, tests[i].y0, tests[i].x1, tests[i].y1, tests[i].xWidth, tests[i].yHeight);
                }
                std::cout << "the pixelSum is : " << tests[i].results.pixelSum << "[" << tests[i].given.pixelSum << "]" << std::endl;
                std::cout << "the pixelAvg is : " << tests[i].results.pixelAvg << "[" << tests[i].given.pixelAvg << "]" << std::endl;
                std::cout << "the nonZeroCnt is : " << tests[i].results.nonZeroCount << "[" << tests[i].given.nonZeroCount << "]" << std::endl;
                std::cout << "the nonZeroCntAvg is : " << tests[i].results.nonZeroAverage << "[" << tests[i].given.nonZeroAverage << "]" << std::endl;


                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ENd Test Case[" << i + 1 << "]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                Passed += status;
            }
        catch (std::exception& e)
        {
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Test Case[" << i + 1 << "]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << "                                         *" << "Supposed to fail" << "*" << std::endl;
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Search window: (x0=" << tests[i].x0 << " y0=" << tests[i].y0 << " x1=" << tests[i].x1 << " y1=" << tests[i].y1 <<")"<< std::endl << std::endl;//print search window
        }

    }
    std::cout << "                               << Tests results: " << Passed << " Passed, " << (numTests - Passed) << " Failed >>" << std::endl;
}

query_results Tests::compute_values(PixelSum& p_sum, int x0, int y0, int x1, int y1)
{
    query_results res = { 0,0.0,0,0.0 };
    p_sum.checkSwap(x0, y0, x1, y1); // rearrange search window if necessary to satisfy x0 <=x1 & y0<=y1
    int num_pxl = (x1 - x0 + 1) * (y1 - y0 + 1); //to account for the original size of the search window   
    if (p_sum.checkOutOfBorders(x0, y0, x1, y1))// if search window completely outside the buffer no need to computer anything
        return res;
    p_sum.clampBorders(x0, y0, x1, y1);
    res.pixelSum = p_sum.getPixelSum(x0, y0, x1, y1);
    res.pixelAvg = p_sum.getPixelAverage(x0, y0, x1, y1, num_pxl);
    res.nonZeroCount = p_sum.getNonZeroCount(x0, y0, x1, y1);
    res.nonZeroAverage = p_sum.getNonZeroAverage(x0, y0, x1, y1);
    return res;
}
Tests::~Tests() {}



