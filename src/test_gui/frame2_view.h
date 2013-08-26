#pragma once

class CFrame2View : public CWindowImpl<CFrame2View>
{
public:
    DECLARE_WND_CLASS(L"CFrame2View")

    BEGIN_MSG_MAP_EX(CFrame2View)
        MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

private:
    void OnPaint(CDCHandle)
    {
        CPaintDC dc(m_hWnd);
        CRect      rc;
        SYSTEMTIME st;
        CString    sTime;

        GetClientRect (rc);
        GetLocalTime (&st);
        sTime.Format(
            L"The time is %d:%02d:%02d",
            st.wHour,
            st.wMinute,
            st.wSecond);

        dc.SaveDC();
        dc.SetBkColor(RGB(255,153,0));
        dc.SetTextColor(RGB(0,0,0));
        dc.ExtTextOut(0, 0, ETO_OPAQUE, rc, sTime, sTime.GetLength(), NULL);
        dc.RestoreDC(-1);
        SetMsgHandled(true);
    }
};