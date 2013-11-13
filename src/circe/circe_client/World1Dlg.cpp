#include "stdafx.h"
#include "resource.h"

#include "World1Dlg.h"

#pragma comment(lib, "../../../3rdparty/hge/lib/hge.lib")
#pragma comment(lib, "../../../3rdparty/hge/lib/hgehelp.lib")

HGE *g_pHge=0;
hgeResourceManager* myRes;
int x,y,width,height;
HWND hwnd = 0;

bool FrameFunc()
{
    return false;
}

bool RenderFunc()
{
    float dt=g_pHge->Timer_GetDelta();
    g_pHge->Gfx_BeginScene();
    g_pHge->Gfx_Clear(0); 
    for(int x=0;x<=width/50;x++)
    {
        g_pHge->Gfx_RenderLine((float)(x*50),0.0,(float)(x*50),(float)height);
    }
    for(int x=0;x<=height/50;x++)
    {
        g_pHge->Gfx_RenderLine(0.0,(float)x*50,(float)width,(float)x*50);
    }
    g_pHge->Gfx_EndScene();
    return false;
}

LRESULT CWorld1Dlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if (hwnd)
    {
        ::ShowWindow(hwnd, SW_SHOW);
    }
    g_pHge = hgeCreate(HGE_VERSION);
    CWindow w=GetDlgItem(IDC_W1_BG);

    RECT rect;
    w.GetClientRect(&rect);
    x=0;
    y=0;
    width=rect.right - rect.left;
    height=rect.bottom - rect.top;

    g_pHge->System_SetState(HGE_SHOWSPLASH, false);
    g_pHge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    g_pHge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    g_pHge->System_SetState(HGE_TITLE, "∫«∫«");
    //g_pHge->System_SetState(HGE_HWNDPARENT, w.m_hWnd);
    g_pHge->System_SetState(HGE_SCREENWIDTH, width);
    g_pHge->System_SetState(HGE_SCREENHEIGHT, height);
    g_pHge->System_SetState(HGE_WINDOWED, true);
    g_pHge->System_Initiate();
    SetTimer(1, 100);

    hwnd = g_pHge->System_GetState(HGE_HWND);
    long lStyle = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, lStyle & ~WS_CAPTION);

    m_nMoney = 100;
    CenterWindow(GetParent());
    return TRUE;
}

LRESULT CWorld1Dlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    KillTimer(1);
    ::ShowWindow(hwnd, SW_HIDE);
    //g_pHge->System_Shutdown();
   // g_pHge->Release();
    EndDialog(0);
    return 0;
}

LRESULT CWorld1Dlg::OnBnClickedW1Zhiliao(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (m_nMoney > 0)
    {
        MessageBox(L"“©≤ªƒ‹Õ££°");
        m_nMoney -= 20;
        CString strText;
        strText.Format(L"”‡∂Ó£∫%d", m_nMoney);
        SetDlgItemText(IDC_W1_MONEY, strText);
    }
    else
    {
        MessageBox(L"√ª«Æ¡À£¨«Î≥‰÷µ£°");
    }
    return 0;
}

void CWorld1Dlg::OnTimer( UINT_PTR nIDEvent )
{
    if (nIDEvent == 1)
    {
        RenderFunc();
    }
}
