#pragma once

class CSystemInfoSupport : private boost::noncopyable
{
public:
    CSystemInfoSupport();
    ~CSystemInfoSupport();

public:
    int GetSystemVersion();
    std::wstring GetComputerName();
    int GetCpuNumber();
    int GetCpuCoreNumber();
};