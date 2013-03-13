#include "ipccallclient.h"

namespace zl
{
namespace Ipc
{

ipcCallClient::ipcCallClient()
{
    m_bConnected = FALSE;
}

ipcCallClient::~ipcCallClient()
{
    this->DisConnect();
}

BOOL ipcCallClient::ConnectServer(LPCTSTR szSvrName)
{
    ZTString tmpStr = _T("\\\\.\\pipe\\");
    tmpStr += szSvrName;
    m_strSvrName = tmpStr;
    //m_strSvrName.Format(_T("\\\\.\\pipe\\%s"), szSvrName);
    return this->Connect();
}

BOOL ipcCallClient::Connect()
{
    if (m_bConnected) return TRUE;

    if (m_strSvrName.empty()) return FALSE;
    
    m_pipeClt.SetPipeName(m_strSvrName.c_str());
    m_bConnected = m_pipeClt.Connect();

    return m_bConnected;
}

void ipcCallClient::DisConnect()
{
    if (!m_bConnected) return;
    
    m_bConnected = FALSE;
    m_pipeClt.DisConnect();
}

std::string ipcCallClient::RunJson(std::string& jsonString)
{
    BOOL bRetCode = FALSE;
    std::string json_ret_string;
    enumReturnValule json_ret_error = enumRet_Error;

    if (!m_bConnected) goto Exit0;

    bRetCode = m_pipeClt.Send((void*)jsonString.c_str(), jsonString.size());
    if (!bRetCode) goto Exit0;

    void* pRecvBuffer = NULL;
    DWORD dwRetSize = 0;
    bRetCode = m_pipeClt.Recv(&pRecvBuffer, &dwRetSize);
    if (!bRetCode) goto Exit0;

    json_ret_string = (char*)pRecvBuffer;
    delete[] pRecvBuffer;

    json_ret_error = enumRet_Succeed;
    
Exit0:
    if (json_ret_error == enumRet_Succeed)
    {
        return json_ret_string;
    }
    else
    {
        return ipcJsonReturn(json_ret_error).ToString();
    }
}

std::string ipcCallClient::Alloc(LPCSTR szClassName)
{
    std::string json_call_string;
    
    json_call_string = CreateObjectCreateCall(szClassName);

    return this->RunJson(json_call_string);
}

std::string ipcCallClient::Dealloc(int id)
{
    std::string json_call_string;

    json_call_string = CreateObjectDeleteCall(id);

    return this->RunJson(json_call_string);
}

std::string ipcCallClient::Call(LPCSTR szFuncName)
{
    std::string json_call_string;

    json_call_string = CreateGlobalCall(szFuncName);

    return this->RunJson(json_call_string);
}

std::string ipcCallClient::Call(int id, LPCSTR szFuncName)
{
    std::string json_call_string;

    json_call_string = CreateClassCall(id, szFuncName);

    return this->RunJson(json_call_string);
}

}
}