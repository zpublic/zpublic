#ifndef __SERVICE_APPLICATION_H__
#define __SERVICE_APPLICATION_H__

#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Util/OptionSet.h"

class ServiceApplication
	: public Poco::Util::ServerApplication
{
public:
	void initialize(Poco::Util::Application& self);
	void uninitialize();
	void defineOptions(Poco::Util::OptionSet& options);
	void handleOption(const std::string& name, const std::string& value);
	int main(const std::vector<std::string>& args);

private:
	Poco::Net::SocketReactor _reactor;
};

#endif // !__SERVICE_APPLICATION_H__
