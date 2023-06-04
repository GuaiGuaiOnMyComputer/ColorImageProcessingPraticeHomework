#include "../scopetimer.hpp"

int main()
{
    my::ScopedTimer tmr(5000);
    for(int i = 0; i < 1000; i++){
        tmr.StartTimer();
        tmr.EndTimer(i);
    }
    tmr.ExportToTxt("blah");
}