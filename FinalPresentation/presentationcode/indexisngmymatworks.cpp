#include "../slowmat.hpp"

int main()
{
    // create an instance of my::SlowMat having size 1440x1920 
    // and 3 channels
    my::SlowMat myMat = my::SlowMat(1440, 1920, 3);
    myMat[2][1400][1900] = 23; // index its data using custom [] operators
}

