#pragma once

#include <opencv2/imgcodecs.hpp>

class CommonBase
{
protected:
    CommonBase(const cv::Mat &imgOrg):m_ImgSize(imgOrg.size()), m_Original(imgOrg)
    {}
public:
    const cv::Size2d m_ImgSize;
    const cv::Mat &m_Original;
    int m_Width, m_Height;
};