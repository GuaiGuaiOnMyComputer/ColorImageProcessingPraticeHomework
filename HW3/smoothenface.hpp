#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


class L1
{
    public:
        L1(const cv::Mat& original):m_original(original), m_ImgSize(original.size())
        {
            m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8U);
            m_MakeFaceMask();
        }

        void static SmoothenFace(const cv::Mat& original)
        {
        }

    private:
        void m_MakeFaceMask()
        {
            cv::Mat imgHsv(m_original);
            cv::cvtColor(imgHsv, imgHsv, cv::COLOR_BGR2HSV);
            cv::inRange(imgHsv, cv::Scalar(0, 20, 80), cv::Scalar(40, 255, 255), m_FaceMask);
        }

    private:
        cv::Mat m_FaceMask;
        const cv::Size2d m_ImgSize;
        const cv::Mat &m_original;
};
