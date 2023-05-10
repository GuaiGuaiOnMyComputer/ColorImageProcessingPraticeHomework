#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "commonbase.hpp"

/*
    L1 inherits from CommonBase, handling the smoothening functionality of the program. Member method 
    c_SmoothenFace is called from class Renderer. The smoothend face is created upon class construction,
    Use the smoothen face trackbar to adjust the weight of the smoothened face and the original face.
*/

class L1 : public CommonBase
{
public:
    static cv::Mat s_SmthResult;
public:
    L1(const cv::Mat &orig):CommonBase(orig)
    {
        m_BlurredFace = cv::Mat::zeros(c_ImgSize.height, c_ImgSize.width, CV_8UC3);
        m_OriginalFace = cv::Mat::zeros(c_ImgSize.height, c_ImgSize.width, CV_8UC3);
        m_Background = cv::Mat::zeros(c_ImgSize.height, c_ImgSize.width, CV_8UC3);
        s_SmthResult = orig.clone();
        m_MakeBlurredFace();
    }

    virtual ~L1(){}

protected:
    void c_SmoothenFace(int smthBarPos)
    {
        m_Smoothness = (float)smthBarPos / 100;
        cv::addWeighted(m_BlurredFace, m_Smoothness, m_OriginalFace, 1.0 - m_Smoothness, 0, s_SmthResult);
        s_SmthResult += m_Background;
    }


private:
    void m_MakeBlurredFace()
    {
        cv::Mat temp(c_ImgSize.height, c_ImgSize.width, CV_8UC3);     // the original image converted into HSV space
        cv::Mat faceMask(c_ImgSize.height, c_ImgSize.width, CV_8UC1); // the mask used to extract face area from the orignal image
        cv::cvtColor(c_Original, temp, cv::COLOR_BGR2HSV);
        cv::inRange(temp, cv::Scalar(0, 30, 80), cv::Scalar(40, 255, 255), faceMask); // create mask

        cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
        cv::erode(faceMask, faceMask, erosionElement);
        c_Original.copyTo(m_OriginalFace, faceMask); // extract the original face area using the mask
        c_Original.copyTo(m_Background, cv::Scalar(255, 255, 255) - faceMask); // extract the background area of the orignal image using the mask
        cv::cvtColor(temp, temp, cv::COLOR_HSV2BGR);
        cv::GaussianBlur(c_Original, temp, cv::Size(13, 13), 100, 100);
        temp.copyTo(m_BlurredFace, faceMask); // extract the blurred face using the mask
    }

private:
    float m_Smoothness;     // the weight of the blurred face in s_SmthResult
    cv::Mat m_BlurredFace;  // the face area of the original image blurred using Gaussian filter, excluding background
    cv::Mat m_OriginalFace; // the face area of the original image, excluding background
    cv::Mat m_Background;   // the original background, excluding face
};

cv::Mat L1::s_SmthResult;