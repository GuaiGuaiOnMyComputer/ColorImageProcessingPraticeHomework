#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "renderer.hpp"
#include "eventno.hpp"

constexpr char g_WINDOWNAME[] = "HW3_B10831020";

int main()
{
    int smoothnessVal{0};
    int ivrtVal{0};

    cv::namedWindow(g_WINDOWNAME, cv::WINDOW_AUTOSIZE);

    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    Renderer rend(imgOrig, g_WINDOWNAME);
    Renderer::CbkArgLst smthTrackBarArgs{&rend, SMOOTHEN_TRACKBAR};
    Renderer::CbkArgLst ivrtTrackBarArgs{&rend, INVERRT_TRACKBAR};
    cv::imshow(g_WINDOWNAME, imgOrig);
    // cv::createTrackbar("Smoothness", g_WINDOWNAME, nullptr, 100, Renderer::SmoothenCbk, &rend);
    // cv::createTrackbar("Invert", g_WINDOWNAME, nullptr, dispImg.cols, Renderer::InvertCbk, &rend);
    cv::createTrackbar("Smoothness", g_WINDOWNAME, nullptr, 100, Renderer::CommonTrackbarCbk, (void*)&smthTrackBarArgs);
    cv::createTrackbar("Invert", g_WINDOWNAME, nullptr, imgOrig.cols, Renderer::CommonTrackbarCbk, (void*)&ivrtTrackBarArgs);
    cv::setTrackbarPos("Smoothness", g_WINDOWNAME, 0);
    cv::setTrackbarPos("Invert", g_WINDOWNAME, 0);

    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}