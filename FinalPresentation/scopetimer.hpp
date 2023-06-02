#include <vector>
#include <chrono>

namespace my
{
    class ScopedTimer
    {
    public:
        ScopedTimer(uint32_t testCount)
        {
            m_TimerLog.reserve(testCount);
        }

        void StartTimer()
        {
            m_start = std::chrono::high_resolution_clock::now();
        }
            
        void EndTimer()
        {
            m_end = std::chrono::high_resolution_clock::now();
            m_TimerLog.push_back(std::chrono::duration_cast<std::chrono::duration<double>>(m_end - m_start).count());
        }

        const double GetTimerLog(size_t testNo){
            return m_TimerLog[testNo];
        }


    private:
        std::vector<double> m_TimerLog;
        std::chrono::high_resolution_clock::time_point m_start;
        std::chrono::high_resolution_clock::time_point m_end;
    };
};
