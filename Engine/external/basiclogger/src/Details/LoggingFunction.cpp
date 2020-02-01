#include "Details/GlobalLoggerInstance.h"
#include "Sinks/StdOut.h"
#include "Sinks/File.h"
#include "LoggingFunction.h"
namespace cof
{
    namespace basic_logger
    {
        Logger* s_logger;

        void InitStdOut()
        {
            cof::basic_logger::Init<cof::basic_logger::StdOut>(std::move(cof::basic_logger::SinkSettings{}));
        }

        void InitFile(cof::basic_logger::FileSinkSettings&& settings)
        {
            cof::basic_logger::Init<cof::basic_logger::File>(std::move(settings));
        }

        void Deint()
        {
            delete s_logger;
            s_logger = nullptr;
        }
    }
}
