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
#include "Poco/File.h"
#include "Poco/NotificationQueue.h"
#include "logger.h"
#include "message_dispatcher.h"

namespace Venus
{

ServiceApplication::ServiceApplication(const std::string& serviceName, MessageDispatcher* messageDispatcher)
    : _serviceName(serviceName), _messageQueue(messageDispatcher)
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
    Logger::getInstance().init(this->logger());
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
    serverParams->setMaxThreads(1);         //最大IO线程数
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

    //创建消息队列处理线程
    MessageNotificationQueueWorker queueWorker(_messageQueue);
    Poco::ThreadPool::defaultPool().start(queueWorker);

    //创建连接工厂
    ConnectionFactory connectionFactory;
    connectionFactory.setMessageQueue(_messageQueue);

    //启动TCP服务
    Poco::Net::TCPServer server(&connectionFactory, socket, serverParams);
    server.start();
    info_log("Server started.");
    Poco::Util::ServerApplication::waitForTerminationRequest();

    _messageQueue.wakeUpAll();
    server.stop();

    return Application::EXIT_OK;
}

}