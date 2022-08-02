#include "PixelSum.h"
#include <iostream>


PixelSum::PixelSum(const unsigned char* buffer, int xWidth, int yHeight) : xWidth(xWidth), yHeight(yHeight)
{
    int size = xWidth * yHeight;
    if (size > MAX_SIZE)
    {
          throw std::exception("Invalid input size: must best < 4096 x 4096");      //input must be <  < 4096 x 4096
    }
       
    // Create copy of source data
    copyData(sourceData, buffer, size);

    // Create integral image and zero table (for getNonZeroCount)
    integralImage = new unsigned int[size];
    zeroTable = new int[size];

    // @ https://en.wikipedia.org/wiki/Summed-area_table
    integralImage[0] = sourceData[0];
    zeroTable[0] = (sourceData[0] > 0) ? 1 : 0;

    for (int x = 1; x < xWidth; x++)
    {
        integralImage[x] = integralImage[x - 1] + sourceData[x];
        zeroTable[x] = zeroTable[x - 1] + ((sourceData[x] > 0) ? 1 : 0);
    }
    for (int y = 1; y < yHeight; y++)
    {
        unsigned int sum = sourceData[0 + y * xWidth];
        int sumForZeroTable = (sourceData[0 + y * xWidth] > 0) ? 1 : 0;
        integralImage[0 + y * xWidth] = integralImage[0 + (y - 1) * xWidth] + sum;
        zeroTable[0 + y * xWidth] = zeroTable[0 + (y - 1) * xWidth] + sumForZeroTable;
        for (int x = 1; x < xWidth; x++)
        {
            sum += sourceData[x + y * xWidth];
            sumForZeroTable += (sourceData[x + y * xWidth]) ? 1 : 0;
            integralImage[x + y * xWidth] = integralImage[x + (y - 1) * xWidth] + sum;
            zeroTable[x + y * xWidth] = zeroTable[x + (y - 1) * xWidth] + sumForZeroTable;
        }
    }
}

PixelSum::~PixelSum()
{
    delete[] sourceData;
    delete[] integralImage;
    delete[] zeroTable;
}

PixelSum::PixelSum(const PixelSum& otherPixelSum)
{
    // Create and store a copy of otherPixelSum's data
    xWidth = otherPixelSum.xWidth;
    yHeight = otherPixelSum.yHeight;
    int size = xWidth * yHeight;

    // Create copy of source data
    copyData(sourceData, otherPixelSum.sourceData, size);
    // Create copy of integral image
    copyData(integralImage, otherPixelSum.integralImage, size);
    // Create copy of zero table
    copyData(zeroTable, otherPixelSum.zeroTable, size);
}

PixelSum& PixelSum::operator=(const PixelSum& otherPixelSum)
{
    // Check if the same object
    if (this == &otherPixelSum)
    {
        return *this;
    }
    // Create and store a copy of otherPixelSum's data
    xWidth = otherPixelSum.xWidth;
    yHeight = otherPixelSum.yHeight;
    int size = xWidth * yHeight;

    // Create copy of source data
    copyData(sourceData, otherPixelSum.sourceData, size);
    // Create copy of integral image
    copyData(integralImage, otherPixelSum.integralImage, size);
    // Create copy of zero table
    copyData(zeroTable, otherPixelSum.zeroTable, size);

    return *this; // return a reference to self
}

bool PixelSum::sizeCheck(int t_xWidth, int t_yHeight)
{
    if (t_xWidth * t_xWidth > MAX_SIZE)
        return 0;
    else
        return 1;
}

unsigned int PixelSum::getPixelSum(int x0, int y0, int x1, int y1) const
{
    // Calculate
    auto pixelSum = getSumOfSearchWindow(integralImage, x0, y0, x1, y1);
    return pixelSum;
}

double PixelSum::getPixelAverage(int x0, int y0, int x1, int y1, int num_pxl) const
{
    // average = pixel sum / total num of pixels
    auto pixelSum = getPixelSum(x0, y0, x1, y1);
    //int numPixels = (x1 - x0 + 1) * (y1 - y0 + 1);
    double average = double(pixelSum) / double(num_pxl);

    return average;
}

int PixelSum::getNonZeroCount(int x0, int y0, int x1, int y1) const
{
    auto nonZeroCount = getSumOfSearchWindow(zeroTable, x0, y0, x1, y1);
    return nonZeroCount;
}

double PixelSum::getNonZeroAverage(int x0, int y0, int x1, int y1) const
{
    auto nonZeroCount = getNonZeroCount(x0, y0, x1, y1);
    if (nonZeroCount != 0)
    {
        auto pixelSum = getPixelSum(x0, y0, x1, y1);
        double average = double(pixelSum) / double(nonZeroCount);
        return average;
    }
    return 0.0;
}

template<typename T>
void PixelSum::copyData(T*& dst, const T* src, int size)
{
    delete[] dst; // should be no memory leak
    dst = new T[size];
    for (int i = 0; i < size; ++i)
    {
        dst[i] = src[i];
    }
}

template<typename T>
T PixelSum::getSumOfSearchWindow(const T* src, int x0, int y0, int x1, int y1) const
{
    // @ https://en.wikipedia.org/wiki/Summed-area_table    

    T A = 0, B = 0, C = 0, D = 0;

    D = src[x1 + y1 * xWidth];
    if (x0 > 0)
    {
        C = src[(x0 - 1) + y1 * xWidth];
    }
    if (y0 > 0)
    {
        B = src[x1 + (y0 - 1) * xWidth];
    }
    if (x0 > 0 && y0 > 0)
    {
        A = src[(x0 - 1) + (y0 - 1) * xWidth];
    }

    return D - B - C + A;
}

bool PixelSum::checkOutOfBorders(int& x0, int& y0, int& x1, int& y1) const
{
    if (x0 >= xWidth || x1 < 0)
    {
        return true;
    }

    if (y0 >= yHeight || y1 < 0)
    {
        return true;
    }

    return false;
}

void PixelSum::clampBorders(int& x0, int& y0, int& x1, int& y1) const
{
    // Check if values too large
    x0 = (x0 >= xWidth) ? xWidth - 1 : x0;
    y0 = (y0 >= yHeight) ? yHeight - 1 : y0;
    x1 = (x1 >= xWidth) ? xWidth - 1 : x1;
    y1 = (y1 >= yHeight) ? yHeight - 1 : y1;

    // Check if negative
    x0 = (x0 < 0) ? 0 : x0;
    y0 = (y0 < 0) ? 0 : y0;
    x1 = (x1 < 0) ? 0 : x1;
    y1 = (y1 < 0) ? 0 : y1;
}

void PixelSum::checkSwap(int& x0, int& y0, int& x1, int& y1) const
{
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);
}