#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"
#include "eventno.hpp"
#include "mousecontrol.hpp"

/*
    Render class inherits from classes L1, L2 and L3, containing all the callbacks for the UI elements.
*/
class Renderer : public L1, public L2, public L3
{
public:
    static int s_IvrtTrackbarPos; // the real-time position of the color invert trackbar
    static int s_SmthTrackbarPos; // the real-time position of the face smoothen trackbar
    static cv::Mat s_DisplayImg;  // image to be shown on the UI window

    /*
        Struct CbkArgLst is passed into the callback functions of the two trackbars as the second
        argument (void*), so the static callback functions can manipulate the render instance through
        a pointer to the rend instance in the main() function. Also, to identify which trackbar is being
        moved, this struct also contains an int identifying the operating trackbar.
    */
    struct CbkArgLst
    {
        Renderer* rndObj_ptr;
        Events eventNo;
    };


    Renderer(const cv::Mat& original, const char* windowName):L1(original), L2(original), L3(), m_WindowName(windowName)
    {
        s_DisplayImg = original.clone(); // show the original image in the beginning
        s_IvrtTrackbarPos = 0;
        s_SmthTrackbarPos = 0;
    }

    /*
        The common callback function for the trackbars. The argument void* argLst is an instance pointer of 
        struct CbkArgLst. The member eventNo in CbkArgLst identifies which trackbar is being moved and use 
        switch-case statement to trigger the corresponding operations.

        SmoothenFaceTrackberMoves:
            update s_SmthResult in class L1
            update s_IvrtResult in class L2
            draw the existing lines in class L3
            display image

        InvertColorTrackberMoves:
            update s_IvrtResult in class L2
            draw the existing lines in class L3
            display image
    */
    static void CommonTrackbarCbk(int pos, void* argLst)
    {
        CbkArgLst &args = *static_cast<CbkArgLst*>(argLst);
        Renderer rndObj = *args.rndObj_ptr;
        Events eventType = args.eventNo;
        switch (eventType)
        {
        case SMOOTHEN_TRACKBAR:
            rndObj.c_SmoothenFace(pos);
            rndObj.c_InvertColor(rndObj.s_IvrtTrackbarPos, rndObj.s_SmthResult);
            rndObj.c_DrawLines(s_IvrtResult, s_DisplayImg);
            break;

        case INVERT_TRACKBAR:
            rndObj.c_InvertColor(pos, rndObj.s_SmthResult);
            rndObj.c_DrawLines(s_IvrtResult, s_DisplayImg);
            break;
        }
        cv::imshow(rndObj.m_WindowName, s_DisplayImg);
    }

    /*
        The callback function for the mouse events. The argument rnd_ptr is a pointer to an instance of Render object
        from main().

        LeftButtonDown:
            add line starting point into c_Points in class L3
            display image

        LeftButtonUp:
            add line end point into c_Points in class L3
            draw all the lines on the image
            display image
    */
    static void MouseCbk(int event, int x, int y, int _, void* rnd_ptr)
    {
        Renderer &rndObj = *static_cast<Renderer*>(rnd_ptr);
        switch (event)
        {
        case cv::EVENT_LBUTTONDOWN:
            rndObj.c_AddLineStartPt(x, y);
            break;
        case cv::EVENT_LBUTTONUP:
            rndObj.c_AddLineEndPt(x, y);
            rndObj.c_DrawLines(s_IvrtResult, s_DisplayImg);
            break;
        default:
            break;
        }
        cv::imshow(rndObj.m_WindowName, s_DisplayImg);
    }

private:
    const char* m_WindowName;
};

int Renderer::s_IvrtTrackbarPos;
int Renderer::s_SmthTrackbarPos;
cv::Mat Renderer::s_DisplayImg;