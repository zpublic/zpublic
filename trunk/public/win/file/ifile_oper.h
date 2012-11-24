/********************************************************************
* @file      : ifile_oper.h
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/24 13:32
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _I_FILE_OPER_HPP_
#define _I_FILE_OPER_HPP_

#include "./../../basic/interface.hpp"

#ifndef SEEK_SET
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#endif

namespace zl
{

    class IFileCtrl : public Interface
    {
    public:
        IFileCtrl()
        {
        }

        virtual ~IFileCtrl()
        {
        }

    public:
        virtual bool Open(
            const char* lpFilePath,
            const char* lpMode) = 0;
        virtual void Close() = 0;
    };

    class IFileRead : public Interface
    {
    public:
        IFileRead()
        {
        }

        virtual ~IFileRead()
        {
        }

    public:
        virtual unsigned int Read(
            void * lpData,
            unsigned int uSize,
            unsigned int uCount) = 0;
        virtual void Seek(
            long lOffset,
            int origin = SEEK_SET) = 0;
    };

    class IFileWrite : public Interface
    {
    public:
        IFileWrite()
        {
        }

        virtual ~IFileWrite()
        {
        }

    public:
        virtual unsigned int Write(
            const void * lpData,
            unsigned int uSize,
            unsigned int uCount) = 0;
    };

}

#endif