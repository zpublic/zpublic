#pragma once
#include "resource.h"
#include "lab/particle/particle_gdi_base.h"
#include "lab/publish/gdiplus_resource.h"

#define WM_PPP  WM_USER + 1

class CLab2 : public CSimpleDialog<IDD_DIALOG6>
{
public:
    BEGIN_MSG_MAP(CLab2)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PAINT(OnPaint)
        COMMAND_ID_HANDLER(IDOK, OnOk)
        MESSAGE_HANDLER(WM_PPP, OnAnimate)
    END_MSG_MAP()

public:
    CLab2()
    {
        m_dwCurrent = ::GetTickCount();
        HBITMAP hBitmap = (HBITMAP)LoadImage(
            _Module.m_hInst,
            MAKEINTRESOURCE(IDB_BITMAP3),
            IMAGE_BITMAP,
            0,
            0,
            LR_CREATEDIBSECTION);
        m_pImage = new Bitmap(hBitmap, NULL);
        m_hdcMem = NULL;
    }
    ~CLab2()
    {
        if (m_pImage)
        {
            delete m_pImage;
        }
        if (m_hdcMem)
        {
            ::DeleteDC(m_hdcMem);
        }
    }

private:
    BOOL OnInitDialog(CWindow, LPARAM)
    {
        HDC hdc = ::GetDC(m_hWnd);
        m_hdcMem = ::CreateCompatibleDC(hdc);
        CenterWindow();
        SetTimer(1, 20);
        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        CPaintDC dcPaint(m_hWnd);
        CRect rcClient(0,0,500,300);
        m_particle.Render(dcPaint, m_pImage, rcClient);
    }

    LRESULT OnAnimate(UINT, WPARAM, LPARAM, BOOL& bHandled)
    {
        bHandled = TRUE;
        CRect rcClient(0,0,500,300);
        DWORD dwCur = ::GetTickCount();
        m_particle.Update(float(dwCur - m_dwCurrent) / 1000.0f);
        m_dwCurrent = dwCur;
        InvalidateRect(rcClient);
        return 0;
    }

    void OnTimer(UINT_PTR nIDEvent)
    {
        if (nIDEvent == 1)
        {
            PostMessage(WM_PPP);
        }
    }

    BOOL OnEraseBkgnd(CDCHandle dc)
    {
        return FALSE;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDOK);
        return 0;
    }

private:
    ParticleGdiBase m_particle;
    DWORD           m_dwCurrent;
    Bitmap*         m_pImage;
    HDC             m_hdcMem;
};

void Lab2()
{
    CLab2 dlg;
    dlg.DoModal();
}
