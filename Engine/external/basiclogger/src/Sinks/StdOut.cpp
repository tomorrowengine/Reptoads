#include "Sinks/StdOut.h"

#if defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_) || defined(DEBUG_) || defined(_DEBUG) || defined(COF_USE_LOGGER)
#include "fmt/format.h"
#endif

void cof::basic_logger::StdOut::Process(Level lvl, fmt::memory_buffer&& buffer)
{
    if constexpr (g_enabled) {
        auto* stdOut = (lvl == Level::ERR) ? stderr : stdout;
        fwrite(buffer.data(), sizeof(char), buffer.size(), stdOut);
        fflush(stdOut);
    }
}
