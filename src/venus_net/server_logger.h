#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/LogStream.h>
#include <Poco/Path.h>
#include "singleton.h"
#include <mutex>

class ServerLogger
{
public:
    ServerLogger(const std::string& loggerName);
    virtual ~ServerLogger();

public:
    Poco::Logger& getLogger();
    Poco::LogStream& getLogStream();

private:
    std::string _loggerName;
    Poco::LogStream* _log_stream;
    static Poco::Logger* _logger;
};

class FileChannelLogger
    : public Venus::Singleton<FileChannelLogger>, public ServerLogger
{
public:
    enum LoggerPriority
    {
        PRIO_FATAL = 1,   /// A fatal error. The application will most likely terminate. This is the highest priority.
        PRIO_ERROR,       /// An error. An operation did not complete successfully, but the application as a whole is not affected.
        PRIO_WARNING,     /// A warning. An operation completed with an unexpected result.
        PRIO_INFORMATION, /// An informational message, usually denoting the successful completion of an operation.
        PRIO_DEBUG,       /// A debugging message.
        PRIO_TRACE        /// A tracing message. This is the lowest priority.
    };

public:
    FileChannelLogger();
    FileChannelLogger(const std::string& path);
    virtual ~FileChannelLogger();

public:
    bool initLogger(const std::string& path);
    std::string format_log(char* format, ...);
    
private:
    Poco::AutoPtr<Poco::Channel> _file_channel;
    Poco::AutoPtr<Poco::Channel> _console_channel;
};

//logger
#define __G_LOGGER__ \
    FileChannelLogger::getInstance().getLogger() \

#define __FORMAT__(fmt, ...) \
    FileChannelLogger::getInstance().format_log(fmt, __VA_ARGS__)

#define __FILE_NAME__ Poco::Path(__FILE__).getFileName().c_str()

#define fatal_log(fmt, ...) \
    __G_LOGGER__.fatal(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)

#define error_log(fmt, ...) \
    __G_LOGGER__.error(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)

#define warning_log(fmt, ...) \
    __G_LOGGER__.warning(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)

#define info_log(fmt, ...) \
    __G_LOGGER__.information(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)

#define debug_log(fmt, ...) \
    __G_LOGGER__.debug(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)

#define trace_log(fmt, ...) \
    __G_LOGGER__.trace(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)


#endif // !__LOGGER_H__
