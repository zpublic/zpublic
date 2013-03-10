#ifndef ZPUBLIC_IPS_REF_COUNT
#define ZPUBLIC_IPS_REF_COUNT

/*
@ ClassName	: ipcRefCount
@ Brief		: 引用计数，其他类可以has it, use it, is it
*/

namespace zl
{
namespace Ipc
{

struct ipcRefCount
{
	ipcRefCount() : m_lRefCount(0){}
	virtual ~ipcRefCount(){}

	long AddRef(void){ return ::InterlockedIncrement(&m_lRefCount); }
	long Release(void){ return ::InterlockedDecrement(&m_lRefCount); }
	long RefCount(void){ return m_lRefCount; }
	
private:
	long m_lRefCount;
};

}
}

#endif