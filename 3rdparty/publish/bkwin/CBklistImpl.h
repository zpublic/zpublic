
#pragma once
#include <stdarg.h>
#include "bkres\bkstringpool.h"

template <class T>
class ATL_NO_VTABLE CBkListImpl
{
public:

	CString GetListItemText(int nListItem)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		CString strItemText = _T("");

		if (pWnd)
		{
			strItemText = pWnd->GetInnerText();
			return strItemText;
		}

		return strItemText;
	}

	BOOL SetListItemAttribute(int nListItem, LPCSTR lpszAttrib, LPCSTR lpszValue)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (pWnd)
		{
			pT->_InvalidateControl(pWnd);

			pWnd->SetAttribute(lpszAttrib, lpszValue, FALSE);

			pT->_InvalidateControl(pWnd);
			return TRUE;
		}

		return FALSE;
	}

	BOOL SetListItemStringAttribute(int nListItem, LPCSTR lpszAttrib, LPCTSTR lpszValue)
	{
		return SetListItemAttribute(nListItem, lpszAttrib, CT2A(lpszValue, CP_UTF8));
	}

	BOOL SetListItemIntAttribute(int nListItem, LPCSTR lpszAttrib, int nValue)
	{
		CStringA strValue;
		strValue.Format("%d", nValue);

		return SetListItemAttribute(nListItem, lpszAttrib, strValue);
	}

	BOOL SetListItemDWordAttribute(int nListItem, LPCSTR lpszAttrib, DWORD dwValue)
	{
		CStringA strValue;
		strValue.Format("%u", dwValue);

		return SetListItemAttribute(nListItem, lpszAttrib, strValue);
	}

	BOOL SetListItemColorAttribute(int nListItem, LPCSTR lpszAttrib, COLORREF crValue)
	{
		CStringA strValue;
		strValue.Format("%02X%02X%02X", GetRValue(crValue), GetGValue(crValue), GetBValue(crValue));

		return SetListItemAttribute(nListItem, lpszAttrib, strValue);
	}

	
	BOOL EnableListItem(int nListItem, BOOL bEnable)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (pWnd)
		{
			if (bEnable)
				pWnd->ModifyState(0, BkWndState_Disable);
			else
				pWnd->ModifyState(BkWndState_Disable, BkWndState_Hover);

			pT->_InvalidateControl(pWnd);
			return TRUE;
		}

		return FALSE;
	}

	BOOL IsListItemEnable(int nListItem, BOOL bCheckParent = FALSE)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (pWnd)
			return !pWnd->IsDisabled(bCheckParent);

		return FALSE;
	}

	BOOL SetListItemVisible(int nListItem, BOOL bVisible)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (pWnd)
		{
			pWnd->BkSendMessage(WM_SHOWWINDOW, (WPARAM)bVisible);

			pT->_InvalidateControl(pWnd, FALSE);

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsListItemVisible(int nListItem, BOOL bCheckParent = FALSE)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (pWnd)
			return pWnd->IsVisible(bCheckParent);

		return FALSE;
	}

	BOOL GetListItemRect(int nListItem, RECT &rcItem)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (!pWnd)
			return FALSE;

		pWnd->GetRect(&rcItem);

		return TRUE;
	}

	BOOL SetListItemNeedReDraw(int nListItem)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (!pWnd)
			return FALSE;

		pT->_InvalidateControl(pWnd);
		return TRUE;
	}

	BOOL SetListItemPaintHook(int nListItem, IBkWindowPaintHook* pPaintHook)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (!pWnd)
			return FALSE;

		pWnd->SetPaintHook(pPaintHook);

		return TRUE;
	}

	BOOL SetListItemChildCtrlMouseHook(int nListItem, IBkWindowMouseHook *pMouseHook)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->GetListItem(nListItem);
		if (!pWnd)
			return FALSE;

		pWnd->SetMouseHook(pMouseHook);

		return TRUE;
	}

	BOOL SetListItemPanelXml(int nListItem, UINT uResID)
	{
		CStringA strXml;
		BOOL bRet = BkResManager::LoadResource(uResID, strXml);
		if (!bRet)
			return FALSE;

		return SetListItemPanelXml(nListItem, strXml);
	}

	BOOL SetListItemPanelXml(int nListItem, LPCSTR lpszXml)
	{
		T* pT = static_cast<T*>(this);
		TiXmlDocument xmlDoc;

		{ // Free stack
			xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
			return FALSE;

		CBkListItem *pWndPanel = pT->GetListItem(nListItem);
		if (!pWndPanel)
			return FALSE;

		TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

		pWndPanel->LoadChilds(pXmlRootElem);
		pWndPanel->RepositionChilds();

		T* pT = static_cast<T*>(this);
		pT->_InvalidateControl(pWndPanel);

		if (pWndPanel->IsVisible(TRUE))
			pWndPanel->BkSendMessage(WM_SHOWWINDOW, TRUE);

		return TRUE;
	}

//////////////////////////////////////////////////////
////////////////////////////////////////////////////////
    BOOL SetListItemChildCtrlRichText(int nListItem, UINT uItemID, UINT uResID)
    {
        CStringA strXml;
        BOOL bRet = BkResManager::LoadResource(uResID, strXml);
        if (!bRet)
            return FALSE;

        return SetListItemChildCtrlRichText(nListItem, uItemID, strXml);
    }

    BOOL SetListItemChildCtrlRichText(int nListItem, UINT uItemID, LPCWSTR lpszXml)
    {
        CStringA strXml = CW2A(lpszXml, CP_UTF8);

        return SetListItemChildCtrlRichText(nListItem, uItemID, strXml);
    }

    BOOL SetListItemChildCtrlRichText(int nListItem, UINT uItemID, LPCSTR lpszXml)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkRichText::GetClassName()))
            return FALSE;

        TiXmlDocument xmlDoc;

        { // Free stack
            CStringA strXml;
            strXml = L'<';
            strXml += CBkRichText::GetClassName();
            strXml += L'>';
            strXml += lpszXml;
            strXml += L'<';
            strXml += L'/';
            strXml += CBkRichText::GetClassName();
            strXml += L'>';
            xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);
        }

        if (xmlDoc.Error())
            return FALSE;

        CBkRichText *pWndRichText = (CBkRichText *)pWnd;
        pWndRichText->LoadChilds(xmlDoc.RootElement());

		T* pT = static_cast<T*>(this);
        pT->_InvalidateControl(pWnd);

        if (pWnd->IsVisible(TRUE))
            pWnd->BkSendMessage(WM_SHOWWINDOW, TRUE);

        return TRUE;
    }

    BOOL SetListItemChildCtrlText(int nListItem, UINT uItemID, LPCTSTR lpszText)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);

        if (pWnd)
        {
			T* pT = static_cast<T*>(this);

            pT->_InvalidateControl(pWnd);

            pWnd->SetInnerText(lpszText);

            pT->_InvalidateControl(pWnd);

            return TRUE;
        }

        return FALSE;
    }

	CString GetListItemChildCtrlText(int nListItem, UINT uItemID)
	{
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
		CString strItemText = _T("");

		if (pWnd)
		{
			T* pT = static_cast<T*>(this);

			pT->_InvalidateControl(pWnd);

			strItemText = pWnd->GetInnerText();

			pT->_InvalidateControl(pWnd);

			return strItemText;
		}

		return strItemText;
	}

    BOOL SetListItemChildCtrlAttribute(int nListItem, UINT uItemID, LPCSTR lpszAttrib, LPCSTR lpszValue)
    {
        T* pT = static_cast<T*>(this);
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);

        if (pWnd)
        {
            pT->_InvalidateControl(pWnd);

            pWnd->SetAttribute(lpszAttrib, lpszValue, FALSE);

            pT->_InvalidateControl(pWnd);

            return TRUE;
        }

        return FALSE;
    }

    BOOL SetListItemChildCtrlStringAttribute(int nListItem, UINT uItemID, LPCSTR lpszAttrib, LPCTSTR lpszValue)
    {
        return SetListItemChildCtrlAttribute(nListItem, uItemID, lpszAttrib, CT2A(lpszValue, CP_UTF8));
    }

    BOOL SetListItemChildCtrlIntAttribute(int nListItem, UINT uItemID, LPCSTR lpszAttrib, int nValue)
    {
        CStringA strValue;
        strValue.Format("%d", nValue);

        return SetListItemChildCtrlAttribute(nListItem, uItemID, lpszAttrib, strValue);
    }

    BOOL SetListItemChildCtrlDWordAttribute(int nListItem, UINT uItemID, LPCSTR lpszAttrib, DWORD dwValue)
    {
        CStringA strValue;
        strValue.Format("%u", dwValue);

        return SetListItemChildCtrlAttribute(nListItem, uItemID, lpszAttrib, strValue);
    }

    BOOL SetListItemChildCtrlColorAttribute(int nListItem, UINT uItemID, LPCSTR lpszAttrib, COLORREF crValue)
    {
        CStringA strValue;
        strValue.Format("%02X%02X%02X", GetRValue(crValue), GetGValue(crValue), GetBValue(crValue));

        return SetListItemChildCtrlAttribute(nListItem, uItemID, lpszAttrib, strValue);
    }

    BOOL GetListItemChildCtrlCheck(int nListItem, UINT uItemID)
    {
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);

        if (pWnd)
            return pWnd->IsChecked();

        return FALSE;
    }

    BOOL SetListItemChildCtrlCheck(int nListItem, UINT uItemID, BOOL bCheck)
    {
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);

        if (pWnd)
        {
			T* pT = static_cast<T*>(this);
            if (bCheck)
                pWnd->ModifyState(BkWndState_Check, 0);
            else
                pWnd->ModifyState(0, BkWndState_Check);

            pT->_InvalidateControl(pWnd);

            return TRUE;
        }

        return FALSE;
    }

    BOOL EnableListItemChildCtrl(int nListItem, UINT uItemID, BOOL bEnable)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);

        if (pWnd)
        {
            if (bEnable)
                pWnd->ModifyState(0, BkWndState_Disable);
            else
                pWnd->ModifyState(BkWndState_Disable, BkWndState_Hover);

			T* pT = static_cast<T*>(this);
			pT->_InvalidateControl(pWnd);

            return TRUE;
        }

        return FALSE;
    }

    BOOL IsListItemChildCtrlEnable(int nListItem, UINT uItemID, BOOL bCheckParent = FALSE)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (pWnd)
            return !pWnd->IsDisabled(bCheckParent);

        return FALSE;
    }

    BOOL SetListItemChildCtrlVisible(int nListItem, UINT uItemID, BOOL bVisible)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (pWnd)
        {
			T* pT = static_cast<T*>(this);
            pWnd->BkSendMessage(WM_SHOWWINDOW, (WPARAM)bVisible);

            pT->_InvalidateControl(pWnd, FALSE);

            return TRUE;
        }

        return FALSE;
    }

    BOOL IsListItemChildCtrlVisible(int nListItem, UINT uItemID, BOOL bCheckParent = FALSE)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (pWnd)
            return pWnd->IsVisible(bCheckParent);

        return FALSE;
    }

    BOOL GetListItemChildCtrlRect(int nListItem, UINT uItemID, RECT &rcItem)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        pWnd->GetRect(&rcItem);

        return TRUE;
    }

	BOOL SetListItemChildCtrlNeedReDraw(int nListItem, UINT uItemID)
	{
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
		if (!pWnd)
			return FALSE;

		T* pT = static_cast<T*>(this);
		pT->_InvalidateControl(pWnd);

		return TRUE;
	}

    BOOL SetListItemChildCtrlRect(int nListItem, UINT uItemID, RECT &rcItem)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        pWnd->SetRect(&rcItem);
        
        return TRUE;
    }

    BOOL SetListItemChildCtrlTabCurSel(int nListItem, UINT uItemID, int nPage)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
            return FALSE;

        BOOL bRet = ((CBkTabCtrl *)pWnd)->SetCurSel(nPage);

        return bRet;
    }

    BOOL SetListItemChildCtrlTabTitle(int nListItem, UINT uItemID, int nPage, LPCTSTR lpszTitle)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
            return FALSE;

        BOOL bRet = ((CBkTabCtrl *)pWnd)->SetTabTitle(nPage, lpszTitle);

        return bRet;
    }

    int GetListItemChildCtrlTabCurSel(int nListItem, UINT uItemID)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return -1;

        if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
            return -1;

        return ((CBkTabCtrl *)pWnd)->GetCurSel();
    }

	CBkWindow* GetListItemChildCtrlItem(int nListItem, UINT uItemID)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->FindListItemChildCtrlByCmdID(nListItem, uItemID);
		
		return pWnd;
	}

    BOOL IsListItemChildCtrlTabPageVisible(int nListItem, UINT uItemID, int nPage)
    {
       CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
            return FALSE;

        return ((CBkTabCtrl *)pWnd)->IsPageVisible(nPage);
    }

    BOOL SetListItemChildCtrlTabPageVisible(int nListItem, UINT uItemID, int nPage, BOOL bVisible)
    {
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
            return FALSE;


        ((CBkTabCtrl *)pWnd)->SetPageVisible(nPage, bVisible);

        return TRUE;
    }

    BOOL SetListItemChildCtrlTabIconHandle(int nListItem, UINT uItemID, HICON hIcon)
    {
        CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
        if (!pWnd)
            return FALSE;

        if (!pWnd->IsClass(CBkIconWnd::GetClassName()))
            return FALSE;

        ((CBkIconWnd *)pWnd)->AttachIcon(hIcon);

        (static_cast<T*>(this))->_InvalidateControl(pWnd);

        return TRUE;
    }
	
	BOOL SetListItemChildCtrlPaintHook(int nListItem, UINT uItemID, IBkWindowPaintHook* pPaintHook)
	{
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
		if (!pWnd)
			return FALSE;

		pWnd->SetPaintHook(pPaintHook);

		return TRUE;
	}

	BOOL SetListItemChildCtrlMouseHook(int nListItem, UINT uItemID, IBkWindowMouseHook *pMouseHook)
	{
		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
		if (!pWnd)
			return FALSE;

		pWnd->SetMouseHook(pMouseHook);

		return TRUE;
	}
	

    BOOL FormatListItemChildCtrlRichText(int nListItem, UINT uItemID, UINT uStringID, ...)
    {
        CString strFormat = BkString::Get(uStringID);
        va_list args;
        CString strText;

        va_start(args, uStringID);

        strText.FormatV(strFormat, args);

        return SetListItemChildCtrlRichText(nListItem, uItemID, strText);
    }

    BOOL FormatListItemChildCtrlRichText(int nListItem, UINT uItemID, LPCTSTR lpszFormat, ...)
    {
        va_list args;
        CString strText;

        va_start(args, lpszFormat);

        strText.FormatV(lpszFormat, args);

        return SetListItemChildCtrlRichText(nListItem, uItemID, strText);
    }

    BOOL FormatListItemChildCtrlText(int nListItem, UINT uItemID, LPCTSTR lpszFormat, ...)
    {
        va_list args;
        CString strText;

        va_start(args, lpszFormat);

        strText.FormatV(lpszFormat, args);

        return SetListItemChildCtrlText(nListItem, uItemID, strText);
    }

	BOOL SetListItemChildCtrlPanelXml(int nListItem, UINT uItemID, UINT uResID)
	{
		CStringA strXml;
		BOOL bRet = BkResManager::LoadResource(uResID, strXml);
		if (!bRet)
			return FALSE;

		return SetListItemChildCtrlPanelXml(nListItem, uItemID, strXml);
	}

	BOOL SetListItemChildCtrlPanelXml(int nListItem, UINT uItemID, LPCSTR lpszXml)
	{
		TiXmlDocument xmlDoc;

		{ // Free stack
			xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
			return FALSE;

		CBkWindow *pWnd = GetListItemChildCtrlItem(nListItem, uItemID);
		if (!pWnd)
			return FALSE;

		CBkPanel* pWndPanel = dynamic_cast<CBkPanel*>(pWnd);
		if (pWndPanel == NULL) return FALSE;

		TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

		pWndPanel->LoadChilds(pXmlRootElem);
		pWndPanel->RepositionChilds();

		T* pT = static_cast<T*>(this);
		pT->_InvalidateControl(pWnd);

		if (pWnd->IsVisible(TRUE))
			pWnd->BkSendMessage(WM_SHOWWINDOW, TRUE);

		return TRUE;
	}

};

