#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"
class L2 : public CommonBase
{
public:

    L2(cv::Mat imgOrig):CommonBase(imgOrig)
    {
        s_IvrtResult = imgOrig.clone();
    }
public:
    static cv::Mat s_IvrtResult;

protected:
    void c_InvertColor(const int ivrtTrackBarPos, const cv::Mat& smthResult)
    {
        c_InvertBarPos = ivrtTrackBarPos;
        s_IvrtResult = smthResult.clone();
        cv::Mat ivrtRng = s_IvrtResult.colRange(0, c_InvertBarPos);
        ivrtRng = cv::Scalar(255, 255, 255) - ivrtRng;
    }
    void c_InvertColor(const cv::Mat& smthResult)
    {
        s_IvrtResult = smthResult.clone();
        if (c_InvertBarPos == 0)
            return;
        cv::Mat ivrtRng = s_IvrtResult.colRange(0, c_InvertBarPos);
        ivrtRng = cv::Scalar(255, 255, 255) - ivrtRng;
    }

protected:
    int c_InvertBarPos;
    int c_PreviousSmoothenBarVal;
};

cv::Mat L2::s_IvrtResult;