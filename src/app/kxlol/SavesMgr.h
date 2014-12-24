#pragma once
#include "SavesDef.h"

///> 存档管理类
class CSavesMgr
{
public:
    CSavesMgr();
    ~CSavesMgr();

public:
    bool Load()
    {
    }

    int Save()
    {
        int nRet = -1;
        KXLOL_SAVES_HEAD head;
        KXLOL_SAVES_DATA data;
        data.m_ullShenli = GAME.Player()->BasicInfo()->GetShenli();

        CString sPath = ZLPath::GetModuleFolder(NULL) + "data\\kxlol.dat";
        if (!ZLFileOperation::EnsureFileDir(sPath))
        {
            MessageBox(0, L"创建存档目录失败！玩毛", 0, 0);
            return -2;
        }

        HANDLE hFile = ::CreateFile(
            sPath,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            nRet = -3;
            DWORD dwWrite = 0;
            ::WriteFile(hFile, &head, sizeof(head), &dwWrite, NULL);
            if (dwWrite == sizeof(head))
            {
                ::WriteFile(hFile, &data, sizeof(data), &dwWrite, NULL);
                if (dwWrite == sizeof(data))
                {
                    nRet = 0;
                }
            }
            ::CloseHandle(hFile);
        }
        return nRet;
    }
};

