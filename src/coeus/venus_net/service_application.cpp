#include "service_application.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TcpServer.h"
#include "Poco/NObserver.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/OptionSet.h"
#include "server_handler.h"

void ServiceApplication::initialize(Poco::Util::Application& self)
{
    // load default configuration files, if present
    ServerApplication::loadConfiguration();
    ServerApplication::initialize(self);
}

void ServiceApplication::uninitialize()
{
    ServerApplication::uninitialize();
}

void ServiceApplication::defineOptions(Poco::Util::OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Poco::Util::Option("help", "h", "display help information on command line arguments")
        .required(false)
        .repeatable(false));
}

void ServiceApplication::handleOption(const std::string& name, const std::string& value)
{
    ServerApplication::handleOption(name, value);

    if (name == "help")
    {
        std::cout << "show help" << std::endl;
    }
}

int ServiceApplication::main(const std::vector<std::string>& args)
{
    //从配置读取服务器信息
    unsigned short port = (unsigned short) config().getInt("port", 36911);

    Poco::Net::ServerSocket socket(port);
    Poco::Net::SocketAcceptor<ServerHandler> acceptor(socket, _reactor);

    Poco::Thread thread;
    thread.start(_reactor);

    // wait for CTRL-C or kill
    waitForTerminationRequest();

    // Stop the SocketReactor
    _reactor.stop();
    thread.join();
    return Application::EXIT_OK;
}