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
 * @brief к╫сп╤я
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{

    class ZLPrivateHeap
    {
    private:
        ZLPrivateHeap(const ZLPrivateHeap&);
        ZLPrivateHeap operator = (const ZLPrivateHeap&);

    public:
        ZLPrivateHeap(DWORD dwOptions = 0, SIZE_T nInitSize = 0, SIZE_T nMaxSize = 0)
            : m_dwOption(dwOptions)
            , m_nInitSize(nInitSize)
            , m_nMaxSize(nMaxSize)
        {
            m_hHeap = ::HeapCreate(m_dwOption, nInitSize, nMaxSize);
        }
        
        ~ZLPrivateHeap()
        {
            if (IsValid())
            {
                ::HeapDestroy(m_hHeap);
            }
        }

        operator HANDLE() { return m_hHeap; }

    public:
        BOOL IsValid()
        {
            return m_hHeap != NULL ? TRUE : FALSE;
        }

        BOOL Reset()
        {
            if (IsValid())
            {
                ::HeapDestroy(m_hHeap);
            }
            m_hHeap = ::HeapCreate(m_dwOption, m_nInitSize, m_nMaxSize);
            return IsValid();
        }

        PVOID Alloc(SIZE_T size, DWORD dwFlag = 0)
        {
            return ::HeapAlloc(m_hHeap, dwFlag, size);
        }

        PVOID ReAlloc(PVOID pMem, SIZE_T size, DWORD dwFlag = 0)
        {
            return ::HeapReAlloc(m_hHeap, dwFlag, pMem, size);
        }

        SIZE_T Size(PVOID pMem, DWORD dwFlag = 0)
        {
            return ::HeapSize(m_hHeap, dwFlag, pMem);
        }

        BOOL Free(PVOID pMem, DWORD dwFlag = 0)
        {
            return ::HeapFree(m_hHeap, dwFlag, pMem);
        }

        SIZE_T Compact(DWORD dwFlag = 0)
        {
            return ::HeapCompact(m_hHeap, dwFlag);
        }

    private:
        HANDLE          m_hHeap;
        DWORD           m_dwOption;
        SIZE_T          m_nInitSize;
        SIZE_T          m_nMaxSize;
    };

}
}
