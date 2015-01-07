#ifndef _KXLOL_CUSTOMMSG_H
#define _KXLOL_CUSTOMMSG_H

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#define KXLOL_WM_MSG_BEGIN      WM_USER + 1024

enum
{
    KXLOL_WM_UPDATEGUI          = KXLOL_WM_MSG_BEGIN + 1,       ///> 刷新主界面 
    KXLOL_WM_INFO_OUTPUT        = KXLOL_WM_MSG_BEGIN + 2,       ///> 输出一条信息
};

#endif