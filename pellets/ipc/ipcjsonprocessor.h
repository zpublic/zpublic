#ifndef ZPUBLIC_MSG_PROCESSOR
#define ZPUBLIC_MSG_PROCESSOR

#include <string>
#include <list>
#include "ipcimsgprocessor.h"

/*
@ Desc	: ipcMsgProcessor 消息处理的基本功能，负责处理ipcPipeSvrChannel提供的消息，及错误。更多功能由子类扩充。
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