#pragma once

#include "mongoose/mongoose.h"

///> 下面是平台差异代码的临时实现,还需调整

#ifdef OS_ANDROID

#define THREAD_RESULT_TYPE  void *
#define THREAD_NULL         0

    CREATE_THREAD(Handle, Func, Param)                  \
    {                                                       \
        int err;                                            \
        pthread_t ntid;                                     \
        err = pthread_create(&Handle, NULL, Func, Param);   \
        if (err != THREAD_NULL)                             \
            return E_FAIL;                                  \
    }

#define TERMINATE_THREAD(Handle)
            {if (THREAD_NULL == Handle) {pthread_kill(Handle, SIGKILL); Handle = -1; }}

#else

#include <Windows.h>

#define HAROLD_THREAD_RESULT_TYPE  HANDLE
#define HAROLD_THREAD_NULL         NULL

inline HAROLD_THREAD_RESULT_TYPE HAROLD_CREATE_THREAD(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
    HAROLD_THREAD_RESULT_TYPE handle = ::CreateThread(NULL, 0, lpStartAddress, lpParameter, 0, NULL);
    return handle;
}

inline void HAROLD_TERMINATE_THREAD(HAROLD_THREAD_RESULT_TYPE handle)
{
    if (handle != HAROLD_THREAD_NULL)
    {
        if (::WaitForSingleObject(handle, 1000) == WAIT_TIMEOUT)
        {
            ::TerminateThread(handle, 0); handle = NULL;
        }
        ::CloseHandle(handle);
    }
}

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

#endif
