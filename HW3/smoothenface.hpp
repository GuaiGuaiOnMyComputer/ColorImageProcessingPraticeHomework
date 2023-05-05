#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <memory.h>

class L1
{
    public:
        struct SmthCfg
        {
            L1 &Smoothen_ref;
            const char* WindowName;
            cv::Mat &dispImg_ref;
        };

        L1(const cv::Mat& original):m_original(original), m_ImgSize(original.size())
        {
            m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8U);
            m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_Background = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_MakeFaceMask();
        }

        static void SmoothenCbk(int pos, void* smoothenerStruct)
        {
            float smoothness = (float)pos / 100;
            static cv::Mat tmp(ms_Height, ms_Width, CV_8UC3);
            SmthCfg* cfg_ptr = (SmthCfg*)(smoothenerStruct);
            const cv::Mat &faceArea = cfg_ptr->Smoothen_ref.m_OriginalFace;
            const cv::Mat &bgArea = cfg_ptr->Smoothen_ref.m_Background;
            const cv::Mat &originalImg = cfg_ptr->Smoothen_ref.m_original;
            const cv::Mat &mask = cfg_ptr->Smoothen_ref.m_FaceMask;
            cv::Mat &dispImg_ref = cfg_ptr->dispImg_ref;
            cv::GaussianBlur(faceArea, tmp, cv::Size(13, 13), 100, 100);
            tmp += bgArea;
            
            cv::addWeighted(tmp, smoothness, originalImg, 1 - smoothness, 0, dispImg_ref);
            cv::imshow(cfg_ptr->WindowName, dispImg_ref);
        }

        inline SmthCfg GetConfig(const char* windowName, cv::Mat &dispImg)
        {
            ms_Height = m_ImgSize.height;
            ms_Width = m_ImgSize.width;
            return SmthCfg{ *this, windowName, dispImg};
        }

    private:
        void m_MakeFaceMask()
        {
            cv::Mat imgHsv(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            cv::cvtColor(m_original, imgHsv, cv::COLOR_BGR2HSV);
            cv::inRange(imgHsv, cv::Scalar(0, 30, 80), cv::Scalar(40, 255, 255), m_FaceMask);
            m_FaceMask /= 255;

            cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
            cv::erode(m_FaceMask, m_FaceMask, erosionElement);
            cv::copyTo(m_original, m_OriginalFace, m_FaceMask);
            cv::copyTo(m_original, m_Background, 1 - m_FaceMask);
        }

    private:
        cv::Mat m_FaceMask;
        cv::Mat m_OriginalFace;
        cv::Mat m_Background;
        const cv::Size2d m_ImgSize;
        const cv::Mat &m_original;
        static int ms_Width, ms_Height;

};