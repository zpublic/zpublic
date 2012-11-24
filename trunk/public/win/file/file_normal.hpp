/********************************************************************
* @file      : file_normal.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/24 13:59
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _FILE_NORMAL_HPP_
#define _FILE_NORMAL_HPP_

#include "ifile_oper.h"
#include <stdlib.h>

namespace zl
{

    class CFileNormal
        : public IFileCtrl
        , public IFileRead
        , public IFileWrite
    {
    public:
        CFileNormal() : m_pf(0)
        {
        }

        virtual ~CFileNormal()
        {
            Close();
        }

    public:
        virtual bool Open(
            const char* lpFilePath,
            const char* lpMode)
        {
            Close();
            if (::fopen_s(&m_pf, lpFilePath, lpMode)) m_pf = 0;
            return m_pf ? true : false;
        }
        virtual void Close()
        {
            if (m_pf) ::fclose(m_pf);
        }
        virtual unsigned int Read(
            char* lpData,
            unsigned int uSize,
            unsigned int uCount)
        {
            unsigned int uRead = 0;
            if (m_pf)
            {
                uRead = ::fread(lpData, uSize, uCount, m_pf);
            }
            return uRead;
        }
        virtual unsigned int Write(
            const char* lpData,
            unsigned int uSize,
            unsigned int uCount)
        {
            unsigned int uWrite = 0;
            if (m_pf)
            {
                uWrite = ::fwrite(lpData, uSize, uCount, m_pf);
            }
            return uWrite;
        }

    private:
        mutable FILE *m_pf;
    };

}

#endif