/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_PEPARSER_H
#define ZL_PEUTILS_PEPARSER_H

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "pefile.h"
#include "pesectionobject.h"
#include "peimport.h"

namespace zl
{
namespace Peutils
{

class CPEParser
{
public:
    CPEParser();
    virtual ~CPEParser();

    PEStatus Parse(CPEFile& PEFile);

    PEStatus Close();

    bool IsVaild() const;

    bool IsExe();

    bool IsDLL();

    bool IsSys();

    Define::uint16 GetSubSystemType() const;

    Define::uint32 GetSectionNum() const;

    PIMAGE_FILE_HEADER GetFileHead();

protected:
    Define::uint64 _ConverOffsetOfRVA(Define::uint64 nRVA);

    Define::uint128 _ConverOffsetOfRVA(Define::uint128 nRVA);

    PEStatus _ParseImport32(CPEFile& PEFile);

    PEStatus _ParseImport64(CPEFile& PEFile);

private:
    bool m_IsX64;
    bool m_IsVaild;
    IMAGE_DOS_HEADER*        m_DosHead_Ptr;
    IMAGE_NT_HEADERS32*      m_NtHead32_Ptr;
    IMAGE_NT_HEADERS64*      m_NtHead64_Ptr;
    std::vector<CPESectionObject> m_SectionList;
    std::vector<CPEImportObject> m_ImporObjecttList;
};

}
}

#endif