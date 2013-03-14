#include "ipcpipesvr.h"

#define defDefaultPipeBufferSize	2048

namespace zl
{
namespace Ipc
{

ipcPipeSvr::ipcPipeSvr()
{
    m_hExitEvent	= NULL;
    m_hListenThread = NULL;
    m_pMsgProcessor = NULL;
    m_hStartComplete = NULL;
}

ipcPipeSvr::~ipcPipeSvr()
{
    this->Stop();
}

void ipcPipeSvr::SetMsgProcessor(IMsgProcessor* pMsgProcessor)
{
    m_pMsgProcessor = pMsgProcessor;
}

BOOL ipcPipeSvr::Start(LPCTSTR szPipeName)
{
    BOOL bReturn = TRUE;
    m_strPipeName = _T("\\\\.\\pipe\\");
    m_strPipeName += szPipeName;
    //m_strPipeName.Format(_T("\\\\.\\pipe\\%s"), szPipeName);
    
    m_hExitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!m_hExitEvent) return FALSE;

    m_hStartComplete = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!m_hStartComplete) return FALSE;

    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ipcPipeSvr::ListenThread, (LPVOID)this, 0, NULL);
    
    if (::WaitForSingleObject(m_hStartComplete, 100) != WAIT_OBJECT_0 &&
        ::WaitForSingleObject(m_hListenThread, 100) == WAIT_OBJECT_0)
    {
        bReturn = FALSE;
        CloseHandle(m_hListenThread);
        m_hListenThread = NULL;
    }

    CloseHandle(m_hStartComplete);
    m_hStartComplete = NULL;
    
    return bReturn;
}

void ipcPipeSvr::Stop()
{
    if (m_hExitEvent) ::SetEvent(m_hExitEvent);
    WaitForSingleObject(m_hListenThread, INFINITE);

    this->WaitAllChannelsDisconnect();
}

UINT WINAPI ipcPipeSvr::ListenThread(LPVOID p)
{
    ipcPipeSvr* pThis = (ipcPipeSvr*)p;
    
    HANDLE hPipe; 
    BOOL bRetCode = FALSE;
    DWORD dwWaitRet = 0;
    DWORD dwError = 0;
    OVERLAPPED overLapped;
    const int cnObjectCount = 2;
    HANDLE hObjectArray[cnObjectCount];
    
    ZeroMemory(&overLapped, sizeof(overLapped));
    overLapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    hObjectArray[0] = overLapped.hEvent;
    hObjectArray[1] = pThis->m_hExitEvent;

    hPipe = CreateNamedPipe( 
        pThis->m_strPipeName.c_str(),             // pipe name 
        PIPE_ACCESS_DUPLEX |	  // read/write access 
        FILE_FLAG_OVERLAPPED,     // asynchronous way
        PIPE_TYPE_BYTE |		  // message type pipe 
        PIPE_READMODE_BYTE |	  // message-read mode 
        PIPE_WAIT,                // blocking mode 
        PIPE_UNLIMITED_INSTANCES, // max. instances  
        defDefaultPipeBufferSize, // output buffer size 
        defDefaultPipeBufferSize, // input buffer size 
        NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
        pThis->GetSecurityDescriptor());                    // default security attribute 
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        hPipe = NULL;
        goto Exit0;
    }

    bRetCode = ConnectNamedPipe(hPipe, &overLapped);
    if (!bRetCode)
    {
        dwError = ::GetLastError();
        if (dwError != ERROR_IO_PENDING &&
            dwError != ERROR_PIPE_CONNECTED)
        {
            goto Exit0;
        }
    }
    
    ::SetEvent(pThis->m_hStartComplete);
    
    for (;;) 
    { 
        dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
        if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;
        
        ipcPipeSvrChannel* pChannel = new ipcPipeSvrChannel;
        pThis->AddChannel(pChannel);
        pChannel->SetExitEvent(pThis->m_hExitEvent);
        pChannel->SetProcessor(pThis->m_pMsgProcessor->Clone());
        pChannel->Start(hPipe);

        hPipe = CreateNamedPipe( 
            pThis->m_strPipeName.c_str(),             // pipe name 
            PIPE_ACCESS_DUPLEX |	  // read/write access 
            FILE_FLAG_OVERLAPPED,     // asynchronous way
            PIPE_TYPE_BYTE |		  // message type pipe 
            PIPE_READMODE_BYTE |	  // message-read mode 
            PIPE_WAIT,                // blocking mode 
            PIPE_UNLIMITED_INSTANCES, // max. instances  
            defDefaultPipeBufferSize, // output buffer size 
            defDefaultPipeBufferSize, // input buffer size 
            NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
            pThis->GetSecurityDescriptor());                    // default security attribute 
        if (hPipe == INVALID_HANDLE_VALUE) 
        {
            hPipe = NULL;
            goto Exit0;
        }

        ::ResetEvent(overLapped.hEvent);
        bRetCode = ConnectNamedPipe(hPipe, &overLapped);
        if (!bRetCode)
        {
            dwError = ::GetLastError();
            if (dwError != ERROR_IO_PENDING &&
                dwError != ERROR_PIPE_CONNECTED)
            {
                goto Exit0;
            }
        }
        
        pThis->RemoveDisconnectedChannel();
    } 

Exit0:
    if (overLapped.hEvent) CloseHandle(overLapped.hEvent);
    if (hPipe) CloseHandle(hPipe);

    return 0;
}

void ipcPipeSvr::AddChannel(ipcPipeSvrChannel* pChannel)
{
    m_lstPipeChannels.push_back(pChannel);
}

void ipcPipeSvr::RemoveDisconnectedChannel()
{
    ipcPipeSvrChannel* pTempChannel = NULL;
    std::list<ipcPipeSvrChannel*>::iterator iter;

    for (iter = m_lstPipeChannels.begin(); iter != m_lstPipeChannels.end();)
    {
        pTempChannel = *iter;
        if (pTempChannel->IsConnected())
        {
            iter++;
        }
        else
        {
            IMsgProcessor* pProcessor = pTempChannel->GetProcessor();
            pTempChannel->SetProcessor(NULL);
            this->m_pMsgProcessor->Delete(pProcessor);
            delete pTempChannel;
            m_lstPipeChannels.erase(iter++);
        }
    }
}

void ipcPipeSvr::WaitAllChannelsDisconnect()
{
    int nConnectCount = 0;
    ipcPipeSvrChannel* pTempChannel = NULL;
    std::list<ipcPipeSvrChannel*>::iterator iter;

    for (iter = m_lstPipeChannels.begin(); iter != m_lstPipeChannels.end(); iter++)
    {
        pTempChannel = *iter;

        do 
        {
            if (pTempChannel->IsConnected())
                Sleep(1);
            else
                break;
        } while (true);
    }

    Sleep(100);

    for (iter = m_lstPipeChannels.begin(); iter != m_lstPipeChannels.end();)
    {
        pTempChannel = *iter;

        IMsgProcessor* pProcessor = pTempChannel->GetProcessor();
        pTempChannel->SetProcessor(NULL);
        this->m_pMsgProcessor->Delete(pProcessor);
        delete pTempChannel;

        m_lstPipeChannels.erase(iter++);
    }
}

PSECURITY_ATTRIBUTES ipcPipeSvr::GetSecurityDescriptor(void)
{
    static BOOL bInited = FALSE;
    static SECURITY_DESCRIPTOR sd = { 0 };
    static SECURITY_ATTRIBUTES sa = { 0 };

    if (!bInited)
    {
        ::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        ::SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = &sd;

        bInited = TRUE;
    }

    return &sa;
}

}
}