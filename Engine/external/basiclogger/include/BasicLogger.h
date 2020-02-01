#pragma once

#include "Sinks/Sink.h"
#include "fmt/format.h"

#include <string>
#include <memory>

namespace cof
{

namespace basic_logger
{
#if defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_) || defined(DEBUG_) || defined(_DEBUG) || defined(COF_USE_LOGGER)

class Logger
{
   public:
    Logger() = delete;
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;

    Logger(Logger&& other) noexcept : sink_(std::move(other.sink_)) {}

    Logger& operator=(Logger&& other) noexcept
    {
        sink_ = std::move(other.sink_);

        return *this;
    }

    
    Logger(basic_logger::Sink* sink) : sink_(sink) {}
    
    template <typename... Args>
    void Log(const char* formStr, const Args&... args)
    {
        sink_->SinkIn(Level::LOG, std::move(fmt::vformat(formStr, fmt::make_format_args(args...))));
    }
    
    template <typename... Args>
    void Warn(const char* formStr, const Args&... args)
    {
        sink_->SinkIn(Level::WARN, std::move(fmt::vformat(formStr, fmt::make_format_args(args...))));
    }
    
    template <typename... Args>
    void Debug(const char* formStr, const Args&... args)
    {
        sink_->SinkIn(Level::DEBUG, std::move(fmt::vformat(formStr, fmt::make_format_args(args...))));
    }
    
    template <typename... Args>
    void Info(const char* formStr, const Args&... args)
    {
        sink_->SinkIn(Level::INFO, std::move(fmt::vformat(formStr, fmt::make_format_args(args...))));
    }
    
    template <typename... Args>
    void Error(const char* formStr, const Args&... args)
    {
        sink_->SinkIn(Level::ERR, std::move(fmt::vformat(formStr, fmt::make_format_args(args...))));
    }

   private:
    std::unique_ptr<basic_logger::Sink> sink_;
};
#else
class Logger
{
   public:
    Logger() = delete;

    Logger(const Logger& other) = default;

    Logger(Logger&& other) = default;
    Logger& operator=(const Logger& other) = default;

    Logger& operator=(Logger&& other) noexcept = default;

    Logger(basic_logger::Sink* sink) {}
    ~Logger() = default;

    template <typename... Args>
    void Log(const char* formStr, const Args&... args)
    {
    }
    template <typename... Args>
    void Warn(const char* formStr, const Args&... args)
    {
    }
    template <typename... Args>
    void Debug(const char* formStr, const Args&... args)
    {
    }
    template <typename... Args>
    void Info(const char* formStr, const Args&... args)
    {
    }
    template <typename... Args>
    void Error(const char* formStr, const Args&... args)
    {
    }
};
#endif
};
}
