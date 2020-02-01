#pragma once
#include <chrono>
#include <functional>

namespace core
{
    class DeltaTime
    {
    public:

        DeltaTime() = default;
        DeltaTime(const DeltaTime& other) = default;
        DeltaTime(DeltaTime&& other) noexcept = default;
        DeltaTime& operator=(const DeltaTime& other) = default;
        DeltaTime& operator=(DeltaTime&& other) noexcept = default;

    public:

        void Initialize();
        void Update();
        void FrameIndependentUpdate(std::function<void()>& func);
		double GetDeltaTime() const { return deltaTime; }
        
        double GetTotalTime() const
		{
            return std::chrono::duration_cast<std::chrono::duration<double>>(lastTime - startTime).count();
		}
    public:
        double deltaTime{};

    private:
        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point lastTime{};
        double timeLeft{};
        double maxTimeStep{};
        double maxSteps{};
        int steps{};

    };
}
