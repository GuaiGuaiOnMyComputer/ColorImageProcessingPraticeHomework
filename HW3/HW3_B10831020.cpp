#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"
#include "renderer.hpp"

constexpr char g_WINDOWNAME[] = "HW3_B10831020";

int main()
{
    int smoothnessVal{0};
    int ivrtVal{0};

    cv::namedWindow(g_WINDOWNAME, cv::WINDOW_AUTOSIZE);

    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    Renderer rend(imgOrig);
    cv::Mat dispImg = imgOrig.clone();
    cv::imshow(g_WINDOWNAME, dispImg);
    cv::createTrackbar("Smoothness", g_WINDOWNAME, nullptr, 100, L1::SmoothenCbk, &rend);
    cv::createTrackbar("Invert", g_WINDOWNAME, nullptr, dispImg.cols, L2::invertCbk, &rend);
    cv::setTrackbarPos("Smoothness", g_WINDOWNAME, 0);
    cv::setTrackbarPos("Invert", g_WINDOWNAME, 0);

    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}