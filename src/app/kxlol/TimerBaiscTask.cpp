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
    CString sPath = ZLPath::GetModuleFolder(NULL) + "data\\timer_basic_minute_task.lua";
    if (::PathFileExists(sPath))
    {
        z_lua_state lua;
        if (lua.create() == 0)
        {
            GAME.Script().RegAll(lua);
            if (0 == lua.open_all_libs()
                && 0 == lua.dofile(ZLW2A(sPath)))
            {
                int n = lua.call<int>("dowork");
                ASSERT(n == 0);
            }
        }
    }
}
