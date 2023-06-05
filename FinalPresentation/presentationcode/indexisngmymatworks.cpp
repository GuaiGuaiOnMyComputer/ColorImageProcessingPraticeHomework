#include "../slowmat.hpp"

int main()
{
    my::SlowMat myMat = my::SlowMat(1440, 1920, 3);
    myMat[2][1400][1900] = 23;
}

