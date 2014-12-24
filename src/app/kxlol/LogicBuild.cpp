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
            lua.open_all_libs();
            lua.dofile(ZLW2A(sPath));
            nRet = lua.call<int>("build");
        }
    }
    return nRet;
}
