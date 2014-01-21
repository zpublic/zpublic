#include "venus_net/venus_net.h"
#include "protocol/protocol.h"
#include "protocol/opcodes.h"

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

class GameMessageHandler : public MessageHandler
{
public:
    virtual void onConnected()
    {
        printf("client has been connected to server.\n");
    }

    virtual void onMessage(uint16 opcode, const NetworkPacket::Ptr& message)
    {
        printf("onMessage() : [opcode = %d]\n", opcode);


        //CSTestPacketRsp requestMessage;
        //loginRequest.decode((const byte*)&message->messageBody[0], message->message.size());
        //DECODE_MESSAGE(requestMessage, message);
        //printf("        [value = %d]\n", requestMessage.uint_value);
        //printf("        [string = %s]\n", requestMessage.string_value.c_str());
    }

    virtual void onShutdown()
    {
        printf("disconnected.\n");
    }
};

int main(int argc, char** argv)
{
    Poco::Net::SocketAddress serverAddress("127.0.0.1:36911");
    GameMessageHandler handler;

    const int client_count = 100;
    TcpClient tcpClient(handler);

    try
    {
        std::cout << "starting.." << std::endl;
        for (int i = 0; i < 1; i++)
        {
            tcpClient.connect(serverAddress);

//             Protocol::CSRegisterReq registerRequest;
//             registerRequest.username = "coeus_user";
//             registerRequest.password = "coeus_password";
//             tcpClient.sendMessage(Opcodes::CSRegisterReq, registerRequest);
            
            Protocol::CSLoginReq loginRequest;
            loginRequest.account = "coeus_user";
            loginRequest.password = "coeus_password";
            tcpClient.sendMessage(Opcodes::CSLoginReq, loginRequest);

            loginRequest.account = "powman";
            loginRequest.password = "demaciaaaaa";
            tcpClient.sendMessage(Opcodes::CSLoginReq, loginRequest);

            tcpClient.close();
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