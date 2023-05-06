#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class L2
{
public:
    struct IvrtCfg
    {
        const char* windowName;
        cv::Mat &dispImg;
    };

    inline static IvrtCfg GetConfig(const char* windowName, cv::Mat &dispImg)
    {
        return IvrtCfg{windowName, dispImg};
    }

    static void invertCbk(int pos, void* cfg)
    {
        IvrtCfg *cfg_ptr = (IvrtCfg *)(cfg);
        cv::Mat ivrtCols = cfg_ptr->dispImg.colRange(0, pos);
        ivrtCols = cv::Scalar(255, 255, 255) - ivrtCols;
        cv::imshow(cfg_ptr->windowName, cfg_ptr->dispImg);
    }
protected:
    static cv::Mat s_IvrtResult;
};