#include "../slowmat.hpp"

int main()
{
    int i = 0;
    while(i < 100000000){
        my::SlowMat m(3, 3, 3);
        i++;
    }
    return 0;
}