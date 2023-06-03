#include "../slowmat.hpp"
#include <iostream>

void fn(my::SlowMat _m)
{
    std::cout << _m.cols << std::endl;
}

int main()
{
    while (true)
    {
        std::shared_ptr<my::SlowMat> m = my::SlowMat::zeros(3, 3, 3);
    }
    return 0;
}