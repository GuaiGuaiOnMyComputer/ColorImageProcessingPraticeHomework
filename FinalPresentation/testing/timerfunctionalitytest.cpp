#include "../scopetimer.hpp"
#include <iostream>
#include <iomanip>

int main()
{
    my::ScopedTimer tmr(10000);
    for (size_t i = 0; i < 10000; i ++){
        tmr.StartTimer();
        std::cout << "   " << i << std::endl;
        tmr.EndTimer();
        std::cout << std::setprecision(19) << "Round " << i << " took " << tmr.GetTimerLog(i) << " secs" << std::endl;
    }
}