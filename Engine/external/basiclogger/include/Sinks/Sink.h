#pragma once

#include <array>
#include <string>
#include "fmt/format.h"




namespace cof
{
namespace basic_logger
{

enum class Level
{
    LOG,
    INFO,
    DEBUG,
    ERR,
    WARN
};

size_t inline GetLevel(const Level& level)
{
    return static_cast<size_t>(level);
}

#if defined(COF_USE_LOGGER) || defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_) || defined(DEBUG_) || defined(_DEBUG)
constexpr bool g_enabled = true;
#else

constexpr bool g_enabled = false;
#endif

struct SinkSettings
{
    SinkSettings() = default;
    SinkSettings(SinkSettings&& ) noexcept {}
    virtual ~SinkSettings() {};
};

class Sink
{
   public:

    virtual ~Sink() = default;
    
    void SinkIn(Level lvl, std::string&& message);

   protected:
    virtual void Process(Level lvl, fmt::memory_buffer&& message) = 0;
    std::string GetFormattedDate();
	std::array<const char*, 5> levels_{ {"LOG", "INFO", "DEBUG", "ERR", "WARN"} };
};
}
}
