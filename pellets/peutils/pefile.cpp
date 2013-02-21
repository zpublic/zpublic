#include "pefile.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

TCHAR* CPEFile::_ConverFileType(PEFileType pefileType)
{
    switch (pefileType)
    {
        case PEFILE_R: return (_T("r"));
        case PEFILE_W: return (_T("w"));
        case PEFILE_A: return  (_T("a"));
        case PEFILE_ATO_A: return (_T("a+"));
        case PEFILE_ATO_R: return (_T("r+"));
        case PEFILE_ATO_W: return (_T("w+"));
    }
    return _T("");
}

CPEFile::CPEFile() : m_File_Ptr(0)
{
}

CPEFile::~CPEFile()
{
}

PEStatus CPEFile::Open(TCHAR* lpszPath, PEFileType pefileType)
{
    if (!lpszPath)
    {
        return PEStatus_Err;
    }
    if (::_tfopen_s(&m_File_Ptr, lpszPath, _ConverFileType(pefileType)) != 0)
    {
        return PEStatus_Err;
    }
    if (m_File_Ptr == NULL)
    {
        return PEStatus_Err;
    }
    return PEStatus_Ok;
}

uint32 CPEFile::Read(void* pData, Define::uint32 nDataSize)
{
    if (!IsOpen())
    {
        return -1;
    }
    int RetSizeRead = ::fread(pData, nDataSize, 1, m_File_Ptr);
    if (RetSizeRead == 0)
    {
        return -1;
    }
    return RetSizeRead;
}

uint32 CPEFile::Write(void* pData, Define::uint32 nDataSize)
{
    if (!IsOpen())
    {
        return -1;
    }
    int RetSizeWrite = ::fwrite(pData, nDataSize, 1, m_File_Ptr);
    if (RetSizeWrite == -1)
    {
        return -1;
    }
    return RetSizeWrite;
}

PEStatus CPEFile::Seek(Define::uint32 nSeekPos, PEFileSeekType peSeek)
{
    if (!IsOpen())
    {
        return PEStatus_Err;
    }
    if (::fseek(m_File_Ptr, nSeekPos, peSeek) == -1)
    {
        return PEStatus_Err;
    }
    return PEStatus_Ok;
}

bool CPEFile::IsOpen()
{
    if (!m_File_Ptr)
    {
        return false;
    }
    return true;
}

}
}