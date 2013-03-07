#include "peparser.h"
#include <iostream>
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
    m_SectionList.clear();
    m_ImporObjecttList.clear();

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
            lnSectionTableOffset = m_NtHead32_Ptr->FileHeader.SizeOfOptionalHeader;
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
            lnSectionTableOffset = m_NtHead64_Ptr->FileHeader.SizeOfOptionalHeader;
        }

        if (lnSectionTableOffset != 0)
        {
            ///>Add DOS MZ header Size
            lnSectionTableOffset += m_DosHead_Ptr->e_lfanew + sizeof(uint32) + sizeof(IMAGE_FILE_HEADER);
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
        if (m_IsX64)
        {
            return _ParseImport64(PEFile);
        }
        else
        {
            return _ParseImport32(PEFile);
        }
    }
    return PEStatus_Err;
}

PEStatus CPEParser::_ParseImport32(CPEFile& PEFile)
{
    if (!PEFile.IsOpen())
    {
        return PEStatus_Err;
    }

    uint64 posVAddr = m_NtHead32_Ptr->OptionalHeader.DataDirectory[1].VirtualAddress;
    uint32 nImportSize = m_NtHead32_Ptr->OptionalHeader.DataDirectory[1].Size;

    uint32 nImportNum = nImportSize / sizeof(IMAGE_IMPORT_DESCRIPTOR);
    if (posVAddr == 0 && nImportNum != 0)
    {
        return PEStatus_Err;
    }
    PIMAGE_IMPORT_DESCRIPTOR pstImport = new IMAGE_IMPORT_DESCRIPTOR[nImportNum];
    PEFile.Seek(_ConverOffsetOfRVA(posVAddr), PEFILE_SK_SET);
    if (PEFile.Read(pstImport, nImportSize)
        != nImportSize)
    {
        return PEStatus_Err;
    }

    for (uint32 index = 0; index < nImportNum; index++)
    {
        CPEImportFunObject peFunObject;
        char lpszTmpName[MAX_PATH] = {0};
        PEFile.Seek(_ConverOffsetOfRVA(pstImport[index].Name), PEFILE_SK_SET);
        if (PEFile.Read(lpszTmpName, MAX_PATH) != MAX_PATH)
        {
            return PEStatus_Err;
        }
        uint64 nFunType32 = 0;
        uint64 nOrdinal32 = 0;
        if (pstImport[index].OriginalFirstThunk != NULL)
        {
            IMAGE_THUNK_DATA32 stThunkData32;
            for (uint32 nThunkNum = 0; ;nThunkNum++)
            {
                PEFile.Seek(_ConverOffsetOfRVA(pstImport[index].OriginalFirstThunk)
                    + (sizeof(IMAGE_THUNK_DATA32) * nThunkNum), PEFILE_SK_SET);
                if (PEFile.Read(&stThunkData32, sizeof(IMAGE_THUNK_DATA32))
                    != sizeof(IMAGE_THUNK_DATA32))
                {
                    return PEStatus_Err;
                }
                nFunType32 = stThunkData32.u1.AddressOfData & IMAGE_ORDINAL_FLAG32;
                nOrdinal32 = stThunkData32.u1.Ordinal;
                if (stThunkData32.u1.AddressOfData == 0)
                {
                    break;
                }
                if (nFunType32 == 0)
                {
                    /*Name*/
                    IMAGE_IMPORT_BY_NAME ImportFoName;
                    PEFile.Seek(_ConverOffsetOfRVA(nOrdinal32), PEFILE_SK_SET);
                    if (PEFile.Read(&ImportFoName, sizeof(IMAGE_IMPORT_BY_NAME))
                        != sizeof(IMAGE_IMPORT_BY_NAME))
                    {
                        return PEStatus_Err;
                    }
                    std::string strOrdinalName;
                    PEFile.Seek(_ConverOffsetOfRVA(nOrdinal32) + sizeof(uint16), PEFILE_SK_SET);
                    while (true)
                    {
                        uint8 nTmp = 0;
                        if (PEFile.Read(&nTmp, sizeof(uint8))
                            != sizeof(uint8))
                        {
                            return PEStatus_Err;
                        }
                        if (nTmp == 0)
                        {
                            break;
                        }
                        strOrdinalName += nTmp;
                    }
                    peFunObject.push_back(strOrdinalName, &ImportFoName);
                }
                else
                {
                    /*serial number*/
                    peFunObject.push_back(nFunType32);
                }
            }
            m_ImporObjecttList.push_back(CPEImportObject(lpszTmpName, peFunObject));
        }
    }
    if (pstImport != NULL)
    {
        delete [] pstImport;
        pstImport = NULL;
    }
    return PEStatus_Ok;
}

