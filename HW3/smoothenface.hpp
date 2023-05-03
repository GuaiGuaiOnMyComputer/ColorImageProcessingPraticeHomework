#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <memory.h>

class L1
{
    public:
        struct FaceSmoothenConfig
        {
            std::shared_ptr<L1> Smoothener_ptr;
            const char* WindowName;
        };

        L1(const cv::Mat& original):m_original(original), m_ImgSize(original.size())
        {
            m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8U);
            m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_BackGround = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            m_MakeFaceMask();
        }

        static void SmoothenFace(int pos, void* smoothenFaceInstance)
        {
            static cv::Mat tmp(ms_Height, ms_Width, CV_8UC3);
            FaceSmoothenConfig* cfg = static_cast<FaceSmoothenConfig*>(smoothenFaceInstance);
            tmp = cfg->Smoothener_ptr->m_OriginalFace;
            cv::GaussianBlur(tmp, tmp, cv::Size(10, 10), pos, pos);
            cv::imshow(cfg->WindowName, tmp);
            cv::waitKey();
        }

        inline FaceSmoothenConfig GetConfig(const char* windowName)
        {
            ms_Height = m_ImgSize.height;
            ms_Width = m_ImgSize.width;
            return FaceSmoothenConfig{std::shared_ptr<L1>(this), windowName};
        }

    private:
        void m_MakeFaceMask()
        {
            cv::Mat imgHsv(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
            cv::cvtColor(m_original, imgHsv, cv::COLOR_BGR2HSV);
            cv::inRange(imgHsv, cv::Scalar(0, 20, 80), cv::Scalar(40, 255, 255), m_FaceMask);
            m_FaceMask /= 255;

            cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(17, 17));
            cv::erode(m_FaceMask, m_FaceMask, erosionElement);

            for (size_t pixel = 0; pixel < m_ImgSize.height * m_ImgSize.width; pixel ++){
                for (size_t chan = 0; chan < 3; chan++){
                    m_OriginalFace.data[3 * pixel + chan] = m_original.data[3 * pixel + chan] * m_FaceMask.data[pixel];
                }
            }
            for (size_t pixel = 0; pixel < m_ImgSize.height * m_ImgSize.width; pixel ++){
                for (size_t chan = 0; chan < 3; chan++){
                    m_BackGround.data[3 * pixel + chan] = m_original.data[3 * pixel + chan] * (1 - m_FaceMask.data[pixel]);
                }
            }

            cv::imshow("original", m_original);
            cv::imshow("mask", m_FaceMask * 255);
            cv::imshow("face area", m_OriginalFace);
            cv::imshow("background", m_BackGround);

            cv::waitKey();
        }

    private:
        cv::Mat m_FaceMask;
        cv::Mat m_OriginalFace;
        cv::Mat m_BackGround;
        const cv::Size2d m_ImgSize;
        const cv::Mat &m_original;
        static int ms_Width, ms_Height;

};

// void ss()
// {
//     cv::Mat a = 7 * cv::Mat::ones(3, 7, CV_8UC3);
//     cv::Mat b = 2 * cv::Mat::ones(3, 7, CV_8UC3);
//     cv::Mat c = 3 * cv::Mat::ones(3, 7, CV_8UC3);

//     const cv::Mat &ar = a;
//     c = ar.mul(b);
// }