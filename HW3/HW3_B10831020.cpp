#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "renderer.hpp"
#include "eventno.hpp"

constexpr char g_WINDOWNAME[] = "HW3_B10831020";

int main()
{
    bool programRunning = true;
    cv::namedWindow(g_WINDOWNAME, cv::WINDOW_AUTOSIZE);

    cv::Mat imgOrig = cv::imread("Sun_oil.jpg", cv::IMREAD_COLOR);
    Renderer rend(imgOrig, g_WINDOWNAME);
    Renderer::CbkArgLst smthTrackBarArgs{&rend, SMOOTHEN_TRACKBAR};
    Renderer::CbkArgLst ivrtTrackBarArgs{&rend, INVERT_TRACKBAR};
    cv::imshow(g_WINDOWNAME, imgOrig);
    cv::createTrackbar("Smoothness", g_WINDOWNAME, &Renderer::s_SmthTrackbarPos, 100, Renderer::CommonTrackbarCbk, (void*)&smthTrackBarArgs);
    cv::createTrackbar("Invert", g_WINDOWNAME, &Renderer::s_IvrtTrackbarPos, imgOrig.cols, Renderer::CommonTrackbarCbk, (void*)&ivrtTrackBarArgs);
    cv::setMouseCallback(g_WINDOWNAME, Renderer::MouseCbk, (void*)&rend);
    cv::setTrackbarPos("Smoothness", g_WINDOWNAME, 0);
    cv::setTrackbarPos("Invert", g_WINDOWNAME, 0);

    while (programRunning){
        int keyAscii = cv::waitKeyEx();
        switch (keyAscii)
        {
        case 32: // space key pressed
            rend.Reset();
            break;
        case 82: // R key pressed
            rend.Reset();
            break;
        case 114: // r key pressed
            rend.Reset();
            break;
        case 27: // esc key pressed
            programRunning = false;
        default:
            break;
        }
    }
    
    cv::destroyAllWindows();
    return 0;
}