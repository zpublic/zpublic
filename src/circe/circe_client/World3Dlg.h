#pragma once

#include "finger_guessing/fg_game.h"

class CWorld3Dlg : public CDialogImpl<CWorld3Dlg>
{
public:
    enum { IDD = IDD_WORLD_3 };

    BEGIN_MSG_MAP(CWorld3Dlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(msg_game_update, OnGameUpdate)
        MESSAGE_HANDLER(msg_game_out_card, OnGameFight)
        COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
    END_MSG_MAP()

public:
    CWorld3Dlg();

    LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnGameUpdate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    LRESULT OnGameFight(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
    void _ConvertCardToStirng(const FGCard& card, CString& csConvertString);

    void _ConvertDamageToStirng(int iDamageValue, CString& csConvertString);

    bool _PrintFightInfo(const FGFightInfo* Info);

private:
    CFGGame m_Game;
    CListBox m_SeftCardListBox;
    CEdit m_FightInfoEdit;
    int m_iSeftLift;
    int m_iComputey;
};
