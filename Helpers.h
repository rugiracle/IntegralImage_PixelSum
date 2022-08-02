#ifndef HELPERS_H_
#define HELPERS_H_

#include <iostream>
#include <cmath>

// Helper functions
template<typename T>
void printMatrix(T buffer, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << " "<<int(buffer[j + i * width]) << " ";
		}
		std::cout << std::endl;
	}
}
template<typename T>
void printSearchWindow(T& buffer, int x0, int y0, int x1, int y1, int width, int height)
{
	y1 = y1 > height ? height - 1 : y1;
	x1 = x1 > width ? width - 1 : x1;
	for (int i = y0; i <= y1; i++)
	{
		for (int j = x0; j <= x1; j++)
		{
			std::cout << int(buffer[j + i * width]) << " ";
		}
		std::cout << std::endl;
	}
}

template<typename T>
void updateWindow(T& buffer, int width, int height, int x0, int y0, int x1, int y1, unsigned value)
{
	for (int i = y0; i <= y1; i++)
	{
		for (int j = x0; j <= x1; j++)
		{
			buffer[j + i * width] = value;
		}
	}
}

static bool compare(double a, double b, double epsilon = 0.005)
{
	return (std::fabs(a - b) < epsilon);
}

#endif /* HELPERS_H_ */