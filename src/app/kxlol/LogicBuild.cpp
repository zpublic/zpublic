#include "stdafx.h"
#include "LogicBuild.h"


CLogicBuild::CLogicBuild()
{
}


CLogicBuild::~CLogicBuild()
{
}

int CLogicBuild::Build(const wchar_t* pName)
{
    int nRet = -999;
    CString sPath = ZLPath::GetModuleFolder(NULL) + "data\\build_";
    sPath += pName;
    sPath += L".lua";
    if (::PathFileExists(sPath))
    {
        z_lua_state lua;
        if (lua.create() == 0)
        {
            GAME.Script().RegAll(lua);
            if (0 == lua.open_all_libs()
                && 0 == lua.dofile(ZLW2A(sPath)))
            {
                nRet = lua.call<int>("build");
                if (nRet == 0)
                {
                    MessageBox(0, L"成功  神力-3", 0, 0);
                }
                else
                {
                    MessageBox(0, L"失败  神力不够", 0, 0);
                }
            }
        }
    }
    return nRet;
}
