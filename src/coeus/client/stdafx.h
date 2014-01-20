// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER          0x0600
#define _WIN32_WINNT    0x0601
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atltrace.h>

extern CAppModule _Module;

#include <atlwin.h>

#define _WTL_NO_CSTRING

#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlframe.h>

#include <tinyxml/tinyxml.h>
#include <bkwin/bkdlgview.h>
#include <bkres/bkres.h>
#include <wtlhelper/whwindow.h>
#include <CBkDialogViewImplEx.h>

#include <Windows.h>

#include <vector>
using namespace std;

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "msg_def.h"

///> NET
#include "venus_net/common.h"
#include "venus_net/stream_writer.h"
#include "venus_net/stream_reader.h"
#include "protocol/game_error.h"
#include "protocol/opcodes.h"
#include "protocol/protocol.h"
#include "network_mgr.h"
#include "game_logic.h"
#include "msg_def.h"
#include "game_common/game_define.h"
using namespace Protocol;
