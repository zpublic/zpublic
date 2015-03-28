#pragma once

#include "z_socket/z_socket.hpp"
#ifdef ZSOCKET_TEST_CODE

class client_listener : public ZLPullClientSocketListener
{
public:

    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, int iLength)
    {
        return emZLSocketHandleResultOk;
    }

    virtual void OnClose(ZLConnectId connId)
    {
        std::cout << std::endl;
    }

    virtual void OnError(ZLConnectId connId, emZLSocketOperation emOperation, int iErrorCode)
    {
        std::cout << std::endl;
    }

};

static client_listener cb_client;

inline void begin_server();
inline void end_server();

inline void z_socket_test_code()
{
    ZLClientSocket client(&cb_client);
}

#endif