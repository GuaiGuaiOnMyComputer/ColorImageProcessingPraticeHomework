#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"
#include "eventno.hpp"

class Renderer : public L1, public L2
{
public:
    static int s_IvrtTrackbarPos;
    struct CbkArgLst
    {
        Renderer* rndObj_ptr;
        Events eventNo;
    };


    Renderer(const cv::Mat& original, const char* windowName):L1(original), L2(original), m_WindowName(windowName)
    {
        s_IvrtTrackbarPos = 0;
    }

    static void CommonTrackbarCbk(int pos, void* argLst)
    {
        CbkArgLst &args = *static_cast<CbkArgLst*>(argLst);
        Renderer rndObj = *args.rndObj_ptr;
        Events eventType = args.eventNo;
        switch (eventType)
        {
        case SMOOTHEN_TRACKBAR:
            rndObj.c_SmoothenFace(pos);
            rndObj.c_InvertColor(rndObj.s_SmthResult, rndObj.s_IvrtTrackbarPos);
            break;

        case INVERT_TRACKBAR:
            rndObj.c_InvertColor(pos, rndObj.s_SmthResult);
            break;
        }
        cv::imshow(rndObj.m_WindowName, s_IvrtResult);
    }

private:
    const char* m_WindowName;
};

int Renderer::s_IvrtTrackbarPos;