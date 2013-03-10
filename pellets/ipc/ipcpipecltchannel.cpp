#include "ipcpipecltchannel.h"

namespace zl
{
namespace Ipc
{

const DWORD ipcPipeCltChannel::ms_cdwMaxBufferSize = 100 * 1024 * 1024;
ipcPipeCltChannel::ipcPipeCltChannel()
{
    m_hPipe = NULL;
    m_hOverlappedEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

ipcPipeCltChannel::~ipcPipeCltChannel()
{
    this->DisConnect();
    if (m_hOverlappedEvent) CloseHandle(m_hOverlappedEvent);
}

void ipcPipeCltChannel::SetPipeName(LPCTSTR szPipeName)
{
    m_strPipeName = szPipeName;
}

HANDLE ipcPipeCltChannel::GetPipe()
{
    return m_hPipe;
}

BOOL ipcPipeCltChannel::Connect()
{
    BOOL bReturn = FALSE;

    if (m_hPipe)
    {
        bReturn = TRUE;
        goto Exit0;
    }
    
    do
    { 
        m_hPipe = CreateFile( 
            m_strPipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE, 
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        if (m_hPipe != INVALID_HANDLE_VALUE) 
            break; 

        DWORD dwError = GetLastError();
        //LOGA("ipcPipeCltChannel::Connect CreateFile error:%d", dwError);
        if (dwError != ERROR_PIPE_BUSY) 
        {
            goto Exit0;
        }

        // All pipe instances are busy, so wait for 1 second. 
        if (!WaitNamedPipe(m_strPipeName.c_str(), 1000)) 
        { 
            goto Exit0;
        }
    } while (true);

    // The pipe connected; change to message-read mode. 
    DWORD dwMode = PIPE_READMODE_BYTE; 
    BOOL bSuccess;
    bSuccess = SetNamedPipeHandleState( 
        m_hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!bSuccess) 
    {
        goto Exit0;
    }

    bReturn = TRUE;
    
Exit0:
    if (!bReturn) m_hPipe = NULL;
    
    return bReturn;
}

void ipcPipeCltChannel::DisConnect()
{
    if (m_hPipe)
    {
        FlushFileBuffers(m_hPipe); 
        DisconnectNamedPipe(m_hPipe); 
        CloseHandle(m_hPipe); 
        m_hPipe = NULL;
    }
}
BOOL ipcPipeCltChannel::Send(void* pBuffer, DWORD dwSize)
{
    BOOL bReturn = FALSE;
    OVERLAPPED overLapped;
    const int cnObjectCount = 2;
    HANDLE hObjectArray[cnObjectCount];

    if (!m_hPipe) goto Exit0;
    if (dwSize > ipcPipeCltChannel::ms_cdwMaxBufferSize) goto Exit0;

    ZeroMemory(&overLapped, sizeof(overLapped));

    overLapped.hEvent = this->m_hOverlappedEvent;
    hObjectArray[0] = this->m_hOverlappedEvent;
    hObjectArray[1] = this->m_hPipe;

    BOOL bRetCode = FALSE;
    DWORD dwWaitRet = 0;
    DWORD dwWriteSize = 0;

    ResetEvent(this->m_hOverlappedEvent);

    bRetCode = ::WriteFile(this->m_hPipe, &dwSize, sizeof(DWORD), &dwWriteSize, &overLapped);
    if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING)
    {
        goto Exit0;
    }

    dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
    if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

    bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwWriteSize, FALSE);
    if (!bRetCode || dwWriteSize != sizeof(DWORD)) goto Exit0;
    
    DWORD dwTransferSize = 0;
    do 
    {
        ResetEvent(this->m_hOverlappedEvent);
        bRetCode = ::WriteFile(this->m_hPipe, (UCHAR*)pBuffer + dwTransferSize, dwSize - dwTransferSize, &dwWriteSize, &overLapped);
        if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) 
        {
            goto Exit0;
        }
        
        dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
        if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

        bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwWriteSize, FALSE);
        if (!bRetCode || dwWriteSize == 0) goto Exit0;
        
        if (dwWriteSize + dwTransferSize == dwSize) break;
        
        dwTransferSize += dwWriteSize;
    } while(true);

    bReturn = TRUE;

Exit0:
    return bReturn;
}

BOOL ipcPipeCltChannel::Recv(void** ppBuffer, DWORD* pSize)
{
    BOOL bReturn = FALSE;
    OVERLAPPED overLapped;
    const int cnObjectCount = 2;
    HANDLE hObjectArray[cnObjectCount];
    BOOL bRetCode = FALSE;
    DWORD dwWaitRet = 0;
    DWORD dwReadSize = 0;
    DWORD dwDataSize = 0;
    UCHAR* pDataBuffer = NULL;

    if (!m_hPipe) goto Exit0;
    
    hObjectArray[0] = this->m_hOverlappedEvent;
    hObjectArray[1] = this->m_hPipe;

    ::ResetEvent(this->m_hOverlappedEvent);
    ZeroMemory(&overLapped, sizeof(overLapped));
    overLapped.hEvent = this->m_hOverlappedEvent;
    bRetCode = ::ReadFile(this->m_hPipe, &dwDataSize, sizeof(DWORD), &dwReadSize, &overLapped);
    if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) 
    {
        goto Exit0;
    }

    dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
    if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

    bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwReadSize, FALSE);
    if (!bRetCode || dwReadSize != sizeof(DWORD) || dwDataSize == 0) goto Exit0;

    if (dwDataSize > ipcPipeCltChannel::ms_cdwMaxBufferSize) goto Exit0;

    DWORD dwTransferSize = 0;
    pDataBuffer = new UCHAR[dwDataSize + 1];

    do 
    {
        ::ResetEvent(this->m_hOverlappedEvent);
        ZeroMemory(&overLapped, sizeof(overLapped));
        overLapped.hEvent = this->m_hOverlappedEvent;
        bRetCode = ::ReadFile(this->m_hPipe, (UCHAR*)pDataBuffer + dwTransferSize, dwDataSize - dwTransferSize, &dwReadSize, &overLapped);
        if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING)
        {
            goto Exit0;
        }
        
        dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
        if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

        bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwReadSize, FALSE);
        if (!bRetCode || dwReadSize == 0) goto Exit0;

        if (dwReadSize + dwTransferSize == dwDataSize) break;

        dwTransferSize += dwReadSize;
    } while(true);

    pDataBuffer[dwDataSize] = 0;
    *ppBuffer = pDataBuffer;
    *pSize = dwDataSize;
    bReturn = TRUE;
Exit0:
    if (!bReturn)
    {
        if (pDataBuffer) delete[] pDataBuffer;
    }
    return bReturn;
}

}
}