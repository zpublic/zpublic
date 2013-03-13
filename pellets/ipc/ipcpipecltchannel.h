#ifndef ZPUBLIC_IPS_CHANNEL
#define ZPUBLIC_IPS_CHANNEL

#include "ipc_def.h"

/*
@ Desc		: ipcPipeCltChannel���������ͣ�������������Ϣ
@ Remark	: ���̵߳��ã��ɵ������Լ�������
*/

namespace zl
{
namespace Ipc
{

class ipcPipeCltChannel
{
public:
    ipcPipeCltChannel();
    ~ipcPipeCltChannel();
    
    void SetPipeName(LPCTSTR szPipeName);
    
    BOOL Connect(void);
    void DisConnect(void);

    HANDLE GetPipe(void);

    BOOL Send(void* pBuffer, DWORD dwSize);
    BOOL Recv(void** ppBuffer, DWORD* pSize);

private:
    HANDLE m_hPipe;
    HANDLE m_hOverlappedEvent;
    ZTString m_strPipeName;

private:
    const static DWORD ms_cdwMaxBufferSize;
};

}
}

#endif