#pragma once
#include <string>

class CPropertyTree
{
public:
    CPropertyTree();
    ~CPropertyTree();

public:
    ///> 返回是否获取成功
    bool get(const std::wstring& key, std::wstring& value);
    ///> 返回是否已存在
    bool set(const std::wstring& key, const std::wstring& value, bool cover = true);
};