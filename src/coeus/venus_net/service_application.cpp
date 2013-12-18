#include "service_application.h"
#include "server_handler.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TcpServer.h"
#include "Poco/Thread.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/File.h"

ServiceApplication::ServiceApplication(const std::string& serviceName)
    : _serviceName(serviceName),
    _file_channel(new Poco::FileChannel),
    _console_channel(new Poco::ConsoleChannel)
{
}

ServiceApplication::~ServiceApplication()
{

}

void ServiceApplication::initialize(Poco::Util::Application& self)
{
    ServerApplication::loadConfiguration();
    ServerApplication::initialize(self);

    // init logger
    _logger = &this->logger();
    _logger->setLevel("trace");

    _file_channel->setProperty("path", "server.log");
    _file_channel->setProperty("rotation", "2 M");
    _file_channel->setProperty("archive", "timestamp");
    _file_channel->setProperty("compress", "true");

    Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter());  
    patternFormatter->setProperty("pattern", "[%Y-%m-%d %H:%M:%S.%i %U:%u] %p : %t");

    Poco::AutoPtr<Poco::Channel> fileChannel(new Poco::FormattingChannel(patternFormatter, _file_channel));
    Poco::AutoPtr<Poco::Channel> consleChannel(new Poco::FormattingChannel(patternFormatter, _console_channel));

    Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);
    splitterChannel->addChannel(fileChannel);
    splitterChannel->addChannel(consleChannel);
    _logger->setChannel(splitterChannel);

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
    Poco::Net::TCPServerParams* serverParams = new Poco::Net::TCPServerParams();
    serverParams->setMaxQueued(64);         //连接队列最大数
    serverParams->setMaxThreads(8);         //最大IO线程数
    serverParams->setThreadIdleTime(100);   //线程终止时最大等待时间

    info_log(
        "\n"
        "===================== Server Startup ===================\n"
        "       Service Name = %s\n"
        "       Listen Port  = %d\n"
        "       I/O Thread   = %d\n"
        "       Max Queued   = %d\n"
        "========================================================\n"
        "我的生涯一片无悔，我想起那天夕阳下的奔跑，那是我逝去的青春\n"
        "========================================================",
        _serviceName.c_str(), port, serverParams->getMaxThreads(), serverParams->getMaxQueued()
        );

    Poco::Net::TCPServer server(new ConnectionFactory(), socket, serverParams);
    server.start();

    info_log("Server started.");
    Poco::Util::ServerApplication::waitForTerminationRequest();
    server.stop();

    return Application::EXIT_OK;
}

Poco::Logger* ServiceApplication::_logger = nullptr;
Poco::Logger& ServiceApplication::serviceLogger()
{
    return *_logger;
}