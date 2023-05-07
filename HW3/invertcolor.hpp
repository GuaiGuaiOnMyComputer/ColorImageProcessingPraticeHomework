#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"
class L2 : public CommonBase
{
public:

    L2(cv::Mat imgOrig):CommonBase(imgOrig)
    {
    }

    static cv::Mat s_IvrtResult;
};

cv::Mat L2::s_IvrtResult;