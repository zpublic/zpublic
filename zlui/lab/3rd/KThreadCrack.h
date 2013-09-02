#ifndef _KTHREADCRACK_H_
#define _KTHREADCRACK_H_

template<typename obj>
class _threadhost
{
public:
	typedef int (obj::*pfun)(void*);

	obj *m_pObj;
	pfun m_pFun;
	void *m_lp;
};

template<typename obj>
unsigned int __stdcall _ThreadCrack(void* lpThis)
{
	unsigned int nReturn = 0;
	_threadhost<obj> *pThis = (_threadhost<obj>*)lpThis;
	if (pThis)
	{
		obj *pObj = (obj*)pThis->m_pObj;
		_threadhost<obj>::pfun fun = pThis->m_pFun;

		nReturn = (unsigned int)(pObj->*fun)(pThis->m_lp);
		delete pThis;
	}

	return nReturn;
}

template<typename obj>
HANDLE KCreateThread(obj *pObj, int   (obj::*f)(void*), void *lp)
{
	_threadhost<obj> *pHost = new _threadhost<obj>;

	if (pHost == NULL)
		return 0;

	pHost->m_pObj = pObj;
	pHost->m_lp = lp;
	pHost->m_pFun = f;

	return (HANDLE)_beginthreadex(NULL, 0, _ThreadCrack<obj>, pHost, 0, NULL);
}

template<typename obj>
HANDLE KCreateThread(obj *pObj, int   (obj::*f)(void*), unsigned int *dwThreadID, void *lp)
{
	_threadhost<obj> *pHost = new _threadhost<obj>;

	if (pHost == NULL)
		return 0;

	pHost->m_pObj = pObj;
	pHost->m_lp = lp;
	pHost->m_pFun = f;

	return (HANDLE)_beginthreadex(NULL, 0, _ThreadCrack<obj>, pHost, 0, dwThreadID);
}

#endif