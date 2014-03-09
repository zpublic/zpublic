// coeus.cpp : main source file for coeus.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include "game_login_dlg.h"
#include "config_loader.h"
#include "res_img_pool.h"
#include "game_common\config\config_manager.h"

#include "resource.h"
#include "game_create_role_dlg.h"

#pragma comment(lib, "../lib/venus.lib")

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    NET.Connect("127.0.0.1", 36911);

    ConfigLoader::get_mutable_instance().initialize(ConfigManager::getInstancePtr());
    ConfigManager::getInstance().start();
    ConfigManager::getInstance().wait();

    if (ResImgPool::get_mutable_instance().Initialize())
    {
        ResImgPool::get_mutable_instance().Load();
    }

    GameLoginDlg dlg;
    int nRet = dlg.DoModal();

    NET.Close();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));

    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    ///> ≥ı ºªØBKWIN
    BkString::Load(IDR_BK_STRING_DEF);
    BkSkin::LoadSkins(IDR_BK_SKIN_DEF);
    BkStyle::LoadStyles(IDR_BK_STYLE_DEF);

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
