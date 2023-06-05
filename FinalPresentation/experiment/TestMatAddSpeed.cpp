#include <memory.h>
#include <iostream>
#include <random>
#include "../slowmat.hpp"
#include "../scopetimer.hpp"
#include "../laplacianfilter.hpp"
#include "imageshapes.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#define TEST_COUNT 5000

int main()
{
    // Test cv::Mat zeros consturction time
    my::ScopedTimer cvMatTimer(TEST_COUNT);
    my::ScopedTimer slowMatTimer(TEST_COUNT);
    std::mt19937 randGen(time(0));

    for(int shapeIdx; shapeIdx < SHAPE_OPT_COUNT; shapeIdx ++){
        cv::Mat cvMatResult = cv::Mat::zeros(g_IMAGESHAPES[shapeIdx].width, g_IMAGESHAPES[shapeIdx].height, CV_8UC3);
        cv::Mat cvMat = cv::Mat::zeros(g_IMAGESHAPES[shapeIdx].width, g_IMAGESHAPES[shapeIdx].height, CV_8UC3);
        std::shared_ptr<my::SlowMat> myMat = my::SlowMat::FromCvMat(cvMat);
        for (int i = 0; i < TEST_COUNT; i ++){
            cv::randu(cvMat, 0, 255);
            cvMatTimer.StartTimer();
            cvMatResult = cvMat + cvMat;
            cvMatTimer.EndTimer(i);
            cvMatResult.ptr()[randGen() % (cvMat.cols * cvMat.rows)] = randGen();

            slowMatTimer.StartTimer();
            myMat->Add(*myMat);
            slowMatTimer.EndTimer(i);
            myMat->AlterAnElement(randGen(), randGen());
        }
        char fNameBuffer[500];
        const char fileDir[] = ".\\FinalPresentation\\experiment\\log\\additiontime\\";
        sprintf(fNameBuffer, "%scv_shape%dx%d_testsize%d.txt", fileDir, g_IMAGESHAPES[shapeIdx].height, g_IMAGESHAPES[shapeIdx].width, TEST_COUNT);
        cvMatTimer.ExportToTxt(fNameBuffer);
        memset(fNameBuffer, 0, 500);
        sprintf(fNameBuffer, "%smy_shape%dx%d_testsize%d.txt", fileDir, g_IMAGESHAPES[shapeIdx].height, g_IMAGESHAPES[shapeIdx].width, TEST_COUNT);
        slowMatTimer.ExportToTxt(fNameBuffer);

        slowMatTimer.Reset();
        cvMatTimer.Reset();
    }
    std::cout << "Performance testing done!" << std::endl;
    return 0;
}