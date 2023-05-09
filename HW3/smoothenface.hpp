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
        m_BlurredFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_OriginalFace = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        m_Background = cv::Mat::zeros(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        s_SmthResult = orig.clone();
        m_MakeBlurredFace();
    }

    virtual ~L1(){}

protected:
    void c_SmoothenFace(int smthBarPos)
    {
        m_SmoothenBarPos = smthBarPos;
        m_Smoothness = (float)smthBarPos / 100;
        cv::addWeighted(m_BlurredFace, m_Smoothness, m_OriginalFace, 1.0 - m_Smoothness, 0, s_SmthResult);
        s_SmthResult += m_Background;
    }

private:
    int m_SmoothenBarPos;

private:
    void m_MakeBlurredFace()
    {
        cv::Mat temp(m_ImgSize.height, m_ImgSize.width, CV_8UC3);
        cv::Mat faceMask(m_ImgSize.height, m_ImgSize.width, CV_8UC1);
        cv::cvtColor(m_Original, temp, cv::COLOR_BGR2HSV);
        cv::inRange(temp, cv::Scalar(0, 30, 80), cv::Scalar(40, 255, 255), faceMask);

        cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
        cv::erode(faceMask, faceMask, erosionElement);
        m_Original.copyTo(m_OriginalFace, faceMask);
        m_Original.copyTo(m_Background, cv::Scalar(255, 255, 255) - faceMask);
        cv::cvtColor(temp, temp, cv::COLOR_HSV2BGR);
        cv::GaussianBlur(m_Original, temp, cv::Size(13, 13), 100, 100);
        temp.copyTo(m_BlurredFace, faceMask);
    }

private:
    float m_Smoothness;
    cv::Mat m_BlurredFace;
    cv::Mat m_OriginalFace;
    cv::Mat m_Background;
};

cv::Mat L1::s_SmthResult;