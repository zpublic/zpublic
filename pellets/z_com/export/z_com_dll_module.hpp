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
#include "z_com_dll_lock.hpp"

class ZLComDllModule
{
public:
    ZLComDllModule(void);
    ~ZLComDllModule(void);

    void Init( HINSTANCE hInstance );
    HINSTANCE GetInstance() const;

    HRESULT DllCanUnloadNow(void);
    HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

private:
    HINSTANCE m_hInstance;
};

inline ZLComDllModule::ZLComDllModule(void) : m_hInstance(NULL)
{
}

inline ZLComDllModule::~ZLComDllModule(void)
{
}

inline void ZLComDllModule::Init( HINSTANCE hInstance )
{
    m_hInstance = hInstance;
}

inline HINSTANCE ZLComDllModule::GetInstance() const
{
    return m_hInstance;
}

inline HRESULT ZLComDllModule::DllCanUnloadNow(void)
{
    return ZLComDllLock::GetInstance()->GetLockCount() == 0 ? S_OK : E_FAIL;
}

HRESULT ZL_DllGetClassObject( REFIID riid, LPVOID* ppv );

inline HRESULT ZLComDllModule::DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID* ppv )
{
    if (rclsid == CLSID_NULL)
    {
        return ZL_DllGetClassObject(riid, ppv);
    }
    return E_FAIL;
}
