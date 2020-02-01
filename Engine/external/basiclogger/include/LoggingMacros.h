#pragma once



#define LOGGER_INIT(SinkType, Settings) cof::basic_logger::Init<SinkType>(std::move(Settings));

#define LOGGER_INIT_STDOUT() cof::basic_logger::Init<cof::basic_logger::StdOut>(cof::basic_logger::SinkSettings{});

#define LOGGER_INIT_FILE(settings) cof::basic_logger::Init<cof::basic_logger::File>(std::move(settings));

#define LOGGER_DEINIT cof::logger::Deint();


#ifdef COF_LOGGER_USE_DATE_LINE

#define FORMAT(format) (std::string("File:{} Line:{} \nMessage:") + format).c_str(), __FILE__, __LINE__
#else

#define FORMAT(format) format
#endif


#define LOG(format, ...) cof::Log(FORMAT(format), __VA_ARGS__);

#define WARN(format, ...) cof::Warn(FORMAT(format), __VA_ARGS__);

#define ERR(format, ...) cof::Error(FORMAT(format), __VA_ARGS__);

#define DEBUG(format, ...) cof::Debug(FORMAT(format), __VA_ARGS__);

#define INFO(format, ...) cof::Info(FORMAT(format), __VA_ARGS__);
