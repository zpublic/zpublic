#include "stdafx.h"
#include "SavesMgr.h"


CSavesMgr::CSavesMgr()
{
}


CSavesMgr::~CSavesMgr()
{
}

bool CSavesMgr::Load()
{
    return false;
}

int CSavesMgr::Save()
{
    int nRet = -1;
    KXLOL_SAVES_HEAD head;
    KXLOL_SAVES_DATA data;
    data.m_ullShenli = GAME.Player()->BasicInfo()->GetShenli();

    CString sPath = ZLPath::GetModuleFolder(NULL) + "data\\kxlol.dat";
    if (!ZLFileOperation::EnsureFileDir(sPath))
    {
        MessageBox(0, L"´´½¨´æµµÄ¿Â¼Ê§°Ü£¡ÍæÃ«", 0, 0);
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
