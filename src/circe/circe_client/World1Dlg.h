#pragma once

#include "hge/include/hge.h"
#include "hge/include/hgesprite.h"
#include "hge/include/hgefont.h"
#include "hge/include/hgeparticle.h"
#include "hge/include/hgeresource.h"
#include "hge/include/hgeanim.h"
#include "hge/include/hgegui.h"
#include "hge/include/hgeguictrls.h"

extern HGE *g_pHge;

class CWorld1Dlg : public CDialogImpl<CWorld1Dlg>
{
public:
    enum { IDD = IDD_WORLD_1 };

    BEGIN_MSG_MAP(CWorld1Dlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MSG_WM_TIMER(OnTimer)
        COMMAND_HANDLER(IDC_W1_ZHILIAO, BN_CLICKED, OnBnClickedW1Zhiliao)
    END_MSG_MAP()

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnBnClickedW1Zhiliao(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    void OnTimer(UINT_PTR nIDEvent);

    int     m_nMoney;
};
