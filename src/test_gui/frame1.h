#pragma once

class CFrame1 : public CFrameWindowImpl<CFrame1>
{
public:
    DECLARE_FRAME_WND_CLASS(L"CFrame1", IDR_MAINFRAME1);

    BEGIN_MSG_MAP_EX(CFrame1)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_TIMER(OnTimer)
        CHAIN_MSG_MAP(CFrameWindowImpl<CFrame1>)
    END_MSG_MAP()

private:
    LRESULT OnCreate(LPCREATESTRUCT lpcs)
    {
        SetTimer(1, 3000);
        SetMsgHandled(false);
        return 0;
    }

    void OnTimer(UINT uTimerID)
    {
        if ( 1 != uTimerID )
        {
            SetMsgHandled(false);
        }
        else
        {
            KillTimer(1);
            ::MessageBox(0, L"OnTimer 3s", L"", 0);
        }
    }
};