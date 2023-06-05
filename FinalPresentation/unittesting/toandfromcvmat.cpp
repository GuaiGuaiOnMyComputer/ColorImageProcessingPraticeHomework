#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <memory.h>
#include "../slowmat.hpp"

int main()
{
    cv::Mat fastMat = cv::imread("FinalPresentation/image/dotdatakindofworks.png");
    std::shared_ptr<my::SlowMat> myMat = my::SlowMat::FromCvMat(fastMat);
    cv::Mat converted = myMat->ToCvMat();
    cv::imshow("Frame", converted);
    cv::waitKey();

    return 0;
}