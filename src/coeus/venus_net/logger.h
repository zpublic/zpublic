#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "singleton.h"
#include "Poco/Logger.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/Path.h"
#include "Poco/AutoPtr.h"

class Logger
    : public Venus::Singleton<Logger>
{
public:
    Logger() 
        : _fileChannel(new Poco::FileChannel), _consoleChannel(new Poco::ConsoleChannel)
    {
    }

public:
    bool initialize(Poco::Logger& logger)
    {
        _logger = &logger;

        _logger->setLevel("trace");

        _fileChannel->setProperty("path", "server.log");
        _fileChannel->setProperty("rotation", "2 M");
        _fileChannel->setProperty("archive", "timestamp");
        _fileChannel->setProperty("compress", "true");
        _fileChannel->setProperty("flush", "false");

        Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter());  
        patternFormatter->setProperty("pattern", "[%Y-%m-%d %H:%M:%S.%i %U:%u] %p : %t");

        Poco::AutoPtr<Poco::Channel> fileChannel(new Poco::FormattingChannel(patternFormatter, _fileChannel));
        Poco::AutoPtr<Poco::Channel> consleChannel(new Poco::FormattingChannel(patternFormatter, _consoleChannel));

        Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);
        //splitterChannel->addChannel(fileChannel);
        splitterChannel->addChannel(consleChannel);
        _logger->setChannel(splitterChannel);

        return true;
    }

    void fatal(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->fatal(log, filename.c_str(), line);
    }

    void error(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->error(log, filename.c_str(), line);
    }

    void warning(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->warning(log, filename.c_str(), line);
    }

    void information(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->information(log, filename.c_str(), line);
    }

    void debug(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->debug(log, filename.c_str(), line);
    }

    void trace(const std::string& log, const std::string& filename, int line)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        _logger->trace(log, filename.c_str(), line);
    }

    std::string formatLogString(const char* format, ...)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);

        va_list args; 
        va_start(args, format);
        char buff[1024] = {0};
        vsprintf(buff, format, args);
        va_end(args);

        return buff;
    }

    std::string filename(const std::string& file)
    {
        Poco::ScopedLock<Poco::FastMutex> lock(_mutex);
        return std::move(Poco::Path(file).getFileName());
    }

private:
    Poco::Logger* _logger;
    Poco::AutoPtr<Poco::Channel> _fileChannel;
    Poco::AutoPtr<Poco::Channel> _consoleChannel;
    Poco::FastMutex _mutex;
};


#define __APP_LOGGER__ \
    Logger::getInstance()

#define __FORMAT__(fmt, ...)\
    Logger::getInstance().formatLogString(fmt, __VA_ARGS__)

#define __FILE_NAME__ Logger::getInstance().filename(__FILE__)

#ifdef VENUS_DISABLE_LOGGER
#define fatal_log(fmt, ...)
#define error_log(fmt, ...)
#define warning_log(fmt, ...)
#define info_log(fmt, ...)
#define debug_log(fmt, ...)
#define trace_log(fmt, ...)
#else
#define fatal_log(fmt, ...) __APP_LOGGER__.fatal(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define error_log(fmt, ...) __APP_LOGGER__.error(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define warning_log(fmt, ...) __APP_LOGGER__.warning(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define info_log(fmt, ...) __APP_LOGGER__.information(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define debug_log(fmt, ...) __APP_LOGGER__.debug(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define trace_log(fmt, ...) __APP_LOGGER__.trace(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#endif


#endif // !__LOGGER_H__
