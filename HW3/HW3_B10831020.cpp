#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main()
{
    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    cv::imshow("Original image", imgOrig);
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main()
{
    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    cv::imshow("Original image", imgOrig);

    cv::waitKey();
    return 0;
}