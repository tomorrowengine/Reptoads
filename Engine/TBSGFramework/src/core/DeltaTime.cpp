#include "core/DeltaTime.h"
#include "core/Config.h"

namespace
{
    using ms = std::chrono::duration<float, std::milli>;
    using s = std::chrono::duration<float, std::chrono::seconds>;
}


void core::DeltaTime::Initialize()
{
    
    lastTime = std::chrono::high_resolution_clock::now();
    startTime = std::chrono::high_resolution_clock::now();
    maxTimeStep = tbsg::Config::Get().maxSteptime;
    maxSteps = tbsg::Config::Get().maxSteps;
}


void core::DeltaTime::Update()
{
    
    auto t = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<ms>(t - lastTime).count() / 1000.0;
    lastTime = t;

    steps = static_cast<int>((time / maxTimeStep) + 1);
    if (steps > maxSteps)
        steps = static_cast<int>(maxSteps);

    if (steps == 1)
    {
        deltaTime = time;
        timeLeft = 0.0;
    }
    else
    {
        deltaTime = time / steps;
        timeLeft = std::remainder(time, steps);
    }

}

void core::DeltaTime::FrameIndependentUpdate(std::function<void()>& func)
{
    
    while (steps > 0)
    {
        steps--;
        if (steps == 0)
        {
            deltaTime += timeLeft;
        }

        func();
    }

}
