#include "stdafx.h"
#include "LogicGui.h"


CLogicGui::CLogicGui()
{
    m_bNeedShowMake             = FALSE;
    m_bNeedShowPractice         = FALSE;

    m_bLuaOk = false;
    CString sPath = ZLPath::GetModuleFolder(NULL) + "data\\gui_zhushenkongjian.lua";
    if (::PathFileExists(sPath))
    {
        z_lua_state lua;
        if (lua.create() == 0)
        {
            GAME.Script().RegAll(lua);
            if (0 == lua.open_all_libs()
                && 0 == lua.dofile(ZLW2A(sPath)))
            {
                m_bLuaOk = true;
            }
        }
    }
}

CLogicGui::~CLogicGui()
{
}

void CLogicGui::UpdateZhushenkongjianShow()
{
    if (m_bLuaOk)
    {
        if (!m_bNeedShowMake)
        {
            m_bNeedShowMake = m_lua.call<bool>("need_show_make");
        }
        if (!m_bNeedShowPractice)
        {
            m_bNeedShowPractice = m_lua.call<bool>("need_show_practice");
        }
    }
}
