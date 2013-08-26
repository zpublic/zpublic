#pragma once
#include "frame2_view.h"

class CFrame2 : public CFrameWindowImpl<CFrame2>
{
public:
    DECLARE_FRAME_WND_CLASS(L"CFrame2", IDR_MAINFRAME1);

    BEGIN_MSG_MAP_EX(CFrame2)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_TIMER(OnTimer)
        CHAIN_MSG_MAP(CFrameWindowImpl<CFrame2>)
    END_MSG_MAP()

private:
    LRESULT OnCreate(LPCREATESTRUCT lpcs)
    {
        SetTimer(1, 1000);
        m_hWndClient = m_view.Create(
            m_hWnd,
            CWindow::rcDefault,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN,
            WS_EX_CLIENTEDGE);
        SetMsgHandled(false);
        return 0;
    }

    void OnTimer(UINT_PTR nIDEvent)
    {
        if (nIDEvent == 1)
        {
            m_view.RedrawWindow();
        }
        else
        {
            SetMsgHandled(false);
        }
    }

private:
    CFrame2View m_view;
};