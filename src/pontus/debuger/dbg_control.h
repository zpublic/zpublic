#pragma once

class CDbgControl
{
public:
    CDbgControl();
    ~CDbgControl();

public:
    void start(LPCTSTR lpFilePath);
    void stop();
    void g(BOOL bHandledException);
    void d(unsigned int address = 0, unsigned int length = 128);
    void r();
    void bp(unsigned int address);
    void bc(unsigned int address);
    void p();
    void t();
    void gu();
};