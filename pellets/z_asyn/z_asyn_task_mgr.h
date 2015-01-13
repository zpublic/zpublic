#pragma once

NAMESPACE_ZL_BEGIN

typedef std::map<ZLAsynTaskBase*, HANDLE>           ZLAsynTaskSet;
typedef std::map<ZLAsynTaskBase*, HANDLE>::iterator ZLAsynTaskSetIt;

class ZLAsynTaskMgr;
typedef struct _ZLAsynThreadStub
{
    _ZLAsynThreadStub(ZLAsynTaskMgr* pThis, ZLAsynTaskBase* pTask)
    {
        _this = pThis;
        _task = pTask;
    }
    ZLAsynTaskMgr*      _this;
    ZLAsynTaskBase*     _task;
}ZLAsynThreadStub;

class ZLAsynTaskMgr
{
public:
    bool PostTask(ZLAsynTaskBase* pTask)
    {
        if (!pTask) return false;

        ///> 因为可能还没加到任务集，任务线程就跑完了来删任务线程句柄，
        ///> 所以，这里先把任务加到任务集再创建线程
        {
            z_mutex_guard g(_task_set_mutex);
            _task_set[pTask] = NULL;
        }

        ZLAsynThreadStub* pStub = new ZLAsynThreadStub(this, pTask);
        HANDLE hThread = (HANDLE)_beginthreadex(
            NULL,
            0,
            _Thread_Proc,
            (void*)pStub,
            0,
            NULL);
        if (hThread)
        {
            z_mutex_guard g(_task_set_mutex);
            ZLAsynTaskSetIt it = _task_set.find(pTask);
            if (it != _task_set.end())
                it->second = hThread;
            else
                ::CloseHandle(hThread); ///> 任务线程已经跑完的情况
            return true;
        }
        else
        {
            z_mutex_guard g(_task_set_mutex);
            ZLAsynTaskSetIt it = _task_set.find(pTask);
            if (it != _task_set.end())
                _task_set.erase(it);
            return false;
        }
    }

    void WaitAndStop(DWORD dwMillisecondPerTask = 100)
    {
        z_mutex_guard g(_task_set_mutex);
        ZLAsynTaskSetIt it = _task_set.begin();
        while (it != _task_set.end())
        {
            if (it->second)
            {
                DWORD dwRet = ::WaitForSingleObject(it->second, dwMillisecondPerTask);
                if (dwRet == WAIT_TIMEOUT)
                {
                    ::TerminateThread(it->second, 0);
                    it->second = NULL;
                }
                ::CloseHandle(it->second);
            }
            ++it;
        }
        _task_set.clear();
    }

private:
    static unsigned int WINAPI _Thread_Proc(void* pParam)
    {
        ZLAsynThreadStub* pStub = (ZLAsynThreadStub*)pParam;
        if (pStub)
        {
            if (pStub->_task)
            {
                pStub->_task->DoWork();
                {
                    z_mutex_guard g(pStub->_this->_task_set_mutex);
                    ZLAsynTaskSetIt it = pStub->_this->_task_set.find(pStub->_task);
                    if (it != pStub->_this->_task_set.end())
                    {
                        if (it->second)
                        {
                            ::CloseHandle(it->second);
                        }
                        pStub->_this->_task_set.erase(it);
                    }
                }
                delete pStub->_task;
            }
            delete pStub;
        }
        _endthreadex(0);
        return 0;
    }

    ZLAsynTaskSet                   _task_set;
    z_mutex                         _task_set_mutex;
};

NAMESPACE_ZL_END