#pragma once
/*
This class is only meant to be inherited by class L1, L2 and Render, so the derived classes can share the following properties:
    * c_ImgSize
    * c_Original
    * c_Width
    * c_Height
Both class L1 and L2 calls the constructor of class CommonBase
*/
#include <opencv2/imgcodecs.hpp>

class CommonBase
{
protected:
    CommonBase(const cv::Mat &imgOrg):c_ImgSize(imgOrg.size()), c_Original(imgOrg)
    {}
    const cv::Size2d c_ImgSize;
    const cv::Mat &c_Original;
    int c_Width, c_Height;
};