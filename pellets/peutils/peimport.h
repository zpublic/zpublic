/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_IMPORTOBJECT_H
#define ZL_PEUTILS_IMPORTOBJECT_H

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include "peutils_def.h"
#include "peobject.h"

namespace zl
{
namespace Peutils
{


class CPEImportFunObject : public CPEObject
{
public:
    CPEImportFunObject();
    virtual ~CPEImportFunObject();

    bool IsVaild() const;

    void Close();

    bool push_back(std::string strFunName, IMAGE_IMPORT_BY_NAME* pstImportFun);

    bool push_back(Define::uint64 FunId);

    std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator NameBegin() const;

    std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator NameEnd() const;

    std::vector<Define::uint64>::const_iterator IdBegin() const;

    std::vector<Define::uint64>::const_iterator IdEnd() const;

    bool Copy(const CPEImportFunObject& cpyObject);

    IMAGE_IMPORT_BY_NAME* Get(const std::string& strObject) const;

private:
    std::map<std::string, IMAGE_IMPORT_BY_NAME*> m_ImportFunObject;
    std::vector<Define::uint64> m_ImportFunObjectId;
};

class CPEImportFunObject64 : public CPEObject
{
public:
    CPEImportFunObject64();
    virtual ~CPEImportFunObject64();

    bool IsVaild() const;

    void Close();

    bool push_back(std::string strFunName, IMAGE_IMPORT_BY_NAME* pstImportFun);

    bool push_back(Define::uint128 FunId);

    std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator NameBegin() const;

    std::map<std::string, IMAGE_IMPORT_BY_NAME*>::const_iterator NameEnd() const;

    std::vector<Define::uint128>::const_iterator IdBegin() const;

    std::vector<Define::uint128>::const_iterator IdEnd() const;

    bool Copy(const CPEImportFunObject64& cpyObject);

    IMAGE_IMPORT_BY_NAME* Get(const std::string& strObject) const;

private:
    std::map<std::string, IMAGE_IMPORT_BY_NAME*> m_ImportFunObject;
    std::vector<Define::uint128> m_ImportFunObjectId;
};

class CPEImportObject
{
public:
    CPEImportObject(char* lpszName, CPEImportFunObject& stImportFun);
    CPEImportObject(char* lpszName, CPEImportFunObject64& stImportFun);
    virtual ~CPEImportObject();

    bool IsVaild();

    void Close();

    CPEImportFunObject& Get();
    CPEImportFunObject64& Get64();

private:
    CPEImportObject();

private:
    CPEImportFunObject m_ImportFunObject;
    CPEImportFunObject64 m_ImportFunObject64;
    std::string m_ImportName;
};

}
}

#endif