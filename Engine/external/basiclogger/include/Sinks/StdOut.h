#pragma once

#include "Sinks/Sink.h"

namespace cof
{
    namespace basic_logger
    {
        
        class StdOut : public Sink
        {
        public:
            StdOut(SinkSettings* settings) { delete settings; }
        protected:
            
            void Process(Level lvl, fmt::memory_buffer&& message) override;
        };
    }
}
