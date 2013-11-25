#pragma once

#include "finger_guessing/fg_game.h"
#include <string>

class HearbeatHandler
{
public:
    HearbeatHandler();

    void SetGameDlg(HWND hWnd);

    void SendHearbeat();

    void NotifyResult();

    void NotifyResult(std::string& strErr);

private:
    HWND m_hGameDlg;
};
