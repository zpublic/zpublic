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
#include "z_com_auto_ptr.hpp"
#include "z_com_module.hpp"

template<class T, const CLSID* pclsid = NULL, const IID* piid = &__uuidof(T)>
class ZLComInterfacePtr : public ZLComAutoPtr<T>
{
public:
    ZLComInterfacePtr(bool bInit = true) :
        m_hComDll(HMODULE(0)),
        ZLComAutoPtr<T>(NULL)
    {
        if (bInit)
            LoadSysInterface();
    }

    ZLComInterfacePtr(LPCTSTR pszComDllFile)
    {
        LoadInterface(pszComDllFile);
    }

    ZLComInterfacePtr(HMODULE hModule) : ZLComModuleHandle(hModule)
    {
        LoadInterface(hModule);
    }

    ~ZLComInterfacePtr()
    {
    }

public:
    BOOL LoadSysInterface()
    {
        CLSID clsid = { 0 };
        if (pclsid != NULL)
            clsid = (*pclsid);
        HRESULT hr = ZLComAutoPtr<T>::CoCreateInstance(clsid);
        return SUCCEEDED(hr);
    }

    BOOL LoadInterface(LPCTSTR pszComDllFile)
    {
        m_hComDll.LoadLibrary(pszComDllFile);
        HRESULT hr = CoCreateInstance();
        return SUCCEEDED(hr);
    }

    BOOL LoadInterface(HMODULE hModule)
    {
        HRESULT hr =CoCreateInstance();
        return SUCCEEDED(hr);
    }

    void Free()
    {
        Release();
        //if (SUCCEEDED(m_hComDll.DllCanUnloadNow()))
        {
            m_hComDll.FreeLibrary();
        }
    }

protected:
    HRESULT CoCreateInstance()
    {
        CLSID clsid = { 0 };
        IID   iid   = { 0 };

        if (pclsid != NULL)
            clsid = (*pclsid);
        if (piid != NULL)
            iid = (*piid);

        ZLComAutoPtr<IClassFactory> spClassFactory;
        HRESULT hRetCode = m_hComDll.DllGetClassObject(clsid, 
            IID_IClassFactory, (void**)&spClassFactory);
        if (SUCCEEDED(hRetCode) && spClassFactory != NULL )
        {
            hRetCode = spClassFactory->CreateInstance(NULL, iid, (void**)&m_pT);
            if (FAILED(hRetCode))
                return hRetCode;
        }
        else
        {
            hRetCode = m_hComDll.DllGetClassObject(clsid, 
                iid, (void**)&m_pT);
            if (FAILED(hRetCode))
                return hRetCode;
        }

        hRetCode = S_OK;
        return hRetCode;
    }
    ZLComInterfacePtr<T, pclsid, piid>& operator = (const ZLComInterfacePtr<T, pclsid, piid>&);

private:
    ZLComModuleHandle m_hComDll;
};
