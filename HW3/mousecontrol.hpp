#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "commonbase.hpp"
#include "vector"
#include <iostream>

/*
    This class handles the drawing functionalities of the program. All the member methods are called from class Renderer
*/

class L3
{
public:
    L3() : m_LineCount(0)
    {
        m_Points.reserve(10);
    }

    inline void c_AddLineStartPt(int x, int y)
    {
        m_Points.emplace_back(x, y);
    }

    /*
        Draws all the line to s_DisplayImg. Each pair of cv::Point2d in c_Points are treated as the start point and the end
        point of a line.
    */
    void c_DrawLines(const cv::Mat ivrtResult, cv::Mat &dispImg)
    {
        dispImg = ivrtResult.clone();
        for(size_t i = 0; i < m_Points.size() - 1 && m_Points.size() > 1; i++)
            cv::line(dispImg, m_Points[i+1], m_Points[i], cv::Scalar(0, 0, 255), 8, cv::LineTypes::LINE_AA);
    }

protected:
    void c_DisposePoints()
    {
        m_Points.clear();
        m_LineCount = 0;
    }

private:
    std::vector<cv::Point2d> m_Points; // stores the coordinates of all the points created by user
    int m_LineCount;
};