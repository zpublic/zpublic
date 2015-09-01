/**
 * @file
 * @brief 本文件处理一些平台相关的东西.
 *
 * TODO: 尚未完善,还需调整.
 */

#pragma once
#include <assert.h>
#include "mongoose/mongoose.h"


#ifdef OS_ANDROID

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>

#define HAROLD_PRINT(fmt,...) __android_log_print(ANDROID_LOG_ERROR,"harold",fmt,##__VA_ARGS__)

typedef void*                       HAROLD_ROUTINE_RET_TYPE;
#define HAROLD_THREAD_HANDLE        pthread_t
#define HAROLD_INVALID_HANDLE       0
#define HAROLD_ROUTINE_CALL_TYPE
#define HAROLD_ROUTINE_IMP(type)    type HAROLD_ROUTINE_CALL_TYPE
typedef void *(*HAROLD_THREAD_START_ROUTINE)(void *);

inline HAROLD_THREAD_HANDLE HAROLD_CREATE_THREAD(HAROLD_THREAD_START_ROUTINE lpStartAddress, void* lpParameter)
{
	pthread_t handle = 0;
	pthread_create(&handle, 0, lpStartAddress, lpParameter);
    return handle;
}

inline void HAROLD_TERMINATE_THREAD(HAROLD_THREAD_HANDLE handle)
{
    if (handle != HAROLD_INVALID_HANDLE)
    {
        if(pthread_kill(handle, 0)!= ESRCH)
        {
        	sleep(1);
            pthread_kill(handle, SIGQUIT);
        }
    }
}

#else

#include <Windows.h>

#define HAROLD_PRINT(fmt,...) \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n")

typedef unsigned int                HAROLD_ROUTINE_RET_TYPE;
#define HAROLD_THREAD_HANDLE        HANDLE
#define HAROLD_INVALID_HANDLE       NULL
#define HAROLD_ROUTINE_CALL_TYPE    __stdcall
#define HAROLD_ROUTINE_IMP(type)    type HAROLD_ROUTINE_CALL_TYPE
#define HAROLD_THREAD_START_ROUTINE LPTHREAD_START_ROUTINE

inline HAROLD_THREAD_HANDLE HAROLD_CREATE_THREAD(HAROLD_THREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
    HAROLD_THREAD_HANDLE handle = ::CreateThread(NULL, 0, lpStartAddress, lpParameter, 0, NULL);
    return handle;
}

inline void HAROLD_TERMINATE_THREAD(HAROLD_THREAD_HANDLE handle)
{
    if (handle != HAROLD_INVALID_HANDLE)
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
