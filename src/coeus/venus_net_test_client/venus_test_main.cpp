#include <Poco/Net/SocketConnector.h>
#include "venus_net/service_application.h"

enum TestCaseType
{
    TC_CONCURRENCY_TESTING,         //短连接并发测试（建立连接随后断开）
    TC_CONNECTION_STRESS_TESTING,   //短连接压力测试（建立连接后发送若干长度数据随后断开）
};

class TestCaseRunner
{
public:
    virtual void run() = 0;
};

int main(int argc, char** argv)
{
    Poco::Net::SocketAddress serverAddress("127.0.0.1:36911");
    Poco::Net::StreamSocket clientConnector;

    try
    {
        std::cout << "starting.." << std::endl;
        for (int i = 0; i < 100; i++)
        {
            clientConnector.connect(serverAddress);
            //char buffer[1024] = {0};
            //clientConnector.receiveBytes(buffer, 1024, 0);
            //std::cout << " receive : " << buffer << std::endl;
            clientConnector.sendBytes((const void*)"xxx", 3, 0);
            clientConnector.close();
        }
        std::cout << "finished." << std::endl;
    }
    catch (const Poco::Exception& e)
    {
        std::cout << e.displayText() << std::endl;
    }

    getchar();
    return 0;
}