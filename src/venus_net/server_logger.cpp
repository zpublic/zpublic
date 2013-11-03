#include "server_logger.h"
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/SplitterChannel.h>

Poco::Logger* ServerLogger::_logger;
ServerLogger::ServerLogger(const std::string& loggerName)
    : _loggerName(loggerName), _log_stream(NULL)
{
    _logger = &Poco::Logger::get(_loggerName);
    _logger->setLevel(Poco::Message::PRIO_TRACE);
    _log_stream = new Poco::LogStream(*_logger);
}

ServerLogger::~ServerLogger()
{
    if (_log_stream != nullptr)
    {
        delete _log_stream;
        _log_stream = nullptr; 
    }
}

Poco::Logger& ServerLogger::getLogger()
{
    return *_logger;
}

Poco::LogStream& ServerLogger::getLogStream()
{
    return *_log_stream;
}

FileChannelLogger::FileChannelLogger()
    : ServerLogger("SERVER_LOGGER"), 
    _file_channel(new Poco::FileChannel),
    _console_channel(new Poco::ConsoleChannel)
{
}

FileChannelLogger::~FileChannelLogger()
{

}

std::string FileChannelLogger::format_log(char* format, ...)
{
    va_list args; 
    va_start(args, format);
    char buff[1024] = {0};
    _vsnprintf(buff, 1024, format, args);
    va_end(args);

    return buff;
}

bool FileChannelLogger::initLogger(const std::string& path)
{
    _file_channel->setProperty("path", path);
    _file_channel->setProperty("rotation", "2 M");
    _file_channel->setProperty("archive", "timestamp");
    _file_channel->setProperty("compress", "true");

    //日志格式化
    Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter());  
    patternFormatter->setProperty("pattern", "[%Y-%m-%d %H:%M:%S.%i %U:%u] %p : %t");

    Poco::AutoPtr<Poco::Channel> file_channel(new Poco::FormattingChannel(patternFormatter, _file_channel));
    Poco::AutoPtr<Poco::Channel> consle_channel(new Poco::FormattingChannel(patternFormatter, _console_channel));

    //创建通道分发器
    Poco::AutoPtr<Poco::SplitterChannel> splitter_channel(new Poco::SplitterChannel);
    splitter_channel->addChannel(file_channel);
    splitter_channel->addChannel(consle_channel);

    Poco::Logger& logger = getLogger();
    logger.setChannel(splitter_channel);

    return true;
}