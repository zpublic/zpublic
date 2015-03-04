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

/************************************************************************/
/*  主要针对的问题：
/*      怎样更方便的在c++里异步执行代码？
/*      场景举例：在界面线程里，要做一个实时的数据上报
/*  重要的点：
/*      1，不做线程池设计，每个task一个线程
/*      2，将要执行的代码放到异步线程去执行之后，不提倡查询状态的方法
/*      3，task manager线程安全
/*      3，task manager能在task执行完自动释放资源
/*      4，task manager能等待、释放所有task
/*      5，同一个task不能加入多次，所以请 obj.PostTask(new taskA);
/**************************************************************************/

#include <map>
#include <windows.h>
#include <process.h>
#include "../thread_sync/thread_sync.h"

#ifndef NAMESPACE_ZL_BEGIN
#define NAMESPACE_ZL_BEGIN      namespace zl {
#define NAMESPACE_ZL_END        }
#endif

#include "z_asyn_task_base.h"
#include "z_asyn_task_for_static_function.h"
#include "z_asyn_task_for_member_function.h"
#include "z_asyn_task_mgr.h"
