#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"

/*
    Class L2 inherits from class CommonBase, handling the color-inverting functoinality of the program. The 
    method c_InvertColor is called from class Renderer.
*/
class L2 : public CommonBase
{
public:
    L2(cv::Mat imgOrig):CommonBase(imgOrig)
    {
        s_IvrtResult = imgOrig.clone();
    }

    virtual ~L2(){};
    
public:
    static cv::Mat s_IvrtResult;

protected:
    void c_InvertColor(const int ivrtTrackBarPos, const cv::Mat& smthResult)
    {
        s_IvrtResult = smthResult.clone();
        if (ivrtTrackBarPos == 0) // if the invert color trackbar is at 0, do not proceed. Otherwise ivrtRng will have size 0x0 and crash
            return;
        cv::Mat ivrtRng = s_IvrtResult.colRange(0, ivrtTrackBarPos);
        ivrtRng = cv::Scalar(255, 255, 255) - ivrtRng;
    }
};

cv::Mat L2::s_IvrtResult;