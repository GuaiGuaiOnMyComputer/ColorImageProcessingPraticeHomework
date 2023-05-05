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
            L1 &Smth_ref;
            const char* WindowName;
        };

        L1(const cv::Mat& original):m_original(original), m_ImgSize(original.size())
        {
            m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8U);
            m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_Background = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_MakeFaceMask();
        }

        static void SmoothenFace(int pos, void* smoothenerStruct)
        {
            static cv::Mat tmp(ms_Height, ms_Width, CV_8UC3);
            SmthCfg* cfg_ptr = (SmthCfg*)(smoothenerStruct);
            const cv::Mat &faceArea = cfg_ptr->Smth_ref.m_OriginalFace;
            const cv::Mat &bgArea = cfg_ptr->Smth_ref.m_Background;
            const cv::Mat &mask = cfg_ptr->Smth_ref.m_FaceMask;
            cv::GaussianBlur(faceArea, tmp, cv::Size(11, 11), pos, pos);
            for (size_t pixel = 0; pixel < ms_Height * ms_Width; pixel++){
                tmp.data[3 * pixel + 0] = mask.data[pixel] != 0 ? tmp.data[3 * pixel + 0] : bgArea.data[3 * pixel + 0];
                tmp.data[3 * pixel + 1] = mask.data[pixel] != 0 ? tmp.data[3 * pixel + 1] : bgArea.data[3 * pixel + 1];
                tmp.data[3 * pixel + 2] = mask.data[pixel] != 0 ? tmp.data[3 * pixel + 2] : bgArea.data[3 * pixel + 2];
            }
            cv::imshow(cfg_ptr->WindowName, tmp);
        }

        inline SmthCfg GetConfig(const char* windowName)
        {
            ms_Height = m_ImgSize.height;
            ms_Width = m_ImgSize.width;
            return SmthCfg{ *this, windowName};
        }

    private:
        void m_MakeFaceMask()
        {
            cv::Mat imgHsv(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            cv::cvtColor(m_original, imgHsv, cv::COLOR_BGR2HSV);
            cv::inRange(imgHsv, cv::Scalar(0, 20, 80), cv::Scalar(40, 255, 255), m_FaceMask);
            m_FaceMask /= 255;

            cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(13, 13));
            cv::erode(m_FaceMask, m_FaceMask, erosionElement);

            for (size_t pixel = 0; pixel < m_ImgSize.height * m_ImgSize.width; pixel ++){
                m_OriginalFace.data[3 * pixel + 0] = m_original.data[3 * pixel + 0] * m_FaceMask.data[pixel];
                m_OriginalFace.data[3 * pixel + 1] = m_original.data[3 * pixel + 1] * m_FaceMask.data[pixel];
                m_OriginalFace.data[3 * pixel + 2] = m_original.data[3 * pixel + 2] * m_FaceMask.data[pixel];
            }
            for (size_t pixel = 0; pixel < m_ImgSize.height * m_ImgSize.width; pixel ++){
                m_Background.data[3 * pixel + 0] = m_original.data[3 * pixel + 0] * (1 - m_FaceMask.data[pixel]);
                m_Background.data[3 * pixel + 1] = m_original.data[3 * pixel + 1] * (1 - m_FaceMask.data[pixel]);
                m_Background.data[3 * pixel + 2] = m_original.data[3 * pixel + 2] * (1 - m_FaceMask.data[pixel]);
            }
        }

    private:
        cv::Mat m_FaceMask;
        cv::Mat m_OriginalFace;
        cv::Mat m_Background;
        const cv::Size2d m_ImgSize;
        const cv::Mat &m_original;
        static int ms_Width, ms_Height;

};
