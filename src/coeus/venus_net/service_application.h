#ifndef __SERVICE_APPLICATION_H__
#define __SERVICE_APPLICATION_H__

#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Util/OptionSet.h"
#include "connection_factory.h"

class MessageDispatcher;

namespace Venus
{
class ServiceApplication
	: public Poco::Util::ServerApplication
{
public:
    ServiceApplication(const std::string& serviceName, MessageDispatcher* messageDispatcher);
    virtual ~ServiceApplication();

public:
	int main(const std::vector<std::string>& args);
    virtual void defineOptions(Poco::Util::OptionSet& options);
    virtual void handleOption(const std::string& name, const std::string& value);

private:
    void initialize(Poco::Util::Application& self);
    void uninitialize();

private:
	Poco::Net::SocketReactor _reactor;
    MessageQueue _messageQueue;
    std::string _serviceName;
};

}

#endif // !__SERVICE_APPLICATION_H__
