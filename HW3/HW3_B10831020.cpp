#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "smoothenface.hpp"

constexpr char g_WINDOWNAME[] = "HW3_B10831020";
int L1::ms_Height, L1::ms_Width;

int main()
{
    // ss();
    int smoothnessVal{0};
    cv::namedWindow(g_WINDOWNAME, cv::WINDOW_AUTOSIZE);

    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    L1 faceSmoothener = L1(imgOrig);
    L1::FaceSmoothenConfig faceSmoothConfig = faceSmoothener.GetConfig(g_WINDOWNAME);
    cv::createTrackbar("Smoothness", g_WINDOWNAME, &smoothnessVal, 100, L1::SmoothenFace);

    L1::SmoothenFace(smoothnessVal, &faceSmoothConfig);
    cv::waitKey();
}