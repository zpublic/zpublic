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
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#include "z_timer_task.h"
#include "z_timer_task_queue.h"
#include "z_timer_watcher.h"

class TimerDispatcher
{
public:
    TimerDispatcher(ITimerWatcher* pWatcher = NULL)
    {
        pWatcher_       = pWatcher;
        event_          = NULL;
        thread_         = NULL;
        stop_           = false;
    }
    ~TimerDispatcher() {}

    int Init()
    {
        event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!event_)
        {
            return -1;
        }
        return 0;
    }
    int Uninit(DWORD dwMilliseconds = 2000)
    {
        Stop();
        if (thread_)
        {
            DWORD dwRet = ::WaitForSingleObject(thread_, dwMilliseconds);
            if (dwRet == WAIT_TIMEOUT)
            {
                ::TerminateThread(thread_, 0);
            }
            ::CloseHandle(thread_);
            thread_ = NULL;
        }
        {
            z_mutex_guard x(queue_mutex_);
            while (queue_.Size() > 0)
            {
                TimerTaskBase* pTask = queue_.Pop();
                if (pTask && pTask->Release())
                {
                    delete pTask;
                }
            }
        }
        return 0;
    }
    int Start()
    {
        stop_ = false;
        ///> event有效才做事
        if (!thread_ && event_)
        {
            thread_ = (HANDLE)_beginthreadex(NULL, 0, _ThreadRoute, this, 0, NULL);
        }
        if (!thread_)
        {
            return -1;
        }
        return 0;
    }
    int Stop()
    {
        stop_ = true;
        if (event_)
        {
            ::SetEvent(event_);
        }
        return 0;
    }

    int AddTimerTask(TimerTaskBase* pTask)
    {
        _InsertTask(pTask);
        if (event_)
        {
            ::SetEvent(event_);
        }
        return 0;
    }

private:
    static unsigned int __stdcall _ThreadRoute( LPVOID pRaram )
    {
        TimerDispatcher* pThis = (TimerDispatcher*)pRaram;
        pThis->_WorkThread();
        _endthreadex(0);
        return 0;
    }

    void _WorkThread()
    {
        DWORD dwWait = INFINITE;
        int waittime = -1;
        do 
        {
            _RunTask();

            waittime = -1;
            if (_CalcWaitTime(waittime))
            {
                if (waittime <= 0)
                {
                    waittime = 0;
                }
            }
            dwWait = INFINITE;
            if (waittime >= 0)
            {
                dwWait = waittime;
            }
            DWORD dwRet = ::WaitForSingleObject(event_, dwWait);
            if (dwRet != WAIT_TIMEOUT && dwRet != WAIT_OBJECT_0)
            {
                break;
            }
            if (dwRet == WAIT_OBJECT_0 && stop_)
            {
                break;
            }
        } while (true);
    }

    void _RunTask()
    {
        bool bRepeat = false;
        TimerTaskBase* pTask;
        ///> 执行所有已到时间的任务
        while (_GetTask(&pTask))
        {
            if (pWatcher_)
            {
                pWatcher_->RunTaskBegin(pTask);
            }

            pTask->DoWork();
            pTask->ExecutedAdd();

            if (pWatcher_)
            {
                pWatcher_->RunTaskEnd(pTask);
            }

            ///> 处理要重复执行的任务
            bRepeat = false;
            if (pTask->Repeat()
                || pTask->Times() > pTask->Executed() )
            {
                _InsertTask(pTask);
                bRepeat = true;
            }

            if (!bRepeat && pTask->Release())
            {
                delete pTask;
            }
        }
    }
    void _InsertTask(TimerTaskBase* pTask)
    {
        if (pWatcher_)
        {
            pWatcher_->InsertTask(pTask);
        }
        z_mutex_guard x(queue_mutex_);
        queue_.Insert(pTask);
    }
    bool _CalcWaitTime(int& waittime)
    {
        z_mutex_guard x(queue_mutex_);
        while (queue_.Size() > 0)
        {
            if (queue_.Top() == NULL)
            {
                continue;
            }
            if (queue_.Top()->Invalid())
            {
                TimerTaskBase* pTask = queue_.Pop();
                if (pWatcher_)
                {
                    pWatcher_->RemoveTask(pTask, false);
                }
                if (pTask->Release())
                {
                    delete pTask;
                }
                continue;
            }
            waittime = queue_.Top()->WaitTime();
            return true;
        }
        return false;
    }
    bool _GetTask(TimerTaskBase** ppTask)
    {
        z_mutex_guard x(queue_mutex_);
        while (queue_.Size() > 0)
        {
            if (queue_.Top() == NULL)
            {
                continue;
            }
            if (queue_.Top()->Invalid())
            {
                TimerTaskBase* pTask = queue_.Pop();
                if (pWatcher_)
                {
                    pWatcher_->RemoveTask(pTask, false);
                }
                if (pTask->Release())
                {
                    delete pTask;
                }
                continue;
            }
            int waittime = queue_.Top()->WaitTime();
            if (waittime <= 0)
            {
                *ppTask = queue_.Pop();
                if (pWatcher_)
                {
                    pWatcher_->RemoveTask(*ppTask, true);
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

private:
    ITimerWatcher*      pWatcher_;          ///> 观察任务执行状况
    TimerTaskQueue      queue_;             ///> 定时器任务队列
    z_mutex             queue_mutex_;       ///> 定时器任务队列的访问锁
    HANDLE              event_;             ///> 通知退出或增加任务
    HANDLE              thread_;            ///> 工作线程
    bool                stop_;              ///> 退出标志
};