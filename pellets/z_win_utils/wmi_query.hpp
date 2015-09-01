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

/**
 * @file
 * @brief WMI 相关查询
 */


#pragma once
#include "win_utils_header.h"
#include "com_init.hpp"

#include <wbemcli.h>
#include <comutil.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")

namespace zl
{
namespace WinUtils
{
    /**
    * @brief WMI 相关查询
    */
    class ZLWMIQuery
    {
    public:
        ZLWMIQuery()
            : comInit(TRUE)
            , m_pServices(NULL)
            , m_pLocator(NULL)
        {
        }

        ~ZLWMIQuery()
        {
        }

    public:
        /**
        * @brief 初始化
        * @return 成功返回TRUE,失败FALSE
        */
        BOOL Init()
        {
            UnInit();
            BOOL bRet = FALSE;
            if (_GetWbemLocator(m_pLocator))
            {
                bRet = _GetWbemServices(m_pLocator, m_pServices);
            }
            return bRet;
        }

        /**
        * @brief 反初始化
        * @return 成功返回TRUE,失败FALSE
        */
        void UnInit()
        {
            if (m_pServices)
            {
                m_pServices->Release();
                m_pServices = NULL;
            }

            if (m_pLocator)
            {
                m_pLocator->Release();
                m_pLocator = NULL;
            }
        }

        /**
        * @brief 查询WMI信息
        * @param[in] cstrClass WMI类名 （类名对照地址 http://msdn.microsoft.com/en-us/library/dn792258(v=vs.85).aspx）
        * @param[in] cstrValueName WMI需查询的键值名
        * @param[out] vt 返回查询到的值 返回结果类型为VARIANT
        * @return 成功返回TRUE,失败FALSE
        */
        BOOL Query(const CStringA& cstrClass, const CString& cstrValueName, VARIANT& vt)
        {
            return _Query(cstrClass, cstrValueName, vt);
        }

    private:
        BOOL _GetWbemLocator(IWbemLocator*& pLoc)
        {
            HRESULT hres = ::CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID *) &pLoc);
            return SUCCEEDED(hres) ? TRUE : FALSE;
        }

        BOOL _GetWbemServices(IWbemLocator* pLoc, IWbemServices*& pSvc)
        {
            BOOL bRet = FALSE;
            if (pLoc != NULL)
            {
                HRESULT hres = pLoc->ConnectServer(
                    _bstr_t(L"ROOT\\CIMV2"),
                    NULL,
                    NULL,
                    0,
                    NULL,
                    0,
                    0,
                    &pSvc);
                if (SUCCEEDED(hres))
                {
                    hres = ::CoSetProxyBlanket(
                        pSvc,
                        RPC_C_AUTHN_WINNT,
                        RPC_C_AUTHZ_NONE,
                        NULL,
                        RPC_C_AUTHN_LEVEL_CALL,
                        RPC_C_IMP_LEVEL_IMPERSONATE,
                        NULL,
                        EOAC_NONE);
                    if (SUCCEEDED(hres))
                    {
                        bRet = TRUE;
                    }
                }
            }
            return bRet;
        }

        BOOL _GetEnumerator(IEnumWbemClassObject*& pEnumerator, const CStringA& cstrClass)
        {
            BOOL bRet = FALSE;
            if (m_pServices != NULL)
            {
                HRESULT hres = m_pServices->ExecQuery(
                    bstr_t("WQL"),
                    bstr_t("SELECT * FROM " + cstrClass),
                    WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                    NULL,
                    &pEnumerator);
                if (SUCCEEDED(hres))
                {
                    bRet = TRUE;
                }
            }
            return bRet;
        }

        BOOL _GetClassObjectValue(IEnumWbemClassObject* pEnumerator, const CString& cstrValueName, VARIANT& vt)
        {
            BOOL bRet = FALSE;
            IWbemClassObject *pclsObj = NULL;
            if (pEnumerator != NULL)
            {
                ULONG uReturn = 0;
                while (pEnumerator)
                {
                    HRESULT hr = pEnumerator->Next(WBEM_INFINITE,
                        1,
                        &pclsObj,
                        &uReturn);
                    if(0 == uReturn || pclsObj == NULL)
                    {
                        break;
                    }

                    VARIANT vtProp;
                    hr = pclsObj->Get(cstrValueName, 0, &vtProp, 0, 0);
                    if (FAILED(hr))
                    {
                        continue;
                    }

                    vt = vtProp;
                    bRet = TRUE;
                    break;
                }
            }

            if (pclsObj)
            {
                pclsObj->Release();
            }
            return bRet;
        }

        BOOL _Query(const CStringA& cstrClass, const CString& cstrValueName, VARIANT& vt)
        {
            BOOL bRet = FALSE;
            IEnumWbemClassObject* pEnumerator = NULL;
            if (_GetEnumerator(pEnumerator, cstrClass))
            {
                bRet = _GetClassObjectValue(pEnumerator, cstrValueName, vt);
            }

            if (pEnumerator)
            {
                pEnumerator->Release();
            }
            return bRet;
        }

    private:
        ZLComInit comInit;
        IWbemServices* m_pServices;
        IWbemLocator* m_pLocator;
    };
}
}