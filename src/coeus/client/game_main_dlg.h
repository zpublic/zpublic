/* -------------------------------------------------------------------------
//  File        :   game_main_dlg.h
//  Author      :   Sum
//  Datet       :   2013/12/13 21:58
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_MAIN_DLG_H_
#define __GAME_MAIN_DLG_H_

#include <vector>
#include <wtlhelper/whwindow.h>
#include <CBkDialogViewImplEx.h>
#include <bkres/bkres.h>
#include "game_item_def.h"
#include "unit_tinyxml.h"

#define WM_TRAYMESSAGE                      (WM_USER + 2002)

class GameMainDlg
    : public CBkDialogViewEx
    , public CWHRoundRectFrameHelper<GameMainDlg>
{
public:
    GameMainDlg()
        : CBkDialogViewEx(IDR_BK_MAIN_DIALOG)
    {
    }

    enum
    {
        ID_CLOSE_BTN    = 403,
        IDC_LIST_CTRL   = 1000,
    };

    enum
    {
        IDC_LIST_CHILD_ITEM_ICON                = 10000,
        IDC_LIST_CHILD_ITEM_TEXT_ITEMNAME       = 10001,
        IDC_LIST_CHILD_ITEM_TEXT_ITEMNAME_DE    = 10002,
        IDC_LIST_CHILD_ITEM_TEXT_NUM            = 10003,
        IDC_LIST_CHILD_ITEM_TEXT_WEAPON         = 10004,
        IDC_LIST_CHILD_ITEM_LINE                = 10005,
        IDC_LIST_CHILD_ITEM_DISBUTTON_DLG       = 20000,
    };

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

    void OnClose();

    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    void OnSysCommand(UINT nID, CPoint point);

    void OnTimer(UINT_PTR nIDEvent);

    void OnBtnClose();

    void OnListItemLButtonUp(int nListItem);

    void OnListItemMouseHover(int nListItem);

    void OnListItemMouseLeave(int nListItem);

    LRESULT OnTrayMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    struct ListItemData
    {
        UINT nNum;
        HICON hICon;
        CString csItemName;
        ItemDataType emItemType;
        CString csDescribeName;
        HBKWND  hListitemBkWnd;

        ListItemData()
        {
            nNum = 0;
            emItemType = emItemDataType_Null;
            hListitemBkWnd = NULL;
            hICon = NULL;
        }
    };

protected:
    BOOL InitItemList();

    BOOL CreateListItemXml(ListItemData& itemdata, UnitTinyXml& unitTinyXML);

    BOOL AddOneItemToListWnd(ListItemData& itemdata, UINT nListCtrlId);

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
        BK_NOTIFY_ID_COMMAND(ID_CLOSE_BTN, OnBtnClose)
        //         BK_NOTIFY_ID_COMMAND(ID_OPEN_DLG, OnBtnOpenDlg)
        //         BK_NOTIFY_ID_COMMAND(ID_BTN_LOGIN, OnBtnLogin)
        //         BK_NOTIFY_ID_COMMAND(ID_BTN_RESET, OnBtnReset)
        BK_LISTWND_NOTIFY_BEGIN(IDC_LIST_CTRL)
            BK_LISTWND_LISTITEM_MOUSEHOVER(OnListItemMouseHover)
            BK_LISTWND_LISTITEM_MOUSELEAVE(OnListItemMouseLeave)
            BK_LISTWND_LISTITEM_LBUTTONUP(OnListItemLButtonUp)
        BK_NOTIFY_MAP_END()
    BK_NOTIFY_MAP_END()

    BEGIN_MSG_MAP_EX(GameMainDlg)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN_EX)
        CHAIN_MSG_MAP(CBkDialogViewEx)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<GameMainDlg>)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        MESSAGE_HANDLER_EX(WM_TRAYMESSAGE, OnTrayMessage)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

private:
    BOOL _CreateTray(HINSTANCE hInst, UINT nIConId);

    BOOL _ResetTray(HINSTANCE hInst, UINT nIConId);

    BOOL _DeleteTray(HINSTANCE hInst, UINT nIConId);

    BOOL _SetTray(HINSTANCE hInst, UINT nIConId, ULONG ulType);

    void _Close();
};

#endif // __GAME_MAIN_DLG_H_