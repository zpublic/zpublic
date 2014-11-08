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
#include "stream_writter.hpp"
#include "z_http_interface.h"

namespace zl
{
    namespace http
    {
        class ZLMemWrite : public ICurlWrite
        {
        public:
            ZLMemWrite()
            {

            }

            virtual ~ZLMemWrite()
            {

            }

            virtual int Write(BYTE *pData, int nLength)
            {
                m_stream.WriteBinary(pData, nLength);
                return nLength;
            }

            virtual const BYTE* GetData()
            {
                return (const BYTE*)m_stream.GetStream();
            }

            virtual int GetLength()
            {
                return m_stream.GetSize();
            }

            ZLStreamWriter& GetStreamWriter()
            {
                return m_stream;
            }

        private:
            ZLStreamWriter m_stream;
        };
    }
}
