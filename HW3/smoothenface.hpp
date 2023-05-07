#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"

class L1 : public CommonBase
{
public:
    static cv::Mat s_SmthResult;
public:
    L1(const cv::Mat &orig):CommonBase(orig)
    {
        s_SmthResult = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_Background = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_MakeFaceMask();
    }

protected:
    void c_SmoothenFace(int smthBarPos)
    {
        m_SmoothenBarPos = smthBarPos;
        m_Smoothness = (float)smthBarPos / 100;
        cv::GaussianBlur(m_OriginalFace, s_SmthResult, cv::Size2d(13, 13), 100, 100);
        s_SmthResult += m_Background;
        cv::addWeighted(s_SmthResult, m_Smoothness, m_Original, 1.0 - m_Smoothness, 0, s_SmthResult);
    }

private:
    int m_SmoothenBarPos;

private:
    void m_MakeFaceMask()
    {
        cv::Mat imgHsv(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        cv::cvtColor(m_Original, imgHsv, cv::COLOR_BGR2HSV);
        cv::inRange(imgHsv, cv::Scalar(0, 30, 80), cv::Scalar(40, 255, 255), m_FaceMask);
        m_FaceMask /= 255;

        cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
        cv::erode(m_FaceMask, m_FaceMask, erosionElement);
        cv::copyTo(m_Original, m_OriginalFace, m_FaceMask);
        cv::copyTo(m_Original, m_Background, 1 - m_FaceMask);
    }

private:
    float m_Smoothness;
    cv::Mat m_FaceMask;
    cv::Mat m_OriginalFace;
    cv::Mat m_Background;
};

cv::Mat L1::s_SmthResult;