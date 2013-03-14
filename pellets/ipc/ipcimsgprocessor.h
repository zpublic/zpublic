#ifndef ZPUBLIC_I_IPC_PROCESSOR
#define ZPUBLIC_I_IPC_PROCESSOR

#include <string>

namespace zl
{
namespace Ipc
{

struct IMsgProcessor 
{
    virtual ~IMsgProcessor(){}
    virtual std::string Process(const char* szBuffer) = 0;
    virtual void Disconnect(void) = 0;
    virtual IMsgProcessor* Clone(void) = 0;
    virtual void Delete(IMsgProcessor* pProcessor) = 0;
};

}
}

#endif