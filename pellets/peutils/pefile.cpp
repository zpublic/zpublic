#include "pefile.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

uint32 CPEFile::_ConverFileType(PEFileType pefileType)
{
    switch (pefileType)
    {
        case PEFILE_R: return OPEN_EXISTING;
        case PEFILE_W: return OPEN_EXISTING;
        case PEFILE_A: return OPEN_ALWAYS;
        case PEFILE_ATO_A: return OPEN_EXISTING|OPEN_ALWAYS;
        case PEFILE_ATO_R: return OPEN_EXISTING|OPEN_ALWAYS;
        case PEFILE_ATO_W: return OPEN_EXISTING|OPEN_ALWAYS;
    }
    return -1;
}

CPEFile::CPEFile() : m_File_Ptr(0)
{
}

CPEFile::~CPEFile()
{
    Close();
}

PEStatus CPEFile::Open(TCHAR* lpszPath, PEFileType pefileType)
{
    if (!lpszPath)
    {
        return PEStatus_Err;
    }
    m_File_Ptr = CreateFile(lpszPath, GENERIC_READ, FILE_SHARE_READ, NULL, _ConverFileType(pefileType), 0, NULL);
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
    DWORD RetSizeRead = 0;
    if (!(::ReadFile(m_File_Ptr, pData, nDataSize, &RetSizeRead, NULL)))
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
    DWORD RetSizeWrite = 0;
    if (!(::WriteFile(m_File_Ptr, pData, nDataSize, &RetSizeWrite, NULL)))
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
    if (::SetFilePointer(m_File_Ptr, nSeekPos, 0, peSeek) == 0)
    {
        return PEStatus_Err;
    }
    return PEStatus_Ok;
}

PEStatus CPEFile::Close()
{
    if (m_File_Ptr != NULL)
    {
        CloseHandle(m_File_Ptr);
        m_File_Ptr = NULL;
    }
    return PEStatus_Ok;
}

bool CPEFile::IsOpen()
{
    if (m_File_Ptr == NULL)
    {
        return false;
    }
    return true;
}

}
}