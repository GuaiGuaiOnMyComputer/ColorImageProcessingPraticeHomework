#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "smoothenface.hpp"

int main()
{
    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    L1 foo = L1(imgOrig);
}