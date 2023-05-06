#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "smoothenface.hpp"
class L2
{
public:
    struct IvrtCfg
    {
        const char* windowName;
        L1 faceSmther;
    };

    inline static IvrtCfg GetConfig(const char* windowName, L1 &faceSmther)
    {
        return IvrtCfg{windowName, faceSmther};
    }

    static void invertCbk(int pos, void* cfg)
    {
        IvrtCfg *cfg_ptr = (IvrtCfg *)(cfg);
        cv::Mat ivrtCols = cfg_ptr->faceSmther->s_.colRange(0, pos);
        ivrtCols = cv::Scalar(255, 255, 255) - ivrtCols;
    }

    static cv::Mat s_IvrtResult;
};