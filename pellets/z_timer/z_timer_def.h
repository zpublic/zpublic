#pragma once
#include <windows.h>

///> 这个时间用来计算任务执行点，用boot时间和程序执行时间都是可以的
inline unsigned int TimerGetTime()
{
    return ::GetTickCount();
}

///> 任务执行次数为99999则无限重复
#define TaskRepeatDef 99999
