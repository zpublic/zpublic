/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/


#pragma once
#include <windows.h>


// #ifndef __ATLSTR_H__
// #include "cstring.hpp"
// #endif

#include <atlstr.h>
#include <assert.h>

#include "z_http_def.h"

// 若这个curl路径与你的路径冲突,请定义这个宏,并在stdafx.h包含你的curl路径
#ifndef MY_COSTOM_CURL_DIR

///> libcurl
#ifdef _DEBUG
#pragma comment(lib, "libcurl_d.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif
#define CURL_STATICLIB
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#include <libcurl/curl.h>

#endif
