/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_PEFILE_H
#define ZL_PEUTILS_PEFILE_H

#include <tchar.h>
#include <stdio.h>
#include "peutils_def.h"
#include <Windows.h>

namespace zl
{
namespace Peutils
{

class CPEFile
{
public:
    CPEFile();
    virtual ~CPEFile();

    PEStatus Open(TCHAR* lpszPath, PEFileType pefileType);

    PEStatus Close();

    bool IsOpen();

    Define::uint32 Read(void* pData, Define::uint32 nDataSize);

    Define::uint32 Write(void* pData, Define::uint32 nDataSize);

    PEStatus Seek(Define::uint32 nSeekPos, PEFileSeekType peSeek);

private:

    Define::uint32 _ConverFileType(PEFileType pefileType);

private:
    HANDLE m_File_Ptr;
};

}
}

#endif