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

template <class T>
class ZLComObject : public T
{
public:
    ZLComObject();
    ~ZLComObject();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    ULONG   STDMETHODCALLTYPE AddRef();
    ULONG   STDMETHODCALLTYPE Release();

private:
    LONG m_lRefCount;
};

template <class T>
ZLComObject<T>::ZLComObject() : m_lRefCount(0)
{
}

template <class T>
ZLComObject<T>::~ZLComObject()
{
}

template <class T>
HRESULT ZLComObject<T>::QueryInterface( REFIID riid, void **ppvObject )
{
    HRESULT hr = _InternalQueryInterface(riid, ppvObject);
    if (SUCCEEDED(hr))
    {
        AddRef();
    }
    return hr;
}

template <class T>
ULONG  ZLComObject<T>::AddRef()
{
    ::InterlockedIncrement(&m_lRefCount);
    return m_lRefCount;
}

template <class T>
ULONG  ZLComObject<T>::Release()
{
    ::InterlockedDecrement(&m_lRefCount);
    if (m_lRefCount == 0)
    {
        delete this;
        return 0;
    }
    return m_lRefCount;
}
