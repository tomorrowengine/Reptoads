#pragma once

#include "Sinks/Sink.h"

namespace cof
{
    namespace basic_logger
    {
        
        struct FileSinkSettings : SinkSettings
        {
            FileSinkSettings() :FileStr{} {};
            FileSinkSettings(const char * FileStr) :FileStr{ FileStr } {};
            FileSinkSettings(FileSinkSettings&& other) noexcept : SinkSettings(std::move(other))
            {
                FileStr = other.FileStr;
            }
            std::string FileStr {};
            std::string FileMode { "a+" };
        };
        
        class File : public Sink
        {
        public:
            File(SinkSettings* settings);
            ~File();
        protected:
            virtual void Process(Level lvl, fmt::memory_buffer&& buffer) override final;
        private:
            
            FileSinkSettings settings_;
            FILE* file_;
        };
    };
};
