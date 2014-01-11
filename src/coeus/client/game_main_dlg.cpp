/* -------------------------------------------------------------------------
//  File        :   game_main_dlg.cpp
//  Author      :   Sum
//  Datet       :   2013/12/13 21:58
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_main_dlg.h"
#include "bkwinres.h"
#include "unit_create_xmlfunction.h"
#include "unit_function.h"
#include "resource.h"

#define LIST_ITEM_HEIGHT 56
#define TRAY_TIP                            _T("couse")

LRESULT GameMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    _CreateTray(_Module.GetModuleInstance(), IDR_MAINFRAME);
    ListItemData data;
    data.csItemName = L"黑色切割者";
    data.csDescribeName = L"黑色切割者造成减益效果最多叠加五层";
    data.nNum = 10000;
    data.emItemType = emItemDataType_Weapon;
    AddOneItemToListWnd(data, IDC_LIST_CTRL);
    AddOneItemToListWnd(data, IDC_LIST_CTRL);

    return TRUE;
}

void GameMainDlg::OnClose()
{
    OnBtnClose();
}

void GameMainDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDOK);
    }
}

void GameMainDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameMainDlg::OnBtnClose()
{
    ShowWindow(SW_HIDE);
}

BOOL GameMainDlg::InitItemList()
{
    return TRUE;
}

BOOL GameMainDlg::CreateListItemXml(ListItemData& itemdata, UnitTinyXml& unitTinyXML)
{
    BOOL bReturn = FALSE;
    UnitCreateXMLFunction XmlElFunc(unitTinyXML);
    CString csNum;
    CString csType;

    bReturn = XmlElFunc.AddTinyChild("listitem", 0, NULL, NULL, NULL, "list_item", NULL);
    if (FALSE == bReturn) goto Exit0;

    bReturn = XmlElFunc().Write("height", LIST_ITEM_HEIGHT);
    if (FALSE == bReturn) goto Exit0;

//     bReturn = XmlElFunc.AddTinyChild("icon", IDC_LIST_CHILD_ITEM_ICON, "40,25,-0,-0", NULL, NULL, NULL);
//     if (FALSE == bReturn) goto Exit0;
//     XmlElFunc().Write("size", "32");

    bReturn = XmlElFunc.AddTinyChild("text", IDC_LIST_CHILD_ITEM_TEXT_ITEMNAME, "53,12,160,30", NULL, NULL, "list_item_name");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().Write("tip", itemdata.csItemName);
    XmlElFunc().WriteText(itemdata.csItemName);

    bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_CHILD_ITEM_TEXT_ITEMNAME_DE, "53,35,260,49", NULL, NULL, "list_item_name_de");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().Write("tip", itemdata.csDescribeName);
    XmlElFunc().WriteText(itemdata.csDescribeName);

    UnitFunction::FomatInt(itemdata.nNum, csNum);
    bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_CHILD_ITEM_TEXT_NUM, "261,0,340,-0", NULL, NULL, "list_text_softname_center_gdiplus");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().WriteText(csNum);

    UnitFunction::ConvertItemDataToString(itemdata.emItemType, csType);
    bReturn = XmlElFunc.AddTinySibling("text", IDC_LIST_CHILD_ITEM_TEXT_WEAPON, "340,0,424,-0", NULL, NULL, "list_text_softname_center_gdiplus");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().WriteText(csType);

    bReturn = XmlElFunc.AddTinySibling("dlg", IDC_LIST_CHILD_ITEM_LINE, "0,55,-0,-0", NULL, NULL, NULL);
    XmlElFunc().Write("crbg", "000000");
    if (FALSE == bReturn) goto Exit0;

    bReturn = XmlElFunc.AddTinySibling("dlg", IDC_LIST_CHILD_ITEM_DISBUTTON_DLG, "425,0,-0,-0", NULL, NULL, "list_text_softname_center_gdiplus");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().WriteText(csType);

    bReturn = XmlElFunc.AddTinyChild("button", IDC_LIST_CHILD_ITEM_DISBUTTON_DLG, "14,16,74,39", NULL, NULL, "btn_metro_3");
    if (FALSE == bReturn) goto Exit0;
    XmlElFunc().Write("crtext", "000000");
    XmlElFunc().WriteText(L"使用");
    
Exit0:
    return bReturn;
}

BOOL GameMainDlg::AddOneItemToListWnd(ListItemData& itemdata, UINT nListCtrlId)
{
    BOOL bReturn = FALSE;
    UnitTinyXml tinyXml;

    do 
    {
        BOOL bRetCode = FALSE;

        if (NULL == tinyXml.Open("Root", TRUE))
        {
            break;
        }

        if (TRUE)
        {
            UnitTinyXmlRememberPos(tinyXml);
            bRetCode = CreateListItemXml(itemdata, tinyXml);
        }

        if (!bRetCode || !tinyXml.FirstChild())
        {
            goto Exit0;
        }

        /*	if (TRUE)
        {
        CStringA strXml = tinyXml.ToXml();
        }*/

        int nListItem = AppendListItem(nListCtrlId, tinyXml.GetElement(), -1, FALSE);
        if (nListItem == -1) goto Exit0;

        itemdata.hListitemBkWnd = GetListItemBkWndHandle(nListCtrlId, nListItem);
        bReturn = TRUE;

    }while(FALSE);

Exit0:
    return bReturn;
}

void GameMainDlg::OnListItemLButtonUp(int nListItem)
{

}

void GameMainDlg::OnListItemMouseHover(int nListItem)
{
    SetListItemAttribute(IDC_LIST_CTRL, nListItem, "class", "list_item_hover");
}

void GameMainDlg::OnListItemMouseLeave(int nListItem)
{
    SetListItemAttribute(IDC_LIST_CTRL, nListItem, "class", "list_item");
}

BOOL GameMainDlg::_CreateTray(HINSTANCE hInst, UINT nIConId)
{
    return _SetTray(hInst, nIConId, NIM_ADD);
}

BOOL GameMainDlg::_ResetTray(HINSTANCE hInst, UINT nIConId)
{
    return _SetTray(hInst, nIConId, NIM_MODIFY);
}

BOOL GameMainDlg::_DeleteTray(HINSTANCE hInst, UINT nIConId)
{
    return _SetTray(hInst, nIConId, NIM_DELETE);
}

BOOL GameMainDlg::_SetTray(HINSTANCE hInst, UINT nIConId, ULONG ulType)
{
    UINT nTipLen = lstrlen(TRAY_TIP) + sizeof(TCHAR);
    NOTIFYICONDATA NotifyICon;
    NotifyICon.cbSize = sizeof(NOTIFYICONDATA);
    NotifyICon.hWnd = m_hWnd;
    NotifyICon.uID = IDR_MAINFRAME;
    NotifyICon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    NotifyICon.uCallbackMessage = WM_TRAYMESSAGE;
    NotifyICon.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nIConId));
    _tcsncpy_s(NotifyICon.szTip, nTipLen, TRAY_TIP, nTipLen - 1);
    return ::Shell_NotifyIcon(ulType, &NotifyICon);
}

void GameMainDlg::_Close()
{
    _DeleteTray(_Module.GetModuleInstance(), IDR_MAINFRAME);
    EndDialog(IDOK);
}

LRESULT GameMainDlg::OnTrayMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TRAYMESSAGE)
    {
        switch (lParam)
        {
        case WM_LBUTTONUP:
            ShowWindow(SW_SHOW);
            break;
        default:
            break;
        }
    }

    return TRUE;
}
