#pragma once

#include <chrono>

namespace LifeExe::App
{
/// Measures elapsed time between successive ticks.
class Timer
{
public:
    /// Creates a timer starting at the current time.
    Timer() : m_lastTime{std::chrono::steady_clock::now()} {}

    /// Returns the elapsed time in seconds since the previous tick.
    float tick()
    {
        const auto currentTime{std::chrono::steady_clock::now()};
        const std::chrono::duration<float> dt{currentTime - m_lastTime};
        m_lastTime = currentTime;
        return dt.count();
    }

    /// Resets the timer's reference point to the current time.
    void reset() { m_lastTime = std::chrono::steady_clock::now(); }

private:
    std::chrono::steady_clock::time_point m_lastTime;
};
}  // namespace LifeExe::App