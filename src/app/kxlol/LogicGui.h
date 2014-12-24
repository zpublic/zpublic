#pragma once
class CLogicGui
{
public:
    CLogicGui();
    ~CLogicGui();

    void UpdateZhushenkongjianShow();

public:
    BOOL                m_bNeedShowMake;
    BOOL                m_bNeedShowPractice;

private:
    z_lua_state         m_lua;
    bool                m_bLuaOk;
};
