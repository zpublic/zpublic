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
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#ifndef ZL_HOOK_DEF_H
#define ZL_HOOK_DEF_H

#include <Windows.h>

#define ZL_GET_REAL_ADDR(_api) gRealAddr_##_api
#define ZL_DEF_REAL_ADDR(_api) LPVOID ZL_GET_REAL_ADDR(_api) = NULL
#define ZL_CALL_REAL_API(_api, apiType)  ((apiType)(ZL_GET_REAL_ADDR(_api)))

#define ZL_ADD_HOOK_API(apiContainer, _dll, _api) \
    (apiContainer)->AddApiHook(_dll, #_api, Proxy_##_api, &ZL_GET_REAL_ADDR(_api))

#define ZL_ADD_UNHOOK_API(apiContainer, _api) \
    (apiContainer)->AddUnhookRoutine(Proxy_##_api)

#endif