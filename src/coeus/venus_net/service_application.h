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
    std::string _serviceName;
};

#endif // !__SERVICE_APPLICATION_H__
