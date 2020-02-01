#include "Sinks/File.h"

#if defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_) || defined(DEBUG_) || defined(_DEBUG) || defined(COF_USE_LOGGER)

#include "fmt/format.h"

#endif

cof::basic_logger::File::File(SinkSettings* settings) : settings_{std::move(*dynamic_cast<FileSinkSettings*>(settings))}, file_(nullptr)
{
#ifdef _WIN32
    
    file_ = _fsopen((settings_.FileStr.c_str()), settings_.FileMode.c_str(), _SH_DENYNO);
#else 
file_ = fopen((settings_.FileStr.c_str()), settings_.FileMode.c_str());
#endif
delete settings;
}

cof::basic_logger::File::~File()
{
    fclose(file_);
}

void cof::basic_logger::File::Process(Level lvl, fmt::memory_buffer&& buffer)
{
    if constexpr (g_enabled)
    {
        fwrite(buffer.data(), sizeof(char), buffer.size(), file_);
        fflush(file_);
    }
}
