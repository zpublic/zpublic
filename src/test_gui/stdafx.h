#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

///> c/c++库
#include <stdio.h>
#include <tchar.h>
#include <assert.h>

///> windows库
#include <windows.h>

///> gdi
#include <wingdi.h>


///> zpublic！
//#include "zpublic.hpp"

//////////////////////////////////////////////////////////////////////////
///> atl
#include <atlbase.h>
#include <atlwin.h>
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///> wtl
/*
 * 定义、包含和util类
 * 需要第一个包含
 */
#include "atlapp.h"
/*
 * 资源ID定义
 */
#include "atlres.h"
/*
 * 消息派发宏
 */
#include "atlcrack.h"
/*
 * _U_RECT
 * _U_MENUorID
 * _U_STRINGorID
 * 一些消息宏
 */
#include "atlwinx.h"
/*
 * 常用的一些类，CPoint、CString等
 * #define _WTL_NO_CSTRING
 */
#include "atlmisc.h"
/*
 * DDX实现
 */
#include "atlddx.h"
/*
 * 普通控件
 * 高级控件
 * CommandBar控件
 */
#include "atlctrls.h"
#include "atlctrlx.h"
#include "atlctrlw.h"
/*
 * 框架窗口封装
 */
#include "atlframe.h"
/*
 * 各种对话框封装
 */
#include "atldlgs.h"
/*
 * 编辑框查找功能封装
 */
#include "atlfind.h"
/*
 * GDI绘图封装
 */
#include "atlgdi.h"
/*
 * 资源处理封装
 */
#include "atluser.h"
/*
 * 窗口分割封装
 */
#include "atlsplit.h"
/*
 * 滚动条封装
 */
#include "atlscrl.h"
/*
 * 打印封装
 */
#include "atlprint.h"
/*
 * 桌面管理器DWM的Aero等封装
 * requires _WIN32_WINNT >= 0x0600
 */
//#include "atldwm.h"
/*
 * ribbon风格封装
 * requires the Windows 7 SDK or higher
 */
//#include "atlribbon.h"
/*
 * 系统皮肤风格封装
 */
//#include "atltheme.h"
//////////////////////////////////////////////////////////////////////////

extern CAppModule g_module;