#ifndef ZPUBLIC_IPS_SVR
#define ZPUBLIC_IPS_SVR

#include "ipc_def.h"
#include <atlstr.h>
#include <list>

#include "ipcimsgprocessor.h"
#include "ipcpipesvrchannel.h"

/*
@ ClassName	: ipcPipeSvr
@ Desc		: 建立Pipe服务，监听到连接就创建服务接收请求
*/

namespace zl
{
namespace Ipc
{

class ipcPipeSvr
{
public:
    ipcPipeSvr();
    ~ipcPipeSvr();
    
    void SetMsgProcessor(IMsgProcessor* pMsgProcessor);
    BOOL Start(LPCTSTR szPipeName);
    void Stop(void);

protected:
    static UINT WINAPI ListenThread(LPVOID p);
    void AddChannel(ipcPipeSvrChannel* pChannel);
    void RemoveDisconnectedChannel(void);
    void WaitAllChannelsDisconnect(void);
    PSECURITY_ATTRIBUTES GetSecurityDescriptor(void);

private:
    HANDLE	m_hListenThread;
    HANDLE	m_hExitEvent;
    HANDLE	m_hStartComplete;
    ZTString m_strPipeName;
    IMsgProcessor* m_pMsgProcessor;
    std::list<ipcPipeSvrChannel*> m_lstPipeChannels;
};

}
}

#endif