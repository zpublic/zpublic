#pragma once

namespace zl
{
    template <class T>
    class CWHRoundRectFrameHelper
    {
    protected:

        CWHRoundRectFrameHelper()
        {
            m_sizeWnd.cx = 0;
            m_sizeWnd.cy = 0;
        }
        SIZE m_sizeWnd;

        void OnSize(UINT nType, CSize size)
        {
            T *pT = static_cast<T*>(this);

            if (nType == SIZE_MINIMIZED)
                return;

            if (size == m_sizeWnd)
                return;

            if (size.cx == 0 && size.cy == 0)
                return;

            CRect rcWindow, rcClient;
            CRgn rgnWindow, rgnMinus1, rgnMinus2;

            pT->GetWindowRect(rcWindow);
            pT->GetClientRect(rcClient);
            pT->ClientToScreen(rcClient);

            rcClient.OffsetRect(- rcWindow.TopLeft());

            rgnWindow.CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

            rgnMinus1.CreateRectRgn(rcClient.left, rcClient.top, rcClient.left+2, rcClient.top+2);
            rgnMinus2.CreateRectRgn(rcClient.left, rcClient.top, rcClient.left+1, rcClient.top + 1);
            //ÍÚ×óÉÏ½Ç
            rgnWindow.CombineRgn(rgnMinus1, RGN_XOR);
            rgnMinus2.OffsetRgn(+2, 0);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);
            rgnMinus2.OffsetRgn(-2, 2);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);

            //ÍÚÓÒÉÏ½Ç
            rgnMinus1.OffsetRgn(rcClient.right - 2, 0);
            rgnWindow.CombineRgn(rgnMinus1, RGN_XOR);
            rgnMinus2.OffsetRgn(rcClient.right - 1, 0);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);
            rgnMinus2.OffsetRgn(-2, -2);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);

            //ÍÚÓÒÏÂ½Ç
            rgnMinus1.OffsetRgn(0, rcClient.bottom - 2);
            rgnWindow.CombineRgn(rgnMinus1, RGN_XOR);
            rgnMinus2.OffsetRgn(0, rcClient.bottom -1);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);
            rgnMinus2.OffsetRgn(2, -2);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);

            //ÍÚ×óÏÂ½Ç
            rgnMinus1.OffsetRgn(-(rcClient.right - 2),  0);
            rgnWindow.CombineRgn(rgnMinus1, RGN_XOR);
            rgnMinus2.OffsetRgn(-(rcClient.right - 1),  0);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);
            rgnMinus2.OffsetRgn(2, 2);
            rgnWindow.CombineRgn(rgnMinus2, RGN_XOR);

            pT->SetWindowRgn(rgnWindow);

            m_sizeWnd = size;
        }

    public:
        BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
        {
            BOOL bHandled = TRUE;

            switch(dwMsgMapID)
            {
            case 0:
                if (uMsg == WM_SIZE)
                {
                    OnSize((UINT)wParam, _WTYPES_NS::CSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
                    lResult = 0;
                }
                else if (uMsg == WM_DESTROY)
                {
                    m_sizeWnd.cx = 0;
                    m_sizeWnd.cy = 0;
                }

                break;
            }
            return FALSE;
        }
    };
}
