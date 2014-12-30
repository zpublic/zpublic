#include "stdafx.h"
#include "TimerBaiscTask.h"


TimerBaiscTask::TimerBaiscTask()
{
}


TimerBaiscTask::~TimerBaiscTask()
{
}

void TimerBaiscTask::BasicMinuteTask()
{
    GAME.Player().BasicInfo().AddShenli(3);
    // todo:更新界面
}
