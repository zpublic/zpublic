#pragma once
#include "frame3_view.h"
#include "frame3_subview.h"

class CFrame3 : public CFrameWindowImpl<CFrame3>
{
public:
    DECLARE_FRAME_WND_CLASS(L"CFrame3", IDR_MAINFRAME1);

    BEGIN_MSG_MAP_EX(CFrame3)
        MSG_WM_CREATE(OnCreate)
        CHAIN_MSG_MAP(CFrameWindowImpl<CFrame3>)
    END_MSG_MAP()

private:
    LRESULT OnCreate(LPCREATESTRUCT lpcs)
    {
        m_hWndClient = m_view.Create(
            m_hWnd,
            CWindow::rcDefault,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN,
            WS_EX_CLIENTEDGE);
        AddPage(L"1");
        AddPage(L"2");
        SetMsgHandled(false);
        return 0;
    }

    BOOL AddPage(LPCWSTR lpTitle)
    {
        CFrame3SubView* pSubView = new CFrame3SubView();
        pSubView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
        if(pSubView->IsWindow())
        {
            m_view.AddPage(pSubView->m_hWnd, lpTitle, 0, pSubView);
            return TRUE;
        }
        else
        {
            delete pSubView;
            return FALSE;
        }
    }

private:
    CFrame3View m_view;
};