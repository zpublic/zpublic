#pragma once
class CScriptMgr
{
public:
    CScriptMgr();
    ~CScriptMgr();

    z_lua_function_reg& Player()
    {
        return m_luaregPlayer;
    }

private:
    z_lua_function_reg          m_luaregPlayer;
};

