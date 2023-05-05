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
        
        static void invertCbk(int, void*)
        {

        }
};