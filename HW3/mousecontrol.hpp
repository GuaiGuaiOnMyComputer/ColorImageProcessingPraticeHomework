#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "commonbase.hpp"
#include "vector"

class L3
{
public:
    L3():m_HoldingLeftButton(false)
    {}

    void c_LeftButtonDownCbk(int x, int y, const cv::Mat &ivrtResult, cv::Mat &dispImg)
    {
        m_HoldingLeftButton = true;
        c_Points.emplace_back(x, y);
    }

    void c_LeftButtonUpCbk(int x, int y, const cv::Mat &ivrtResult, cv::Mat &dispImg)
    {
        m_HoldingLeftButton = false;
        c_Points.emplace_back(x, y);
    }

protected:
    std::vector<cv::Point2d> c_Points;

private:
    bool m_HoldingLeftButton;

};