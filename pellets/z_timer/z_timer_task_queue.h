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

/*
 * ACE的定时器队列提供了heap、wheel（数组）、hash、list四种策略方式
 * libevent使用的是minheap
 * libuv使用的是rbtree
 * 从性能、依赖、复杂度等角度考虑后，在这里，使用的是stl的priority_queue
 * 也因此，只能添加任务不能删除任务
 * 非线程安全，外部加锁来保证
 */

NAMESPACE_ZL_BEGIN

class TimerTaskBase;

class TimerTaskQueue
{
public:
    TimerTaskQueue() {}
    ~TimerTaskQueue() {}

public:
    bool Insert(TimerTaskBase* pTask)
    {
        queue_.push(pTask);
        return true;
    }

    TimerTaskBase* Top()
    {
        return queue_.top();
    }

    TimerTaskBase* Pop()
    {
        TimerTaskBase* pTask = queue_.top();
        queue_.pop();
        return pTask;
    }

    unsigned int Size()
    {
        return (unsigned int)queue_.size();
    }

private:
    std::priority_queue<TimerTaskBase*, std::vector<TimerTaskBase*>, TimerTaskCmp> queue_;
};

NAMESPACE_ZL_END