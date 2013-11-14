#pragma once

#include "finger_guessing/fg_game.h"

class CWorld3Dlg : public CDialogImpl<CWorld3Dlg>
{
public:
    enum { IDD = IDD_WORLD_3 };

    BEGIN_MSG_MAP(CWorld3Dlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
    END_MSG_MAP()

public:
    CWorld3Dlg();

    LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    void _ConvertCardToStirng(const FGCard& card, CString& csConvertString);

private:
    CFGGame m_Game;
    CListBox m_SeftCardListBox;
    CEdit m_FightInfoEdit;
    int m_iSeftLift;
    int m_iComputey;
};
