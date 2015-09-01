/*************************************************************************
*                                                                       *
*  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
*  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
*        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
*       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
*     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
*    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
*   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
* !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
*                                                                       *
*               Website: https://github.com/zpublic/zpublic             *
*                                                                       *
************************************************************************/
#pragma once

NAMESPACE_ZL_BEGIN

typedef std::map<ZLAsynTaskBase*, HANDLE>           ZLAsynTaskSet;
typedef std::map<ZLAsynTaskBase*, HANDLE>::iterator ZLAsynTaskSetIt;

class ZLAsynTaskMgr;
typedef struct _ZLAsynThreadStub
{
    _ZLAsynThreadStub(ZLAsynTaskMgr* pThis, ZLAsynTaskBase* pTask, DWORD dwDelay)
    {
        _this   = pThis;
        _task   = pTask;
        _delay = dwDelay;
    }
    ZLAsynTaskMgr*      _this;
    ZLAsynTaskBase*     _task;
    DWORD               _delay;
}ZLAsynThreadStub;

class ZLAsynTaskMgr
{
public:
    ZLAsynTaskMgr() : _stop_event(true) {}
    ~ZLAsynTaskMgr()
    {
        WaitAndStop();
    }

    bool PostTask(ZLAsynTaskBase* pTask, DWORD dwMillisecondDelay = 0)
    {
        if (!pTask) return false;

        ///> 因为可能还没加到任务集，任务线程就跑完了来删任务线程句柄，
        ///> 所以，这里先把任务加到任务集再创建线程
        {
            z_mutex_guard g(_task_set_mutex);
            _task_set[pTask] = NULL;
        }

        ZLAsynThreadStub* pStub = new ZLAsynThreadStub(this, pTask, dwMillisecondDelay);
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
        _stop_event.Set();
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
        _stop_event.Reset();
    }

private:
    static unsigned int WINAPI _Thread_Proc(void* pParam)
    {
        ZLAsynThreadStub* pStub = (ZLAsynThreadStub*)pParam;
        if (pStub)
        {
            if (pStub->_task)
            {
                if (pStub->_this->_stop_event.Wait(pStub->_delay) == WAIT_TIMEOUT)
                    pStub->_task->DoWork();

                if (pStub->_this->_stop_event.Wait(0) == WAIT_TIMEOUT)
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
    ThreadSync::CEvent              _stop_event;
};

NAMESPACE_ZL_END