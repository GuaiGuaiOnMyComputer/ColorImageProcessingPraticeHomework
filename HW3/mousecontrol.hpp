#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "commonbase.hpp"
#include "vector"
#include <iostream>

class L3
{
public:
    L3() : m_LineCount(0)
    {
        c_Points.reserve(10);
    }

    inline void c_AddLineStartPt(int x, int y)
    {
        c_Points.emplace_back(x, y);
    }

    inline void c_AddLineEndPt(int x, int y)
    {
        c_Points.emplace_back(x, y);
        m_LineCount++;
    }

    void c_DrawLines(const cv::Mat ivrtResult, cv::Mat &dispImg)
    {
        dispImg = ivrtResult.clone();
        for(size_t i = 0; i < m_LineCount; i++){
            cv::line(dispImg, c_Points[2*i], c_Points[2*i+1], cv::Scalar(0, 0, 255), 8, cv::LineTypes::LINE_AA);
        }
    }

protected:
    std::vector<cv::Point2d> c_Points;

private:
    int m_LineCount;

};