#pragma once

// 若这个zlib路径与你的路径冲突,请定义这个宏,并在stdafx.h包含你的zlib路径
#ifndef MY_COSTOM_ZLIB_DIR

#define ZLIB_WINAPI
#include "zlib/zlib.h"

#ifdef _DEBUG
#pragma comment(lib, "zlibstat_d.lib")
#else
#pragma comment(lib, "zlibstat.lib")
#endif

#endif
