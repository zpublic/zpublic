#include "venus_net/venus_net.h"
#include "protocol/protocol.h"
#include "protocol/opcodes.h"
#include "game_common/game_define.h"

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

TcpClient* g_tcpClient;
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

        switch (opcode)
        {
        case Opcodes::SCGetRandomNameRsp:
            {
                Protocol::SCGetRandomNameRsp response;
                DECODE_MESSAGE(response, message);
                printf("GetRandomNickname (Nickname = %s)", response.random_name.c_str());
                break;
            }
        case Opcodes::SCLoginRsp:
            {
                Protocol::SCLoginRsp response;
                DECODE_MESSAGE(response, message);
                printf("Login (result = %d, character_create_require = %d, player_id = %d)", 
                    response.login_result, 
                    response.character_create_require, 
                    response.player_id);

                if (response.character_create_require)
                {
                    Protocol::CSGetRandomNameReq getNicknameRequest;
                    getNicknameRequest.gender = Gender::Female;
                    g_tcpClient->sendMessage(Opcodes::CSGetRandomNameReq, getNicknameRequest);

                    Protocol::CSCheckNicknameExistReq checkNicknameExistRequest;
                    checkNicknameExistRequest.nickname = "xxx";
                    g_tcpClient->sendMessage(Opcodes::CSCheckNicknameExistReq, checkNicknameExistRequest);

                    Protocol::CSCreateCharacterReq createRequest;
                    createRequest.belief = 1;
                    createRequest.character_type = 1;
                    createRequest.gender = 0;
                    createRequest.nickname = "勿在浮沙筑高台";
                    g_tcpClient->sendMessage(Opcodes::CSCreateCharacterReq, createRequest);
                }

                break;
            }
        default:
            break;
        }
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
    g_tcpClient = new TcpClient(handler);

    try
    {
        std::cout << "starting.." << std::endl;
        for (int i = 0; i < 1; i++)
        {
            g_tcpClient->connect(serverAddress);

            Protocol::CSRegisterReq registerRequest;
            registerRequest.username = "138001655@qq.com";
            registerRequest.password = "e10adc3949ba59abbe56e057f20f883e";
            g_tcpClient->sendMessage(Opcodes::CSRegisterReq, registerRequest);

            Protocol::CSLoginReq loginRequest;
            loginRequest.account = "138001655@qq.com";
            loginRequest.password = "e10adc3949ba59abbe56e057f20f883e";
            g_tcpClient->sendMessage(Opcodes::CSLoginReq, loginRequest);
        }

        std::cout << "finished." << std::endl;
    }
    catch (const Poco::Exception& e)
    {
        std::cout << e.displayText() << std::endl;
    }

    getchar();

    delete g_tcpClient;
    return 0;
}