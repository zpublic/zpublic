#ifndef ZPUBLIC_C_CLIENT
#define ZPUBLIC_C_CLIENT

#include "ipc_def.h"
#include "JsonParser/ipcjsonrun.h"
#include "ipcpipecltchannel.h"

/*
ClassName	: ipcCallClient
Brief		: 远程调用客户端，多线程支持时，每个Call者要加锁
*/
namespace zl
{
namespace Ipc
{

class ipcCallClient
{
public:
    ipcCallClient();
    ~ipcCallClient();

    BOOL ConnectServer(LPCTSTR szSvrName);
    void DisConnect(void);

    std::string Alloc(LPCSTR szClassName);

    std::string Dealloc(int id);

    std::string Call(LPCSTR szFuncName);
    
    template<class P1>
    std::string Call(LPCSTR szFuncName, const P1& p1);
    
    template<class P1, class P2>
    std::string Call(LPCSTR szFuncName, const P1& p1, const P2& p2);

    template<class P1, class P2, class P3>
    std::string Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3);

    template<class P1, class P2, class P3, class P4>
    std::string Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4);

    template<class P1, class P2, class P3, class P4, class P5>
    std::string Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);
    
    std::string Call(int id, LPCSTR szFuncName);

    template<class P1>
    std::string Call(int id, LPCSTR szFuncName, const P1& p1);

    template<class P1, class P2>
    std::string Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2);
    
    template<class P1, class P2, class P3>
    std::string Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3);
    
    template<class P1, class P2, class P3, class P4>
    std::string Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4);

    template<class P1, class P2, class P3, class P4, class P5>
    std::string Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);
   
    template<class T>
    BOOL CallFunc(LPCSTR funcName, T& funcRet);

    template<class T, class P1>
    BOOL CallFunc(LPCSTR funcName, T& funcRet, const P1& p1);

    template<class T, class P1, class P2>
    BOOL CallFunc(LPCSTR funcName, T& funcRet, const P1& p1, const P2& p2);

    template<class T, class P1, class P2, class P3>
    BOOL CallFunc(LPCSTR funcName, T& funcRet, const P1& p1, const P2& p2, const P3& p3);

    template<class T, class RP1>
    BOOL CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1);

    template<class T, class RP1, class P1>
    BOOL CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1, const P1& p1);

    template<class T, class RP1, class P1, class P2>
    BOOL CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1, const P1& p1, const P2& p2);

    template<class T, class RP1, class RP2>
    BOOL CallFuncRP2(LPCSTR funcName, T& funcRet, RP1& rp1, RP2& rp2);

    template<class T, class RP1, class RP2, class P1>
    BOOL CallFuncRP2(LPCSTR funcName, T& funcRet, RP1& rp1, RP2& rp2, const P1& p1);

protected:
    BOOL Connect(void);
    
    std::string RunJson(std::string& jsonString);

private:
    ZTString m_strSvrName;
    BOOL	m_bConnected;
    ipcPipeCltChannel m_pipeClt; 
};

template<class P1>
std::string ipcCallClient::Call(LPCSTR szFuncName, const P1& p1)
{	
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateGlobalCall(szFuncName, p1);
    
    return this->RunJson(json_call_string);
}

template<class P1, class P2>
std::string ipcCallClient::Call(LPCSTR szFuncName, const P1& p1, const P2& p2)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateGlobalCall(szFuncName, p1, p2);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3>
std::string ipcCallClient::Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateGlobalCall(szFuncName, p1, p2, p3);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3, class P4>
std::string ipcCallClient::Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateGlobalCall(szFuncName, p1, p2, p3, p4);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3, class P4, class P5>
std::string ipcCallClient::Call(LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateGlobalCall(szFuncName, p1, p2, p3, p4, p5);

    return this->RunJson(json_call_string);
}

template<class P1>
std::string ipcCallClient::Call(int id, LPCSTR szFuncName, const P1& p1)
{	
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateClassCall(id, szFuncName, p1);

    return this->RunJson(json_call_string);
}

template<class P1, class P2>
std::string ipcCallClient::Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateClassCall(id, szFuncName, p1, p2);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3>
std::string ipcCallClient::Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateClassCall(id, szFuncName, p1, p2, p3);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3, class P4>
std::string ipcCallClient::Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateClassCall(id, szFuncName, p1, p2, p3, p4);

    return this->RunJson(json_call_string);
}

template<class P1, class P2, class P3, class P4, class P5>
std::string ipcCallClient::Call(int id, LPCSTR szFuncName, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
    std::string json_call_string;

    json_call_string = zl::Ipc::CreateClassCall(id, szFuncName, p1, p2, p3, p4, p5);

    return this->RunJson(json_call_string);
}

template<class T>
BOOL ipcCallClient::CallFunc(LPCSTR funcName, T& funcRet)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class P1>
BOOL ipcCallClient::CallFunc(LPCSTR funcName, T& funcRet, const P1& p1)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, p1);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class RP1>
BOOL ipcCallClient::CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, rp1);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet, rp1))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class RP1, class P1>
BOOL ipcCallClient::CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1, const P1& p1)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, rp1, p1);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet, rp1))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class RP1, class P1, class P2>
BOOL ipcCallClient::CallFuncRP1(LPCSTR funcName, T& funcRet, RP1& rp1, const P1& p1, const P2& p2)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, rp1, p1, p2);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet, rp1))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class RP1, class RP2>
BOOL ipcCallClient::CallFuncRP2(LPCSTR funcName, T& funcRet, RP1& rp1, RP2& rp2)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, rp1, rp2);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet, rp1, rp2))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class RP1, class RP2, class P1>
BOOL ipcCallClient::CallFuncRP2(LPCSTR funcName, T& funcRet, RP1& rp1, RP2& rp2, const P1& p1)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, rp1, rp2, p1);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet, rp1, rp2))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class P1, class P2>
BOOL ipcCallClient::CallFunc(LPCSTR funcName, T& funcRet, const P1& p1, const P2& p2)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, p1, p2);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

template<class T, class P1, class P2, class P3>
BOOL ipcCallClient::CallFunc(LPCSTR funcName, T& funcRet, const P1& p1, const P2& p2, const P3& p3)
{
    BOOL bRet = FALSE;
    zl::Ipc::ipcReturnParse parser = this->Call(funcName, p1, p2, p3);
    if (parser.Error())
        goto Exit0;
    if (!parser.Value(funcRet))
        goto Exit0;
    bRet = TRUE;
Exit0:
    return bRet;
}

}
}

#endif