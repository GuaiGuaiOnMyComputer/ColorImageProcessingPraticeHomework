#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"
#include "eventno.hpp"
#include "mousecontrol.hpp"

class Renderer : public L1, public L2, public L3
{
public:
    static int s_IvrtTrackbarPos, s_SmthTrackbarPos;
    static cv::Mat s_DisplayImg;
    struct CbkArgLst
    {
        Renderer* rndObj_ptr;
        Events eventNo;
    };


    Renderer(const cv::Mat& original, const char* windowName):L1(original), L2(original), L3(), m_WindowName(windowName)
    {
        s_DisplayImg = original.clone();
        s_IvrtTrackbarPos = 0;
        s_SmthTrackbarPos = 0;
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
    static void MouseCbk(int event, int x, int y, int _, void* userData)
    {
        Renderer &rndObj = *static_cast<Renderer*>(userData);
        switch (event)
        {
        case cv::EVENT_LBUTTONDOWN:
            rndObj.c_LeftButtonDownCbk(x, y);
            break;
        case cv::EVENT_LBUTTONUP:
            rndObj.c_LeftButtonUpCbk(x, y, s_IvrtResult, s_DisplayImg);
            break;

        default:
            break;
        }
    }

private:
    const char* m_WindowName;
};

int Renderer::s_IvrtTrackbarPos;
int Renderer::s_SmthTrackbarPos;
cv::Mat Renderer::s_DisplayImg;