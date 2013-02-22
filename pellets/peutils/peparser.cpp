#include "peparser.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

CPEParser::CPEParser()
{
}

CPEParser::~CPEParser()
{
    m_File.Close();
    Close();
}

PEStatus CPEParser::OpenFile(TCHAR* lpszPath)
{
    if (!lpszPath)
    {
        return PEStatus_Err;
    }
    m_File.Open(lpszPath, PEFILE_R);
    if (!m_File.IsOpen())
    {
        return PEStatus_Err;
    }
    m_File.Seek(0, PEFILE_SK_SET);
    if (m_File.Read(&m_DosHead, sizeof(m_DosHead)) != sizeof(IMAGE_DOS_HEADER))
    {
        return PEStatus_Err;
    }
    ///>DOS MZ header
    if (!(m_DosHead.e_magic == IMAGE_DOS_SIGNATURE) && !(m_DosHead.e_lfanew < 0x1000))
    {
        return PEStatus_Err;
    }
    ///>PE header
    m_File.Seek(m_DosHead.e_lfanew, PEFILE_SK_SET);
    if (m_File.Read(&m_NtHead64, sizeof(m_NtHead64)))
    {
        ///>Section Table offset
        uint64 lnSectionTableOffset = 0;
        if (m_NtHead64.Signature != IMAGE_NT_SIGNATURE)
        {
            return PEStatus_Err;
        }
        ///>32
        if (m_NtHead64.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            m_IsX64 = false;
            memcpy(&m_NtHead32, &m_NtHead64, sizeof(m_NtHead32));
            memset(&m_NtHead64, 0, sizeof(m_NtHead64));
            lnSectionTableOffset = FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader) + m_NtHead32.OptionalHeader.SizeOfHeaders;
        }
        ///>64
        else if (m_NtHead64.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            m_IsX64 = true;
            lnSectionTableOffset = FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader) + m_NtHead32.OptionalHeader.SizeOfHeaders;
        }

        if (lnSectionTableOffset != 0)
        {
            ///>Add DOS MZ header Size
            lnSectionTableOffset += m_DosHead.e_lfanew;
            ///>
        }
    }
    return PEStatus_Ok;
}

bool CPEParser::IsExe()
{
    uint16 nSubType = GetSubSystemType();
    return ((nSubType == IMAGE_SUBSYSTEM_WINDOWS_GUI
        || nSubType == IMAGE_SUBSYSTEM_WINDOWS_CUI)
        && (GetFileHead()->Characteristics & IMAGE_FILE_DLL) == 0);
}

bool CPEParser::IsDLL()
{
    uint16 nSubType = GetSubSystemType();
    return ((nSubType == IMAGE_SUBSYSTEM_WINDOWS_GUI
        || nSubType == IMAGE_SUBSYSTEM_WINDOWS_CUI)
        && (GetFileHead()->Characteristics & IMAGE_FILE_DLL) != 0);
}

bool CPEParser::IsSys()
{
    uint16 nSubType = GetSubSystemType();
    return (nSubType == IMAGE_SUBSYSTEM_NATIVE);
}

PIMAGE_FILE_HEADER CPEParser::GetFileHead()
{
    return &(m_IsX64 ? m_NtHead64.FileHeader : m_NtHead32.FileHeader);
}

uint16 CPEParser::GetSubSystemType() const
{
    return m_IsX64 ? m_NtHead64.OptionalHeader.Subsystem : m_NtHead32.OptionalHeader.Subsystem;
}

PEStatus CPEParser::Close()
{
    if (m_File.IsOpen())
    {
        m_File.Close();
        ::memset(&m_NtHead32, 0, sizeof(m_NtHead32));
        ::memset(&m_NtHead64, 0, sizeof(m_NtHead64));
        m_IsX64 = false;
    }
    return PEStatus_Ok;
}

}
}