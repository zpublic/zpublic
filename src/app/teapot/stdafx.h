// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include <atlstr.h>
#define _WTL_NO_CSTRING

// 如果必须要针对低于以下指定版本的平台，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0400		// 将此值更改为适当的值，以适用于 Windows 98 和 Windows 2000 及更高版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为适当的值，以适用于 Windows 2000 或更高版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以适用于 Windows Me 或更高版本。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0400	// 将此值更改为适当的值，以适用于 IE 5.0 或更新版本。
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 ATL 对某些常见但经常可放心忽略的警告消息的隐藏
#define _ATL_ALL_WARNINGS


#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
	//下面的 #import 导入 IDTExtensibility2 接口
	#import <MSADDNDR.DLL> raw_interfaces_only named_guids

	//下面的 #import 导入 VS 命令栏
	#import <vsmso.olb> raw_interfaces_only named_guids

	//下面的 #import 导入 DTE
	#import <dte80a.olb> raw_interfaces_only named_guids

	//下面的 #import 导入 DTE80
	#import <dte80.olb> raw_interfaces_only named_guids

	//下面的 #import 导入 DTE90
	#import <dte90.olb> raw_interfaces_only named_guids

	//下面的 #import 导入 DTE90a
	#import <dte90a.olb> raw_interfaces_only named_guids

	//下面的 #import 导入 DTE100
	#import <dte100.olb> raw_interfaces_only named_guids
#pragma warning( default : 4146 )
#pragma warning( default : 4278 )

#define IfFailGo(x) { hr=(x); if (FAILED(hr)) goto Error; }
#define IfFailGoCheck(x, p) { hr=(x); if (FAILED(hr)) goto Error; if(!p) {hr = E_FAIL; goto Error; } }

class DECLSPEC_UUID("03922356-207E-4192-8DF3-B9CDBC51A930") teapotLib;

using namespace ATL;

class CAddInModule : public CAtlDllModuleT< CAddInModule >
{
public:
	CAddInModule()
	{
		m_hInstance = NULL;
	}

	DECLARE_LIBID(__uuidof(teapotLib))

	inline HINSTANCE GetResourceInstance()
	{
		return m_hInstance;
	}

	inline void SetResourceInstance(HINSTANCE hInstance)
	{
		m_hInstance = hInstance;
	}

private:
	HINSTANCE m_hInstance;
};

extern CAddInModule _AtlModule;

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


///> json
#include <json/json.h>

