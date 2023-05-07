#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "smoothenface.hpp"
#include "invertcolor.hpp"

class Renderer : public L1, public L2
{
public:
    int SmoothenBarVal;

public:

    Renderer(const cv::Mat& original):L1(original), L2(original)
    {
    }

    static void SmoothenCbk(int pos, void* rendObj_ptr)
    {
        float smoothness = (float)pos / 100;
        Renderer* rnder_ptr = (Renderer*)(rendObj_ptr);
        rnder_ptr->SmoothenBarVal = pos;
        rnder_ptr->SmoothenFace(smoothness);
        m_ShowImg(*rnder_ptr);
    }

    static void InvertCbk(int pos, void* rendObj_ptr)
    {
        Renderer *rnder_ptr = (Renderer *)(rendObj_ptr);
        static int previousSmoothVal = rnder_ptr->SmoothenBarVal;
        int currentSmoothVal = rnder_ptr->SmoothenBarVal;
        if (currentSmoothVal != previousSmoothVal)
            s_IvrtResult = 
        cv::Mat ivrtRng = rnder_ptr->s_SmthResult.colRange(0, pos);

        rnder_ptr->s_IvrtResult
    }

private:
    static void m_ShowImg(Renderer &rnd)
    {

    }
};