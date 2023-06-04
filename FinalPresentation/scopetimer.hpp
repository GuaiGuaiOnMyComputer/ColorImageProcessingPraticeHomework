#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

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

        void ExportToTxt(const std::string fullpath)
        {
            std::ofstream logFile = std::ofstream(fullpath);
            if(!logFile.is_open()){
                std::cout << "File creation failed, check file path: " << fullpath << std::endl;
                return;
            }
            logFile.write((char*)m_TimerLog.data(), m_TimerLog.size() * sizeof(double));
            logFile.flush();
            logFile.close();
            std::cout << "File ' " << fullpath << " ' has been saved." << std::endl;
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
