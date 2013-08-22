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
#ifndef ZL_I_HOOK_CONTAINER_H
#define ZL_I_HOOK_CONTAINER_H

interface IHookContainer
{
    //************************************
    // 添加API hook
    // Returns:   BOOL
    // Parameter: LPCSTR pszDLLName     要hook的dll名
    // Parameter: LPCSTR pszApiName     要hook的api名
    // Parameter: PVOID pvHookProxyFunc 代理函数地址
    // Parameter: PVOID * ppvRealAddr   执行完代理函数后跳回去执行的stub
    // Parameter: LPCWSTR lpName        该hook的别名，用于unhook，为NULL则
    // 使用代理函数地址来unhook
    //************************************
    virtual BOOL AddApiHook(
        LPCSTR pszDLLName,
        LPCSTR pszApiName, 
        PVOID pvHookProxyFunc, 
        OUT PVOID* ppvRealAddr,
        LPCWSTR lpName = NULL
        ) = 0;

    //************************************
    // Method:    添加地址 hook
    // Returns:   BOOL
    // Parameter: PVOID pvHookTarget    要hook的地址
    // Parameter: PVOID pvHookProxyFunc 代理函数地址
    // Parameter: PVOID * ppvRealAddr   执行完代理函数后跳回去执行的stub
    // Parameter: LPCWSTR lpName        该hook的别名，用于unhook，为NULL则
    // 使用代理函数地址来unhook
    //************************************
    virtual BOOL AddAddrHook(
        PVOID pvHookTarget,  
        PVOID pvHookProxyFunc, 
        PVOID* ppvRealAddr,
        LPCWSTR lpName = NULL
        ) = 0;

    ///> 使用代理函数地址来unhook
    virtual BOOL AddUnhookRoutine(LPVOID p_proxy_routine_address) = 0;

    ///> 使用函数别名的unhook
    virtual BOOL AddunhookName(LPCWSTR lpName) = 0;
};

#endif