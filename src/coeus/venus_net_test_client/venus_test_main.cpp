#include "common.h"
#include <Poco/Net/SocketConnector.h>
#include "venus_net/service_application.h"
#include "venus_net/basic_stream.h"
#include "venus_net/stream_writer.h"
#include "venus_net/stream_reader.h"
#include "venus_net/tcp_client.h"

struct CSTestPacketReq : public NetworkMessage
{
    uint32 uint_value;
    std::string string_value;

    int byteSize()
    {
        return sizeof(uint_value) + (string_value.length() + 2);
    }

    void encode(byte* buffer, size_t size)
    {
        StreamWriter w((char*)buffer, size);
        w << uint_value;
        w << string_value;
    }

    void decode(const byte* buffer, size_t size)
    {
        StreamReader r((const char*)buffer, size);
        r >> uint_value;
        r >> string_value;
    }
};

struct CSTestPacketRsp : public NetworkMessage
{
    uint32 uint_value;
    std::string string_value;

    int byteSize()
    {
        return sizeof(uint_value) + (string_value.length() + 2);
    }

    void encode(byte* buffer, size_t size)
    {
        StreamWriter w((char*)buffer, size);
        w << uint_value;
        w << string_value;
    }

    void decode(const byte* buffer, size_t size)
    {
        StreamReader r((const char*)buffer, size);
        r >> uint_value;
        r >> string_value;
    }
};


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
        CSTestPacketRsp requestMessage;
        //requestMessage.decode((const byte*)&message->messageBody[0], message->messageBody.size());
        DECODE_MESSAGE(requestMessage, message);
        printf("        [value = %d]\n", requestMessage.uint_value);
        printf("        [string = %s]\n", requestMessage.string_value.c_str());
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
        for (int i = 0; i < 1000; i++)
        {
            tcpClient.connect(serverAddress);
            
            CSTestPacketReq requestMessage;
            requestMessage.uint_value = 10;
            requestMessage.string_value = "SB";
            tcpClient.sendMessage(10001, requestMessage);
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