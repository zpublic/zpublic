#include "stdafx.h"
#include "ScriptMgr.h"
#include "ScriptPlayer.h"

#define  LUA_INSERT(reg, x)     reg.insert(#x, x)

CScriptMgr::CScriptMgr()
{
    m_luaregPlayer.libname("player");
    LUA_INSERT(m_luaregPlayer, get_name);
    LUA_INSERT(m_luaregPlayer, set_name);
    LUA_INSERT(m_luaregPlayer, get_shenli);
    LUA_INSERT(m_luaregPlayer, set_shenli);
    LUA_INSERT(m_luaregPlayer, add_shenli);
    LUA_INSERT(m_luaregPlayer, sub_shenli);
}

CScriptMgr::~CScriptMgr()
{
}
