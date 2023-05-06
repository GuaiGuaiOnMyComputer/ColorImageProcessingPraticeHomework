#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"

class Renderer : public L1, public L2
{
public:
    Renderer(const cv::Mat& original):L1(original), L2(original)
    {
    }

    inline L1::SmthCfg GetConfig(const char* windowName, cv::Mat &dispImg)
    {
        return L1::SmthCfg{ *static_cast<L1*>(this), windowName, dispImg};
    }

public:
    cv::Mat s_SmthReult;

protected:

protected:
    static cv::Mat ps_SmthResult;
};