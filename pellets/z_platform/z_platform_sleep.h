#pragma once
#include "z_platform_def.h"

#ifdef Z_PLATFORM_WIN

#include <Windows.h>
inline void zl_sleep(unsigned int millisecond)
{
    ::Sleep(millisecond);
}

#else

#include <unistd.h>
inline void zl_sleep(unsigned int millisecond)
{
    usleep(millisecond * 1000);
}

#endif
