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

    z_lua_function_reg& Gui()
    {
        return m_luaregGui;
    }

    void RegAll(z_lua_state& lua_state)
    {
        lua_state.reg_lib(m_luaregPlayer);
        lua_state.reg_lib(m_luaregGui);
    }

private:
    z_lua_function_reg          m_luaregPlayer;
    z_lua_function_reg          m_luaregGui;
};

