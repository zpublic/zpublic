#ifndef _KXLOL_CUSTOMMSG_H
#define _KXLOL_CUSTOMMSG_H

#ifndef _WINDOWS_
#include <Windows.h>
#endif

enum
{
	KXLOL_WM_UPDATEGUI = WM_USER + 1024, // 刷新主界面 
};

#endif