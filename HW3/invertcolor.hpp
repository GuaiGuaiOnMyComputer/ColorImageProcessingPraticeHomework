#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"
class L2 : public CommonBase
{
public:

    L2(cv::Mat imgOrig):CommonBase(imgOrig)
    {}
    struct IvrtCfg
    {
        const char* windowName;
    };

    static void invertCbk(int pos, void* cfg)
    {
        // IvrtCfg *cfg_ptr = (IvrtCfg *)(cfg);
        // cv::Mat ivrtCols = cfg_ptr->faceSmther->s_.colRange(0, pos);
        // ivrtCols = cv::Scalar(255, 255, 255) - ivrtCols;
    }

    static cv::Mat s_IvrtResult;
};