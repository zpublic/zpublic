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
#include "z_com_interface_ptr.hpp"

template <typename TInterface>
class ZLComLoader
{
public:
    TInterface* GetInterface()
    {
        return (TInterface*)m_mudule;
    }

protected:
    ZLComInterfacePtr<TInterface> m_mudule;

public:
    HRESULT Initialize(const wchar_t* pszDllPath = NULL)
    {
        HRESULT hr = S_OK;
        BOOL bRetVal = FALSE;
        if (pszDllPath)
            bRetVal = m_mudule.LoadInterface(pszDllPath);
        if (bRetVal == FALSE)
        {
            hr = GetLastError();
            return hr;
        }
        return hr;
    }

    HRESULT UnInitialize()
    {
        m_mudule.Free();
        return S_OK;
    }
};
