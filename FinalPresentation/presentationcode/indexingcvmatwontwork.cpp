#include <opencv2/opencv.hpp>

#define FILENAME "blah/blah/blan"

int main()
{
    cv::Mat myMat = cv::Mat::zeros(1440, 1920, CV_8UC3);
    myMat[100][100][2] = 23;
    return 0;
}

int main()
{
    cv::Mat myMat = cv::Mat::zeros(1440, 1920, CV_8UC3);
    myMat.ptr(100)[100 + 2] = 23;
    return 0;
}

int main()
{
    cv::Mat myMat = cv::Mat::zeros(1440, 1920, CV_8UC3);
    myMat.data[myMat.channels() * (100 + 100) + 2] = 23;
    return 0;
}

