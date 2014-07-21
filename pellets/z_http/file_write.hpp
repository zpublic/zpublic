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
#include "z_http_interface.h"

namespace zl
{
    namespace http
    {
        class ZLFileWrite : public ICurlWrite
        {
        public:
            ZLFileWrite(LPCTSTR szPath)
            {
                m_pFile = NULL;
                m_nLength = 0;
                _tfopen_s(&m_pFile, szPath, _T("wb"));
            }

            virtual ~ZLFileWrite()
            {
                if (m_pFile)
                {
                    fclose(m_pFile);
                    m_pFile = NULL;
                    m_nLength = 0;
                }
            }

            virtual int Write(BYTE *pData, int nLength)
            {
                if (m_pFile)
                    fwrite(pData, 1, nLength, m_pFile);

                m_nLength += nLength;
                return nLength;
            }

            virtual const BYTE* GetData()
            {
                return NULL;
            }

            virtual int GetLength()
            {
                return m_nLength;
            }

        private:
            ZLFileWrite() {}

            int m_nLength;
            FILE *m_pFile;
        };
    }
}