template <class T>
class CBkViewImplForList
{
public:

	friend class CBkListWnd;

	CBkListWnd* FindListWnd(UINT uItemID)
	{
		T* pT = static_cast<T*>(this);
		CBkWindow *pWnd = pT->FindChildByCmdID(uItemID);
		if (!pWnd)
			return NULL;

		if (!pWnd->IsClass(CBkListWnd::GetClassName()))
			return NULL;

		return (CBkListWnd *)pWnd;
	}

	int GetListItemCount(UINT uItemID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList)
		{
			return pWndList->GetItemCount();
		}

		return 0;
	}

	BOOL UpdateLayoutList(UINT uItemID)
	{
		BOOL bRet = FALSE;

		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList)
		{
			pWndList->UpdateLayout();
			pWndList->NotifyInvalidate();
			bRet = TRUE;
		}

		return bRet;
	}

	int GetListItemByBkWndHandle(UINT uItemID, HBKWND hBkWnd)
	{
		int nItemRet = -1;

		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList)
		{
			nItemRet = pWndList->GetListItemByBkWndHandle(hBkWnd);
		}

		return nItemRet;
	}

	HBKWND GetListItemBkWndHandle(UINT uItemID, int nListItem)
	{
		HBKWND hBkWnd = NULL;

		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList)
		{
			hBkWnd = pWndList->GetListItemBKHANDLE(nListItem);
		}

		return hBkWnd;
	}

	int GetCurSelItem(UINT uItemID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList)
		{
			return pWndList->GetSelectItem();
		}

		return -1;
	}

	//index from 0 - end
	int AppendListItem(UINT uItemID, LPCSTR szlistItemXml, int nInsert = -1, BOOL bUpdateLayout = TRUE)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL || szlistItemXml == NULL) return -1;

		TiXmlDocument xmlDoc;

		{ // Free stack
			xmlDoc.Parse(szlistItemXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
			return -1;

		return pWndList->AppendListItem(xmlDoc.RootElement(), nInsert, bUpdateLayout);
	}

	int AppendListItem(UINT uItemID, UINT uResID, int nInsert = -1, BOOL bUpdateLayout = TRUE)
	{
		CStringA strXml;

		BOOL bRet = BkResManager::LoadResource(uResID, strXml);

		if (!bRet)
			return -1;

		return AppendListItem(uResID, strXml, nInsert, bUpdateLayout);
	}

	int AppendListItem(UINT uItemID, TiXmlElement* pChlidElem, int nInsert = -1, BOOL bUpdateLayout = TRUE)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return -1;

		return pWndList->AppendListItem(pChlidElem, nInsert, bUpdateLayout);
	}

	BOOL DeleteListItem(UINT uItemID, int nListItem, BOOL bUpdateLayout = TRUE)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->DeleteItem(nListItem, bUpdateLayout);
	}

	BOOL DeleteAllListItem(UINT uItemID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		pWndList->DeleteAllItem();

		return TRUE;
	}

	BOOL SetListItemVisible(UINT uItemID, int nListItem, BOOL bVisible)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemVisible(nListItem, bVisible);
	}

	BOOL SetListItemAttribute(UINT uItemID, int nListItem, LPCSTR lpszAttrib, LPCSTR lpszValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemAttribute(nListItem, lpszAttrib, lpszValue);
	}

	BOOL SetListItemIntAttribute(UINT uItemID, int nListItem, LPCSTR lpszAttrib, int nValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemIntAttribute(nListItem, lpszAttrib, nValue);
	}

	BOOL SetListItemStringAttribute(UINT uItemID, int nListItem, LPCSTR lpszAttrib, LPCTSTR lpszValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemStringAttribute(nListItem, lpszAttrib, lpszValue);
	}

	BOOL SetListItemDWordAttribute(UINT uItemID, int nListItem, LPCSTR lpszAttrib, DWORD dwValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemDWordAttribute(nListItem, lpszAttrib, dwValue);
	}

	BOOL SetListItemColorAttribute(UINT uItemID, int nListItem, LPCSTR lpszAttrib, COLORREF crValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemColorAttribute(nListItem, lpszAttrib, crValue);
	}

	CBkWindow* FindListItemChildCtrlByCmdID(UINT uItemID, int nItem, UINT uChildId)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return NULL;

		return pWndList->FindListItemChildCtrlByCmdID(nItem, uChildId);
	}

	BOOL SetListItemChildCtrlRichText(UINT uItemID, int nListItem, UINT uListItemChildID, UINT uResID)
	{
		CStringA strXml;
		BOOL bRet = BkResManager::LoadResource(uResID, strXml);
		if (!bRet)
			return FALSE;

		return SetListItemChildCtrlRichText(uItemID, nListItem, uListItemChildID, strXml);
	}

	BOOL SetListItemChildCtrlRichText(UINT uItemID, int nListItem, UINT uListItemChildID, LPCWSTR lpszXml)
	{
		CStringA strXml = CW2A(lpszXml, CP_UTF8);

		return SetListItemChildCtrlRichText(uItemID, nListItem, uListItemChildID, strXml);
	}

	BOOL SetListItemChildCtrlRichText(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszXml)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkRichText::GetClassName()))
			return FALSE;

		TiXmlDocument xmlDoc;

		{ // Free stack
			CStringA strXml;
			strXml = L'<';
			strXml += CBkRichText::GetClassName();
			strXml += L'>';
			strXml += lpszXml;
			strXml += L'<';
			strXml += L'/';
			strXml += CBkRichText::GetClassName();
			strXml += L'>';
			xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
			return FALSE;

		CBkRichText *pWndRichText = (CBkRichText *)pWnd;
		pWndRichText->LoadChilds(xmlDoc.RootElement());

		pWndList->_InvalidateControl(pWnd);

		if (pWnd->IsVisible(TRUE))
			pWnd->BkSendMessage(WM_SHOWWINDOW, TRUE);

		return TRUE;
	}

	BOOL SetListItemChildCtrlText(UINT uItemID, int nListItem, UINT uListItemChildID, LPCTSTR lpszText)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);

		if (pWnd)
		{
			pWndList->_InvalidateControl(pWnd);

			pWnd->SetInnerText(lpszText);

			pWndList->_InvalidateControl(pWnd);

			return TRUE;
		}

		return FALSE;
	}

	CString GetListItemChildCtrlText(UINT uItemID, int nListItem, UINT uListItemChildID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return _T("");

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		CString strItemText = _T("");

		if (pWnd)
		{
			pWndList->_InvalidateControl(pWnd);

			strItemText = pWnd->GetInnerText();

			pWndList->_InvalidateControl(pWnd);

			return strItemText;
		}

		return strItemText;
	}

	BOOL SetListItemChildCtrlAttribute(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszAttrib, LPCSTR lpszValue)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
		{
			pWndList->_InvalidateControl(pWnd);

			pWnd->SetAttribute(lpszAttrib, lpszValue, FALSE);

			pWndList->_InvalidateControl(pWnd);

			return TRUE;
		}

		return FALSE;
	}

	BOOL SetListItemChildCtrlStringAttribute(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszAttrib, LPCTSTR lpszValue)
	{
		return SetListItemChildCtrlAttribute(uItemID, nListItem, uListItemChildID, lpszAttrib, CT2A(lpszValue, CP_UTF8));
	}

	BOOL SetListChildCtrlItemIntAttribute(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszAttrib, int nValue)
	{
		CStringA strValue;
		strValue.Format("%d", nValue);

		return SetListItemChildCtrlAttribute(uItemID, nListItem, uListItemChildID, lpszAttrib, strValue);
	}

	BOOL SetListItemChildCtrlDWordAttribute(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszAttrib, DWORD dwValue)
	{
		CStringA strValue;
		strValue.Format("%u", dwValue);

		return SetListItemChildCtrlAttribute(uItemID, nListItem, uListItemChildID, lpszAttrib, strValue);
	}

	BOOL SetListItemChildCtrlColorAttribute(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszAttrib, COLORREF crValue)
	{
		CStringA strValue;
		strValue.Format("%02X%02X%02X", GetRValue(crValue), GetGValue(crValue), GetBValue(crValue));

		return SetListItemChildCtrlAttribute(uItemID, nListItem, uListItemChildID, lpszAttrib, strValue);
	}

	BOOL GetListItemChildCtrlCheck(UINT uItemID, int nListItem, UINT uListItemChildID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
			return pWnd->IsChecked();

		return FALSE;
	}

	BOOL SetListItemChildCtrlCheck(UINT uItemID, int nListItem, UINT uListItemChildID, BOOL bCheck)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
		{
			if (bCheck)
				pWnd->ModifyState(BkWndState_Check, 0);
			else
				pWnd->ModifyState(0, BkWndState_Check);

			pWndList->_InvalidateControl(pWnd);

			return TRUE;
		}

		return FALSE;
	}

	BOOL EnableListItemChildCtrl(UINT uItemID, int nListItem, UINT uListItemChildID, BOOL bEnable)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);

		if (pWnd)
		{
			if (bEnable)
				pWnd->ModifyState(0, BkWndState_Disable);
			else
				pWnd->ModifyState(BkWndState_Disable, BkWndState_Hover);

			pWndList->_InvalidateControl(pWnd);
			return TRUE;
		}

		return FALSE;
	}

	BOOL IsListItemChildCtrlEnable(UINT uItemID, int nListItem, UINT uListItemChildID, BOOL bCheckParent = FALSE)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
			return !pWnd->IsDisabled(bCheckParent);

		return FALSE;
	}

	BOOL SetListItemChildCtrlVisible(UINT uItemID, int nListItem, UINT uListItemChildID, BOOL bVisible)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
		{
			pWnd->BkSendMessage(WM_SHOWWINDOW, (WPARAM)bVisible);

			pWndList->_InvalidateControl(pWnd, FALSE);

			return TRUE;
		}

		return FALSE;
	}

	BOOL IsListItemChildCtrlVisible(UINT uItemID, int nListItem, UINT uListItemChildID, BOOL bCheckParent = FALSE)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (pWnd)
			return pWnd->IsVisible(bCheckParent);

		return FALSE;
	}

	BOOL GetListItemChildCtrlRect(UINT uItemID, int nListItem, UINT uListItemChildID, RECT &rcItem)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		pWnd->GetRect(&rcItem);

		return TRUE;
	}

	BOOL SetListItemChildCtrlNeedReDraw(UINT uItemID, int nListItem, UINT uListItemChildID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		pWndList->_InvalidateControl(pWnd);

		return TRUE;
	}

	BOOL SetListItemChildCtrlRect(UINT uItemID, int nListItem, UINT uListItemChildID, RECT &rcItem)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		pWnd->SetRect(&rcItem);

		return TRUE;
	}

	BOOL SetListItemChildCtrlTabCurSel(UINT uItemID, int nListItem, UINT uListItemChildID, int nPage)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
			return FALSE;

		BOOL bRet = ((CBkTabCtrl *)pWnd)->SetCurSel(nPage);

		return bRet;
	}

	BOOL SetListItemChildCtrlTabTitle(UINT uItemID, int nListItem, UINT uListItemChildID, int nPage, LPCTSTR lpszTitle)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
			return FALSE;

		BOOL bRet = ((CBkTabCtrl *)pWnd)->SetTabTitle(nPage, lpszTitle);

		return bRet;
	}

	int GetListItemChildCtrlTabCurSel(UINT uItemID, int nListItem, UINT uListItemChildID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return -1;

		if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
			return -1;

		return ((CBkTabCtrl *)pWnd)->GetCurSel();
	}

	BOOL IsListItemChildCtrlTabPageVisible(UINT uItemID, int nListItem, UINT uListItemChildID, int nPage)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
			return FALSE;

		return ((CBkTabCtrl *)pWnd)->IsPageVisible(nPage);
	}

	BOOL SetListItemChildCtrlTabPageVisible(UINT uItemID, int nListItem, UINT uListItemChildID, int nPage, BOOL bVisible)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkTabCtrl::GetClassName()))
			return FALSE;


		((CBkTabCtrl *)pWnd)->SetPageVisible(nPage, bVisible);

		return TRUE;
	}

	BOOL SetListItemChildCtrlTabIconHandle(UINT uItemID, int nListItem, UINT uListItemChildID, HICON hIcon)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkIconWnd::GetClassName()))
			return FALSE;

		((CBkIconWnd *)pWnd)->AttachIcon(hIcon);

		pWndList->_InvalidateControl(pWnd);

		return TRUE;
	}

	BOOL SetListItemChildCtrlPaintHook(UINT uItemID, int nListItem, UINT uListItemChildID, IBkWindowPaintHook* pPaintHook)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		pWnd->SetPaintHook(pPaintHook);

		return TRUE;
	}

	BOOL SetListItemChildCtrlMouseHook(UINT uItemID, int nListItem, UINT uListItemChildID, IBkWindowMouseHook *pMouseHook)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkWindow *pWnd = pWndList->FindListItemChildCtrlByCmdID(nListItem, uListItemChildID);
		if (!pWnd)
			return FALSE;

		pWnd->SetMouseHook(pMouseHook);

		return TRUE;
	}


	BOOL FormatListItemChildCtrlRichText(UINT uItemID, int nListItem, UINT uListItemChildID, UINT uStringID, ...)
	{
		CString strFormat = BkString::Get(uStringID);
		va_list args;
		CString strText;

		va_start(args, uStringID);

		strText.FormatV(strFormat, args);

		return SetListItemChildCtrlRichText(uItemID, nListItem, uListItemChildID, strText);
	}

	BOOL FormatListItemChildCtrlRichText(UINT uItemID, int nListItem, UINT uListItemChildID, LPCTSTR lpszFormat, ...)
	{
		va_list args;
		CString strText;

		va_start(args, lpszFormat);

		strText.FormatV(lpszFormat, args);

		return SetListItemChildCtrlRichText(uItemID, nListItem, uListItemChildID, strText);
	}

	BOOL FormatListItemChildCtrlText(UINT uItemID, int nListItem, UINT uListItemChildID, LPCTSTR lpszFormat, ...)
	{
		va_list args;
		CString strText;

		va_start(args, lpszFormat);

		strText.FormatV(lpszFormat, args);

		return SetListItemChildCtrlText(uItemID, nListItem, uListItemChildID, strText);
	}


	BOOL SetListItemPanelXml(UINT uItemID, int nListItem, LPCSTR lpszXml)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;
		
		CBkListItem* listItem = pWndList->GetListItem(nListItem);
		if (listItem == NULL) return FALSE;

		return pWndList->SetListItemPanelXml(nListItem, lpszXml);
	}

	BOOL SetListItemPanelXml(UINT uItemID, int nListItem, UINT uResID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		CBkListItem* listItem = pWndList->GetListItem(nListItem);
		if (listItem == NULL) return FALSE;

		return pWndList->SetListItemPanelXml(nListItem, uResID);
	}

	BOOL SetListItemChildCtrlPanelXml(UINT uItemID, int nListItem, UINT uListItemChildID, UINT uResID)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemChildCtrlPanelXml(nListItem, uListItemChildID, uResID);
	}

	BOOL SetListItemChildCtrlPanelXml(UINT uItemID, int nListItem, UINT uListItemChildID, LPCSTR lpszXml)
	{
		CBkListWnd *pWndList = FindListWnd(uItemID);
		if (pWndList == NULL) return FALSE;

		return pWndList->SetListItemChildCtrlPanelXml(nListItem, uListItemChildID, uResID);
	}
};

