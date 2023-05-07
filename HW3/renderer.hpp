#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"
#include "eventno.hpp"

class Renderer : public L1, public L2
{
public:
    struct CbkArgLst
    {
        Renderer* rndObj_ptr;
        Events eventNo;
    };


    Renderer(const cv::Mat& original, const char* windowName):L1(original), L2(original), m_WindowName(windowName)
    {
    }

    static void CommonTrackbarCbk(int pos, void* argLst)
    {
        CbkArgLst &args = *static_cast<CbkArgLst*>(argLst);
        Renderer rndObj = *args.rndObj_ptr;
        int eventType = args.eventNo;
        switch (eventType)
        {
        case SMOOTHEN_TRACKBAR:
            rndObj.c_SmoothenFace(pos);
            rndObj.c_InvertColor(rndObj.s_SmthResult);
            break;
        
        default:
            break;
        }
        rndObj.m_ShowImg();
        /*
        if smoothen bar move:
            smoothen original image
            invert selected range
            end

        if invert bar move:
            invert s_smthResult within range
            end
        */
    }
    

    // static void SmoothenCbk(int pos, void* rendObj_ptr)
    // {
    //     float smoothness = (float)pos / 100;
    //     Renderer* rnder_ptr = static_cast<Renderer*>(rendObj_ptr);
    //     rnder_ptr->SmoothenBarVal = pos;
    //     rnder_ptr->c_SmoothenFace(smoothness);
    //     InvertColor(rnder_ptr->IvrtBarVal, rnder_ptr->s_IvrtResult);
    //     m_ShowImg(*rnder_ptr);
    // }

    // static void InvertCbk(int pos, void* rendObj_ptr)
    // {
    //     Renderer *rnder_ptr = static_cast<Renderer*>(rendObj_ptr);
    //     rnder_ptr->IvrtBarVal = pos;
    //     if (rnder_ptr->m_PreviousSmoothenBarVal != rnder_ptr->SmoothenBarVal){
    //         rnder_ptr->c_SmoothenFace();
    //         rnder_ptr->s_IvrtResult = s_SmthResult.clone();
    //         rnder_ptr->m_PreviousSmoothenBarVal = rnder_ptr->SmoothenBarVal;
    //     }
    //     rnder_ptr->InvertColor(pos, rnder_ptr->s_SmthResult);
    //     m_ShowImg(*rnder_ptr);
    // }

private:
    void m_ShowImg()
    {
        cv::imshow(m_WindowName, s_IvrtResult);
    }

private:
    const char* m_WindowName;
};