#include "../slowmat.hpp"
#include "../laplacianfilter.hpp"
#include "../laplacianfilter.hpp"
#include <memory.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define PATH_TO_PROFESSORS_FACE "FinalPresentation/image/Sun_oil.jpg"

int main()
{
    cv::Mat cvMat = cv::imread(PATH_TO_PROFESSORS_FACE);
    std::shared_ptr<my::SlowMat> myMat = my::SlowMat::FromCvMat(cvMat);
    cv::Laplacian(cvMat, cvMat, -1, 5);
    cv::imshow("Professor's face laplacianed!", cvMat);
    cv::waitKey();
    myMat->Convolution(my::LapFilter::LAP_FILTER);
    cv::Mat converted = myMat->ToCvMat();
    cv::imshow("Professor's face laplacianed!", converted);
    cv::waitKey();
    return 0;
}