#pragma once

#include "finger_guessing/fg_game.h"

enum FGFightNotifyResult
{
    emOutCarNotifyResult_Null,
    emOutCarNotifyResult_Update,
    emOutCarNotifyResult_OutCard,
};

class CFGFightHandler
{
public:
    CFGFightHandler();

    void SetGameDlg(HWND hWnd);

    void InitializeGame(UINT nCard, UINT nLeft);

    void OutCard(UINT nIndex);

    void NotifyResult(FGFightNotifyResult emNotityResult);

    void NotifyResult(std::string& strErr);

private:
    HWND m_hGameDlg;
    CFGGame m_Game;
    VECFGCARD m_SeftCard_vec;
    FGFightInfo m_FightInfo;
    FGGameInfo m_GameInfo;
};
