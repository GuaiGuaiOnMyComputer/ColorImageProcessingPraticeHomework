#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"

constexpr char g_WINDOWNAME[] = "HW3_B10831020";
int L1::ms_Height, L1::ms_Width;

int main()
{
    int smoothnessVal{0};
    cv::namedWindow(g_WINDOWNAME, cv::WINDOW_AUTOSIZE);

    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    cv::Mat dispImg = imgOrig.clone();
    cv::imshow(g_WINDOWNAME, dispImg);
    L1 faceSmoothener = L1(imgOrig);
    L1::SmthCfg faceSmoothConfig = faceSmoothener.GetConfig(g_WINDOWNAME, dispImg);
    L2::IvrtCfg colorIvrtConfig = L2::GetConfig(g_WINDOWNAME, faceSmoothener);
    cv::createTrackbar("Smoothness", g_WINDOWNAME, nullptr, 100, L1::SmoothenCbk, &faceSmoothConfig);
    cv::createTrackbar("Invert", g_WINDOWNAME, nullptr, dispImg.cols, L2::invertCbk, &colorIvrtConfig);
    cv::setTrackbarPos("Smoothness", g_WINDOWNAME, 0);
    cv::setTrackbarPos("Invert", g_WINDOWNAME, 0);

    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}