#include <memory.h>
#include <iostream>
#include "../slowmat.hpp"
#include "../scopetimer.hpp"
#include "imageshapes.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

int main()
{
    // Test cv::Mat zeros consturction time
    my::ScopedTimer cvMatTimer(TEST_COUNT);
    my::ScopedTimer slowMatTimer(TEST_COUNT);

    for(int shapeIdx; shapeIdx < 4; shapeIdx ++){
        for (int i = 0; i < TEST_COUNT; i ++){
            cvMatTimer.StartTimer();
            cv::Mat cvMat = cv::Mat::zeros(g_IMAGESHAPES[shapeIdx].height, g_IMAGESHAPES[shapeIdx].width, CV_8UC3);
            cvMatTimer.EndTimer(i);
            std::cout << "cv::Mat rows = " << cvMat.rows << "\n"; 

            slowMatTimer.StartTimer();
            std::shared_ptr<my::SlowMat> myMat = my::SlowMat::zeros(g_IMAGESHAPES[shapeIdx].height, g_IMAGESHAPES[shapeIdx].width, 3);
            slowMatTimer.EndTimer(i);
            std::cout << "cv::Mat rows = " << myMat->rows << "\n"; 
        }
        char fNameBuffer[500];
        const char fileDir[] = ".\\FinalPresentation\\experiment\\log\\creationtime\\";
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