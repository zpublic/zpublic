#include "stdafx.h"
#include "test_hp_socket.h"
#include "hp_socket/socket/ClientSocket.h"

class CClientSocketListenerPP : public IClientSocketListener
{
public:
    virtual EnHandleResult OnPrepareConnect( CONNID dwConnID, SOCKET socket ) 
    {
        cout<<"pre_conn:"<<dwConnID<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnConnect( CONNID dwConnID ) 
    {
        cout<<"conn:"<<dwConnID<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnSend( CONNID dwConnID, const BYTE* pData, int iLength ) 
    {
        cout<<"send:"<<(const char*)pData<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnReceive( CONNID dwConnID, const BYTE* pData, int iLength ) 
    {
        cout<<"recv:"<<(const char*)pData<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnReceive( CONNID dwConnID, int iLength ) 
    {
        cout<<"recv2:"<<iLength<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnClose( CONNID dwConnID ) 
    {
        cout<<"close:"<<dwConnID<<endl;
        return HR_OK;
    }

    virtual EnHandleResult OnError( CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode ) 
    {
        cout<<"err:"<<iErrorCode<<endl;
        return HR_OK;
    }

};

void test_hp_socket()
{
    CClientSocketListenerPP listener;
    CClientSocket client(static_cast<IClientSocketListener *>(&listener));
    client.Start(L"127.0.0.1", 5555);
    client.Send(client.GetConnectionID(), (const BYTE*)"hello", 6);
    Sleep(200);
    client.Stop();
}