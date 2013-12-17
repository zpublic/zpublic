#ifndef __SERVICE_APPLICATION_H__
#define __SERVICE_APPLICATION_H__

#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Util/OptionSet.h"
#include "connection_factory.h"

class ServiceApplication
	: public Poco::Util::ServerApplication
{
public:
    ServiceApplication(const std::string& serviceName);
    virtual ~ServiceApplication();

public:
	void initialize(Poco::Util::Application& self);
	void uninitialize();
	void defineOptions(Poco::Util::OptionSet& options);
	void handleOption(const std::string& name, const std::string& value);
	int main(const std::vector<std::string>& args);
    static Poco::Logger& serviceLogger();
    static std::string formatLogString(const char* format, ...)
    {
        va_list args; 
        va_start(args, format);
        char buff[1024] = {0};
        vsprintf(buff, format, args);
        va_end(args);

        return std::string(buff);
    }

private:
	Poco::Net::SocketReactor _reactor;
    static Poco::Logger* _logger;
    std::string _serviceName;

    Poco::AutoPtr<Poco::Channel> _file_channel;
    Poco::AutoPtr<Poco::Channel> _console_channel;
};


#define __APP_LOGGER__ \
    ServiceApplication::serviceLogger()

#define __FORMAT__(fmt, ...)\
    ServiceApplication::formatLogString(fmt, __VA_ARGS__)

#define __FILE_NAME__ Poco::Path(__FILE__).getFileName().c_str()

#ifndef VENUS_DISABLE_LOGGER
#define fatal_log(fmt, ...) __APP_LOGGER__.fatal(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define error_log(fmt, ...) __APP_LOGGER__.error(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define warning_log(fmt, ...) __APP_LOGGER__.warning(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define info_log(fmt, ...) __APP_LOGGER__.information(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define debug_log(fmt, ...) __APP_LOGGER__.debug(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#define trace_log(fmt, ...) __APP_LOGGER__.trace(__FORMAT__(fmt, __VA_ARGS__), __FILE_NAME__, __LINE__)
#else
#define fatal_log(fmt, ...)
#define error_log(fmt, ...)
#define warning_log(fmt, ...)
#define info_log(fmt, ...)
#define debug_log(fmt, ...)
#define trace_log(fmt, ...)
#endif


#endif // !__SERVICE_APPLICATION_H__
