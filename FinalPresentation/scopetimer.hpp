#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "experiment/imageshapes.hpp"

namespace my
{
    class ScopedTimer
    {
    public:
        ScopedTimer(uint32_t testCount)
        {
        }

        void StartTimer()
        {
            m_start = std::chrono::high_resolution_clock::now();
        }
            
        void EndTimer(size_t testIdx)
        {
            m_end = std::chrono::high_resolution_clock::now();
            m_TimerLog[testIdx] = std::chrono::duration_cast<std::chrono::duration<double>>(m_end - m_start).count();
        }

        void ExportToTxt(const std::string fullpath)
        {
            std::ofstream logFile = std::ofstream(fullpath);
            if(!logFile.is_open()){
                std::cout << "File creation failed, check file path: " << fullpath << std::endl;
                return;
            }
            // logFile.write((char*)m_TimerLog.data(), m_TimerLog.size() * sizeof(double));
            for(auto itr = m_TimerLog.begin(); itr != m_TimerLog.end() - 1; itr ++){
                logFile << std::fixed << std::setprecision(10) << *itr << ',';
            }
            logFile << std::fixed << std::setprecision(10) << *(m_TimerLog.end() - 1);
            logFile.flush();
            logFile.close();
            std::cout << "File ' " << fullpath << " ' has been saved." << std::endl;
        }

        void Reset()
        {
            memset(m_TimerLog.begin(), 0, sizeof(double) * TEST_COUNT);
        }

        const double GetTimerLog(size_t testNo){
            return m_TimerLog[testNo];
        }


    private:
        std::array<double, TEST_COUNT> m_TimerLog;
        std::chrono::high_resolution_clock::time_point m_start;
        std::chrono::high_resolution_clock::time_point m_end;
    };
};
