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
#include "z_http_header.h"

namespace zl
{
    namespace http
    {
        class ZLStreamWriter
        {
        public:
            ZLStreamWriter()
            {
                m_nSize = 1024;
                m_pBuffer = new unsigned char[m_nSize];
                m_nOffset = 0;
            }

            ~ZLStreamWriter()
            {
                if (m_pBuffer)
                    delete[] m_pBuffer;
            }

            void WriteBinary(const void* pBuffer, int nSize)
            {
                if (nSize + m_nOffset > m_nSize)
                    ReallocMem(m_nSize + nSize + 1024);

                memcpy(m_pBuffer + m_nOffset, pBuffer, nSize);
                m_nOffset += nSize;
            }

            void* GetStream(void)
            {
                return m_pBuffer;
            }

            int GetSize(void)
            {
                return m_nOffset;
            }

            void Clear(void)
            {
                m_nOffset = 0;
            }

        protected:
            void ReallocMem(int nSize)
            {
                unsigned char* pBuffer = new unsigned char[nSize];

                if (m_pBuffer && m_nOffset)
                    memcpy(pBuffer, m_pBuffer, m_nOffset);

                if (m_pBuffer)
                    delete[] m_pBuffer;

                m_pBuffer = pBuffer;
                m_nSize = nSize;
            }

        private:
            unsigned char* m_pBuffer;
            int m_nSize;
            int m_nOffset;
        };
    }
}
