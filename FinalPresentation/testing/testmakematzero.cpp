#include "../slowmat.hpp"

int main()
{
    while (true)
    {
        std::shared_ptr<my::SlowMat> m = my::SlowMat::zeros(3, 3, 3);
    }
    return 0;
}