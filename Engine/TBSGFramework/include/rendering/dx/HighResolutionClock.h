#pragma once






#include <chrono>

class HighResolutionClock
{
public:
    HighResolutionClock();

    
    
    
    
    void Tick();

    
    void Reset();

    double GetDeltaNanoseconds() const;
    double GetDeltaMicroseconds() const;
    double GetDeltaMilliseconds() const;
    double GetDeltaSeconds() const;

    double GetTotalNanoseconds() const;
    double GetTotalMicroseconds() const;
    double GetTotalMilliSeconds() const;
    double GetTotalSeconds() const;

private:
    
    std::chrono::high_resolution_clock::time_point m_T0;
    
    std::chrono::high_resolution_clock::duration m_DeltaTime;
    std::chrono::high_resolution_clock::duration m_TotalTime;
};