PEStatus CPEParser::_ParseImport64(CPEFile& PEFile)
{
    if (!PEFile.IsOpen())
    {
        return PEStatus_Err;
    }
    uint128 posVAddr = m_NtHead64_Ptr->OptionalHeader.DataDirectory[1].VirtualAddress;
    uint32 nImportSize = m_NtHead64_Ptr->OptionalHeader.DataDirectory[1].Size;

    uint32 nImportNum = nImportSize / sizeof(IMAGE_IMPORT_DESCRIPTOR);
    if (posVAddr == 0 && nImportNum != 0)
    {
        return PEStatus_Err;
    }
    PIMAGE_IMPORT_DESCRIPTOR pstImport = new IMAGE_IMPORT_DESCRIPTOR[nImportNum];
    PEFile.Seek64(_ConverOffsetOfRVA(posVAddr), PEFILE_SK_SET);
    if (PEFile.Read(pstImport, nImportSize)
        != nImportSize)
    {
        return PEStatus_Err;
    }

    for (uint32 index = 0; index < nImportNum; index++)
    {
        CPEImportFunObject64 peFunObject;
        char lpszTmpName[MAX_PATH] = {0};
        PEFile.Seek(_ConverOffsetOfRVA(pstImport[index].Name), PEFILE_SK_SET);
        if (PEFile.Read(lpszTmpName, MAX_PATH) != MAX_PATH)
        {
            return PEStatus_Err;
        }
        uint128 nFunType64 = 0;
        uint128 nOrdinal64 = 0;
        if (pstImport[index].OriginalFirstThunk != NULL)
        {
            IMAGE_THUNK_DATA64 stThunkData64;
            for (uint32 nThunkNum = 0; nThunkNum != 0xFFFFFF;nThunkNum++)
            {
                PEFile.Seek64(_ConverOffsetOfRVA(pstImport[index].OriginalFirstThunk)
                    + (sizeof(IMAGE_THUNK_DATA64) * nThunkNum), PEFILE_SK_SET);
                if (PEFile.Read(&stThunkData64, sizeof(IMAGE_THUNK_DATA64))
                    != sizeof(IMAGE_THUNK_DATA64))
                {
                    return PEStatus_Err;
                }
                nFunType64 = stThunkData64.u1.AddressOfData & IMAGE_ORDINAL_FLAG64;
                nOrdinal64 = stThunkData64.u1.Ordinal;

                if (stThunkData64.u1.AddressOfData == 0)
                {
                    break;
                }
                if (nFunType64 == 0)
                {
                    /*Name*/
                    IMAGE_IMPORT_BY_NAME ImportFoName;
                    PEFile.Seek64(_ConverOffsetOfRVA(nOrdinal64), PEFILE_SK_SET);
                    if (PEFile.Read(&ImportFoName, sizeof(IMAGE_IMPORT_BY_NAME))
                        != sizeof(IMAGE_IMPORT_BY_NAME))
                    {
                        return PEStatus_Err;
                    }
                    std::string strOrdinalName;
                    PEFile.Seek64(_ConverOffsetOfRVA(nOrdinal64) + sizeof(uint16), PEFILE_SK_SET);
                    while (true)
                    {
                        uint8 nTmp = 0;
                        if (PEFile.Read(&nTmp, sizeof(uint8))
                            != sizeof(uint8))
                        {
                            return PEStatus_Err;
                        }
                        if (nTmp == 0)
                        {
                            break;
                        }
                        strOrdinalName += nTmp;
                    }
                    peFunObject.push_back(strOrdinalName, &ImportFoName);
                }
                else
                {
                    /*serial number*/
                    peFunObject.push_back(nFunType64);
                }
            }
            m_ImporObjecttList.push_back(CPEImportObject(lpszTmpName, peFunObject));
        }
    }
    if (pstImport != NULL)
    {
        delete [] pstImport;
        pstImport = NULL;
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

uint64 CPEParser::_ConverOffsetOfRVA(uint64 nRVA)
{
    for (auto It = m_SectionList.begin(); It != m_SectionList.end(); ++It)
    {
        if (((*It).Get().VirtualAddress <= nRVA)
            && ((*It).Get().SizeOfRawData + (*It).Get().VirtualAddress >= nRVA))
        {
            return (*It).Get().PointerToRawData + (nRVA - (*It).Get().VirtualAddress);
        }
    }
    return 0;
}

uint128 CPEParser::_ConverOffsetOfRVA(uint128 nRVA)
{
    for (auto It = m_SectionList.begin(); It != m_SectionList.end(); ++It)
    {
        if (((*It).Get().VirtualAddress <= nRVA)
            && ((*It).Get().SizeOfRawData + (*It).Get().VirtualAddress >= nRVA))
        {
            return (*It).Get().PointerToRawData + (nRVA - (*It).Get().VirtualAddress);
        }
    }
    return 0;
}

}
}