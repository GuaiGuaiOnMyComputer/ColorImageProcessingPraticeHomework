#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "commonbase.hpp"
#include "vector"

class L3
{
public:
    L3() : m_LineCount(0)
    {
        c_Points = std::vector<cv::Point2d>();
        c_Points.reserve(10);
    }

    void c_LeftButtonDownCbk(int x, int y)
    {
        c_Points.emplace_back(x, y);
    }

    void c_LeftButtonUpCbk(int x, int y, const cv::Mat &ivrtResult, cv::Mat &dispImg)
    {
        c_Points.emplace_back(x, y);
        cv::line(dispImg, c_Points[c_Points.size()-1], c_Points[c_Points.size()-2], cv::Scalar(0, 0, 255));
        m_LineCount += 1;
    }

protected:
    std::vector<cv::Point2d> c_Points;

private:
    int m_LineCount;

};