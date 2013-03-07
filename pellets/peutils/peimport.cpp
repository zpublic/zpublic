#include "peimport.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

CPEImportFunObject::CPEImportFunObject()
{
}

std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator CPEImportFunObject::NameBegin() const
{
    return m_ImportFunObject.begin();
}

std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator CPEImportFunObject::NameEnd() const
{
    return m_ImportFunObject.end();
}

std::vector<Define::uint64>::const_iterator CPEImportFunObject::IdBegin() const
{
    return m_ImportFunObjectId.begin();
}

std::vector<Define::uint64>::const_iterator CPEImportFunObject::IdEnd() const
{
    return m_ImportFunObjectId.end();
}

bool CPEImportFunObject::Copy(const CPEImportFunObject& cpyObject)
{
    if (cpyObject.IsVaild())
    {
        for (auto It = cpyObject.NameBegin(); It != cpyObject.NameEnd(); It++)
        {
            IMAGE_IMPORT_BY_NAME* tmp = new IMAGE_IMPORT_BY_NAME;
            ::memcpy(tmp, It->second, sizeof(IMAGE_IMPORT_BY_NAME));
            m_ImportFunObject[It->first] = tmp;
        }
        for (auto It = cpyObject.IdBegin(); It != cpyObject.IdEnd(); It++)
        {
            m_ImportFunObjectId.push_back(*It);
        }
        return true;
    }
    return false;
}

bool CPEImportFunObject::push_back(std::string strFunName, IMAGE_IMPORT_BY_NAME* pstImportFun)
{
    if ((!strFunName.empty()) && (pstImportFun != NULL))
    {
        IMAGE_IMPORT_BY_NAME* tmp = new IMAGE_IMPORT_BY_NAME;
        ::memcpy(tmp, pstImportFun, sizeof(IMAGE_IMPORT_BY_NAME));
        m_ImportFunObject[strFunName] = tmp;
        return true;
    }
    return false;
}

bool CPEImportFunObject::push_back(Define::uint64 FunId)
{
    m_ImportFunObjectId.push_back(FunId);
    return true;
}

IMAGE_IMPORT_BY_NAME* CPEImportFunObject::Get(const std::string& strObject) const
{
    for (auto It = m_ImportFunObject.begin(); It != m_ImportFunObject.end(); It++)
    {
        if (It->first == strObject)
        {
            return It->second;
        }
    }
    return 0;
}

bool CPEImportFunObject::IsVaild() const
{
    return true;
}

void CPEImportFunObject::Close()
{
    if (!m_ImportFunObject.empty())
    {
        for (auto It = m_ImportFunObject.begin(); It != m_ImportFunObject.end(); It++)
        {
            delete It->second;
        }
    }
}

CPEImportFunObject64::CPEImportFunObject64()
{
}

CPEImportFunObject64::~CPEImportFunObject64()
{
}

std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator CPEImportFunObject64::NameBegin() const
{
    return m_ImportFunObject.begin();
}

std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator CPEImportFunObject64::NameEnd() const
{
    return m_ImportFunObject.end();
}

std::vector<Define::uint128>::const_iterator CPEImportFunObject64::IdBegin() const
{
    return m_ImportFunObjectId.begin();
}

std::vector<Define::uint128>::const_iterator CPEImportFunObject64::IdEnd() const
{
    return m_ImportFunObjectId.end();
}

bool CPEImportFunObject64::Copy(const CPEImportFunObject64& cpyObject)
{
    if (cpyObject.IsVaild())
    {
        for (auto It = cpyObject.NameBegin(); It != cpyObject.NameEnd(); It++)
        {
            IMAGE_IMPORT_BY_NAME* tmp = new IMAGE_IMPORT_BY_NAME;
            ::memcpy(tmp, It->second, sizeof(IMAGE_IMPORT_BY_NAME));
            m_ImportFunObject[It->first] = tmp;
        }
        for (auto It = cpyObject.IdBegin(); It != cpyObject.IdEnd(); It++)
        {
            m_ImportFunObjectId.push_back(*It);
        }
        return true;
    }
    return false;
}

bool CPEImportFunObject64::push_back(std::string strFunName, IMAGE_IMPORT_BY_NAME* pstImportFun)
{
    if ((!strFunName.empty()) && (pstImportFun != NULL))
    {
        IMAGE_IMPORT_BY_NAME* tmp = new IMAGE_IMPORT_BY_NAME;
        ::memcpy(tmp, pstImportFun, sizeof(IMAGE_IMPORT_BY_NAME));
        m_ImportFunObject[strFunName] = tmp;
        return true;
    }
    return false;
}

bool CPEImportFunObject64::push_back(Define::uint128 FunId)
{
    m_ImportFunObjectId.push_back(FunId);
    return true;
}

IMAGE_IMPORT_BY_NAME* CPEImportFunObject64::Get(const std::string& strObject) const
{
    for (auto It = m_ImportFunObject.begin(); It != m_ImportFunObject.end(); It++)
    {
        if (It->first == strObject)
        {
            return It->second;
        }
    }
    return 0;
}

bool CPEImportFunObject64::IsVaild() const
{
    return true;
}

void CPEImportFunObject64::Close()
{
    if (!m_ImportFunObject.empty())
    {
        for (auto It = m_ImportFunObject.begin(); It != m_ImportFunObject.end(); It++)
        {
            delete It->second;
        }
    }
}

CPEImportFunObject::~CPEImportFunObject()
{
}

CPEImportObject::CPEImportObject()
{
}

CPEImportObject::CPEImportObject(char* lpszName, CPEImportFunObject& stImportFun)
{
    if (lpszName != NULL)
    {
        m_ImportName = lpszName;
        if (stImportFun.IsVaild())
        {
            m_ImportFunObject.Copy(stImportFun);
        }
    }
}

CPEImportObject::CPEImportObject(char* lpszName, CPEImportFunObject64& stImportFun)
{
    if (lpszName != NULL)
    {
        m_ImportName = lpszName;
        if (stImportFun.IsVaild())
        {
            m_ImportFunObject64.Copy(stImportFun);
        }
    }
}

CPEImportFunObject& CPEImportObject::Get()
{
    return m_ImportFunObject;
}

CPEImportFunObject64& CPEImportObject::Get64()
{
    return m_ImportFunObject64;
}

bool CPEImportObject::IsVaild()
{
    return true;
}

void CPEImportObject::Close()
{
}

CPEImportObject::~CPEImportObject()
{
}

}
}