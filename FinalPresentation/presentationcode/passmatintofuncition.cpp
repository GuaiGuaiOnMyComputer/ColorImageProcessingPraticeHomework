#include <opencv2/imgcodecs.hpp>
#include "../slowmat.hpp"

void alterValue(my::SlowMat _myMat, cv::Mat _cvMat, int _x, int _y, int _channel, uint8_t val)
{
    _cvMat.ptr(_y)[_x] = val;
    _myMat[_channel][_x][_y] = val;
}

int main()
{
    my::SlowMat myMat = my::SlowMat(1024, 768, 3);
    cv::Mat cvMat = cv::Mat(1024, 768, CV_8UC3);
    alterValue(myMat, cvMat, 20, 30, 1, 88);
    myMat[1][30][20] = 89;
    return 0;
}