#pragma once

/************************************************************************/
/*  关注在c++11之前，无法在windows和android（linux）上共用的平台相关代码
/*  原则：
/*    1，超轻量，无标准库之外的外部依赖，内部尽可能少的依赖
/*    2，只关注无法共用的代码，不封装标准库里有的、可以共用的部分
/**************************************************************************/

#include "z_platform_def.h"
#include "z_platform_mutex.h"
#include "z_platform_event.h"
#include "z_platform_sleep.h"
#include "z_platform_thread.h"
