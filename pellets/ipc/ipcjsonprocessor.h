#ifndef ZPUBLIC_MSG_PROCESSOR
#define ZPUBLIC_MSG_PROCESSOR

#include <string>
#include <list>
#include "ipcimsgprocessor.h"

/*
@ Desc	: ipcMsgProcessor ��Ϣ����Ļ������ܣ�������ipcPipeSvrChannel�ṩ����Ϣ�������󡣸��๦�����������䡣
*/

namespace zl
{
namespace Ipc
{

class ipcJsonProcessor : public IMsgProcessor
{
public:
    virtual ~ipcJsonProcessor(){ ClearAll(); }
    virtual std::string Process(const char* szBuffer);
    virtual void Disconnect(void);
    virtual IMsgProcessor* Clone(void);
    virtual void Delete(IMsgProcessor* pProcessor);


protected:
    void AddObject(int nID);
    void RemoveObject(int nID);
    void ClearAll(void);

private:
    std::list<int>	m_lstObjects;
};

}
}

#endif