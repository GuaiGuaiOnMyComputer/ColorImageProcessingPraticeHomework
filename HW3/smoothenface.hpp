#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"

class L1 : public CommonBase
{
public:
    cv::Mat s_SmthReult;
public:
    L1(const cv::Mat &orig):CommonBase(orig)
    {
        s_SmthReult = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_FaceMask = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_Background = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_MakeFaceMask();
    }
    struct SmthCfg
    {
        L1 &Smoothen_ref;
        const char* WindowName;
        cv::Mat &dispImg_ref;
    };

    static void SmoothenCbk(int pos, void* smoothenerStruct)
    {
        float smoothness = (float)pos / 100;
        L1* cfg_ptr = (L1*)(smoothenerStruct);
        cv::GaussianBlur(cfg_ptr->m_OriginalFace, s_SmthResult, cv::Size(13, 13), 100, 100);
        s_SmthResult += cfg_ptr->m_Background;
        cv::addWeighted(s_SmthResult, smoothness, cfg_ptr->CommonBase::m_Original, 1 - smoothness, 0, cfg_ptr->s_SmthResult);
    }


protected:
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

protected:
    static cv::Mat s_SmthResult;

private:
    cv::Mat m_FaceMask;
    cv::Mat m_OriginalFace;
    cv::Mat m_Background;
};

cv::Mat L1::s_SmthResult;