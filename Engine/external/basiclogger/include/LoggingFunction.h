#pragma once

#include <cassert>
#include <type_traits>
#include "BasicLogger.h"
#include "Details/GlobalLoggerInstance.h"
namespace cof
{
namespace basic_logger
{
    
class StdOut;
class File;
struct FileSinkSettings;


template <typename S, typename SS>
void Init(SS&& settings)
{
    static_assert(std::is_base_of_v<Sink, S>, "Error: The used Sink is not compatible with the Logger! It needs to be of the type cof::Sink");
    if (!s_logger)
    {
        s_logger = new Logger(new S{new SS{std::forward<SS>(settings)}});
    }
    else
    {
        
        assert(false);
    }
};


    void InitStdOut();

    void InitFile(cof::basic_logger::FileSinkSettings&& settings);

    void Deint();
}

#define COF_CHECK_IF_LOGGER_IS_VALID() \
    if (!basic_logger::s_logger)       \
        assert(false);



template <typename... Args>
void Log(const char* format, const Args&... args)
{
    if (cof::basic_logger::g_enabled)
    {
        COF_CHECK_IF_LOGGER_IS_VALID()
        basic_logger::s_logger->Log(format, args...);
    }
}

template <typename... Args>
void Debug(const char* format, const Args&... args)
{
    if (cof::basic_logger::g_enabled)
    {
        COF_CHECK_IF_LOGGER_IS_VALID()
        basic_logger::s_logger->Debug(format, args...);
    }
}

template <typename... Args>
void Warn(const char* format, const Args&... args)
{
    if (cof::basic_logger::g_enabled)
    {
        COF_CHECK_IF_LOGGER_IS_VALID()
        basic_logger::s_logger->Warn(format, args...);
    }
}

template <typename... Args>
void Info(const char* format, const Args&... args)
{
    if (cof::basic_logger::g_enabled)
    {
        COF_CHECK_IF_LOGGER_IS_VALID()
        basic_logger::s_logger->Info(format, args...);
    }
}

template <typename... Args>
void Error(const char* format, const Args&... args)
{
    if (cof::basic_logger::g_enabled)
    {
        COF_CHECK_IF_LOGGER_IS_VALID()
        basic_logger::s_logger->Error(format, args...);
    }
}

#undef COF_CHECK_IF_LOGGER_IS_VALID
}
