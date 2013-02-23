#include "peparser.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

CPEParser::CPEParser()
    : m_DosHead_Ptr(NULL)
    , m_NtHead32_Ptr(NULL)
    , m_NtHead64_Ptr(NULL)
    , m_IsX64(false)
    , m_IsVaild(false)
{
}

CPEParser::~CPEParser()
{
    Close();
}

PEStatus CPEParser::Parse(CPEFile& PEFile)
{
    if (m_DosHead_Ptr != NULL)
    {
        delete m_DosHead_Ptr;
        m_DosHead_Ptr = new IMAGE_DOS_HEADER;
    }
    else
    {
        m_DosHead_Ptr = new IMAGE_DOS_HEADER;
    }

    if (m_NtHead32_Ptr != NULL)
    {
        delete m_NtHead32_Ptr;
        m_NtHead32_Ptr = new IMAGE_NT_HEADERS32;
    }
    else
    {
        m_NtHead32_Ptr = new IMAGE_NT_HEADERS32;
    }

    if (m_NtHead64_Ptr != NULL)
    {
        delete m_NtHead64_Ptr;
        m_NtHead64_Ptr = new IMAGE_NT_HEADERS64;
    }
    else
    {
        m_NtHead64_Ptr = new IMAGE_NT_HEADERS64;
    }

    if (!PEFile.IsOpen())
    {
        return PEStatus_Err;
    }
    PEFile.Seek(0, PEFILE_SK_SET);
    if (PEFile.Read(m_DosHead_Ptr, sizeof(IMAGE_DOS_HEADER)) != sizeof(IMAGE_DOS_HEADER))
    {
        return PEStatus_Err;
    }
    ///>DOS MZ header
    if (!(m_DosHead_Ptr->e_magic == IMAGE_DOS_SIGNATURE) && !(m_DosHead_Ptr->e_lfanew < 0x1000))
    {
        return PEStatus_Err;
    }
    ///>PE header
    PEFile.Seek(m_DosHead_Ptr->e_lfanew, PEFILE_SK_SET);
    if (PEFile.Read(m_NtHead64_Ptr, sizeof(IMAGE_NT_HEADERS64)))
    {
        ///>Section Table offset
        uint64 lnSectionTableOffset = 0;
        if (m_NtHead64_Ptr->Signature != IMAGE_NT_SIGNATURE)
        {
            return PEStatus_Err;
        }
        ///>32
        if (m_NtHead64_Ptr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
            m_IsX64 = false;
            m_IsVaild = true;
            memcpy(m_NtHead32_Ptr, m_NtHead64_Ptr, sizeof(IMAGE_NT_HEADERS32));
            if (m_NtHead64_Ptr != NULL)
            {
                delete m_NtHead64_Ptr;
                m_NtHead64_Ptr = NULL;
            }
            lnSectionTableOffset = FIELD_OFFSET(IMAGE_NT_HEADERS32, OptionalHeader)
                + m_NtHead32_Ptr->OptionalHeader.SizeOfHeaders;
        }
        ///>64
        else if (m_NtHead64_Ptr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            m_IsX64 = true;
            m_IsVaild = true;
            if (m_NtHead32_Ptr != NULL)
            {
                delete m_NtHead32_Ptr;
                m_NtHead32_Ptr = NULL;
            }
            lnSectionTableOffset = FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader)
                + m_NtHead64_Ptr->OptionalHeader.SizeOfHeaders;
        }

        if (lnSectionTableOffset != 0)
        {
            ///>Add DOS MZ header Size
            lnSectionTableOffset += m_DosHead_Ptr->e_lfanew;
            ///>Section Head
            uint32 nSectionNum = GetSectionNum();
            uint32 nSectionSize = sizeof(IMAGE_SECTION_HEADER) * nSectionNum;
            if (nSectionNum == -1)
            {
                return PEStatus_Err;
            }
            PIMAGE_SECTION_HEADER lpSectionHeader = new IMAGE_SECTION_HEADER[nSectionNum];
            PEFile.Seek(lnSectionTableOffset, PEFILE_SK_SET);
            if (PEFile.Read(lpSectionHeader, nSectionSize) != nSectionSize)
            {
                return PEStatus_Err;
            }
            for (uint32 index = 0; index < nSectionNum; index++)
            {
                m_SectionList.push_back(CPESectionObject(&lpSectionHeader[index]));
            }
            if (lpSectionHeader != NULL)
            {
                delete [] lpSectionHeader;
                lpSectionHeader = NULL;
            }
        }
    }
    return PEStatus_Ok;
}

bool CPEParser::IsExe()
{
    if (!IsVaild())
    {
        return false;
    }
    return ((GetFileHead()->Characteristics & IMAGE_FILE_DLL) == 0);
}

bool CPEParser::IsDLL()
{
    if (!IsVaild())
    {
        return false;
    }
    return ((GetFileHead()->Characteristics & IMAGE_FILE_DLL) != 0);
}

bool CPEParser::IsSys()
{
    if (!IsVaild())
    {
        return false;
    }
    uint16 nSubType = GetSubSystemType();
    return (nSubType == IMAGE_SUBSYSTEM_NATIVE);
}

bool CPEParser::IsVaild() const
{
    if (!m_IsVaild)
    {
        return false;
    }
    if (!m_NtHead32_Ptr && !m_NtHead64_Ptr)
    {
        return false;
    }
    return true;
}

PIMAGE_FILE_HEADER CPEParser::GetFileHead()
{
    if (!m_NtHead32_Ptr && !m_NtHead64_Ptr)
    {
        return 0;
    }
    return &(m_IsX64 ? m_NtHead64_Ptr->FileHeader : m_NtHead32_Ptr->FileHeader);
}

uint32 CPEParser::GetSectionNum() const
{
    if (!IsVaild())
    {
        return -1;
    }
    return (m_IsX64
        ? m_NtHead64_Ptr->FileHeader.NumberOfSections
        : m_NtHead32_Ptr->FileHeader.NumberOfSections);
}

uint16 CPEParser::GetSubSystemType() const
{
    if (!m_NtHead32_Ptr && !m_NtHead64_Ptr)
    {
        return -1;
    }
    return (m_IsX64
        ? m_NtHead64_Ptr->OptionalHeader.Subsystem
        : m_NtHead32_Ptr->OptionalHeader.Subsystem);
}

PEStatus CPEParser::Close()
{
    if (m_DosHead_Ptr != NULL)
    {
        delete m_DosHead_Ptr;
        m_DosHead_Ptr = NULL;
    }
    if (m_NtHead64_Ptr != NULL)
    {
        delete m_NtHead64_Ptr;
        m_NtHead64_Ptr = NULL;
    }
    if (m_NtHead32_Ptr != NULL)
    {
        delete m_NtHead32_Ptr;
        m_NtHead32_Ptr = NULL;
    }
    m_IsX64 = false;
    m_IsVaild = false;
    return PEStatus_Ok;
}

}
}