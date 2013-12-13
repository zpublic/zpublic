#pragma  once

#define  WM_BK_NOTIFY_ONMOUSE_SHELL			(WM_APP + 500)
#define  WM_BK_UPDATE_TIP_TEXT				(WM_APP + 501)
#define  WM_BK_LIST_OFFSET					(WM_APP + 502)
#define  WM_BK_GETREAL_CONTAINER_WND		(WM_APP + 503)
#define  WM_BK_IS_LAYERED_WND				(WM_APP + 504)
#define  WM_BK_CONTAINERWND_MOVE			(WM_APP + 505)
#define  WM_BK_GET_LIST_ITEM_INDEX_BY_BKWND (WM_APP + 506)

#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED        0x031E
#endif
// void OnCompisitionChanged()
#define MSG_WM_DWMCOMPOSITIONCHANGED(func) \
	if (uMsg == WM_DWMCOMPOSITIONCHANGED) \
	{ \
	SetMsgHandled(TRUE); \
	func(); \
	lResult = 0; \
	if(IsMsgHandled()) \
	return TRUE; \
	}

#define  BKNM_MOUSERBUTTONUP 13
#define  IDC_RICHVIEW_WIN_EX ((UINT)(-1))

#define BK_NOTIFY_MOUSERBUTTONUP(itemid, func)                                          \
	if (BKNM_MOUSERBUTTONUP == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
} 

#define BKNM_LISTWND_NOTIFY 14

//BKNM_LISTWND_SUBNOTIFY
#define BKNM_LISTWND_SCROLL_LBUTTONUP					1
#define BKNM_LISTWND_SCROLL_LBUTTONDOWN					2
#define BKNM_LISTWND_SCROLL_RBUTTONUP					3
#define BKNM_LISTWND_SCROLL_RBUTTONDOWN					4
#define BKNM_LISTWND_SCROLL_MOUSE_HOVER					5
#define BKNM_LISTWND_SCROLL_MOUSE_LEAVE					6
#define BKNM_LISTWND_SCROLL_LBUTTON_DBCLICK				7

#define BKNM_LISTWND_LISTITEM_MOUSEHOVER				21
#define BKNM_LISTWND_LISTITEM_LBUTTONDOWN				22
#define BKNM_LISTWND_LISTITEM_LBUTTONUP					23
#define BKNM_LISTWND_LISTITEM_RBUTTONDOWN				24
#define BKNM_LISTWND_LISTITEM_RBUTTONUP					25
#define BKNM_LISTWND_LISTITEM_MOUSELEAVE				26
#define BKNM_LISTWND_LISTITEM_LBUTTON_DBCLICK			27

#define BKNM_LISTWND_LISTITEM_CHILD_MOUSEHOVER			101
#define BKNM_LISTWND_LISTITEM_CHILD_LBUTTONDOWN			102
#define BKNM_LISTWND_LISTITEM_CHILD_LBUTTONUP			103
#define BKNM_LISTWND_LISTITEM_CHILD_RBUTTONDOWN			104
#define BKNM_LISTWND_LISTITEM_CHILD_RBUTTONUP			105
#define BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE			106
#define BKNM_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK		107

typedef struct
{
	UINT uCmdID;
	LPCSTR      szItemClass;
}BKLISTEMCHILDNOFITY, *PBKLISTEMCHILDNOFITY;

typedef struct
{
	NMHDR       hdr;
	UINT		uItemID;		// listwnd ctrl id
	UINT		uListCode;	    // listÍ¨ÖªÂë
	int			nItem;	        // list's item index
	LPCSTR		szListClass;    // listwnd's class name

	UINT		uListItemID;    // listWnd's item or scrollbar's ID
	LPCSTR		szListItemClass;// listWnd's item or scrollbar class name

	BKLISTEMCHILDNOFITY childNotify;

}BKNOTIFYLISTITEM, *PBKNOTIFYLISTITEM;

#define BK_LISTWND_NOTIFY_BEGIN(ctrlid) \
if (BKNM_LISTWND_NOTIFY == uCode && ctrlid == ((PBKNOTIFYLISTITEM)pnmh)->uItemID)\
{\
	UINT uListCode = ((PBKNOTIFYLISTITEM)pnmh)->uListCode;

#define BK_LISTWND_NOTIFY_END() return TRUE;}

                                      
#define BK_LISTWND_SCROLL_LBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_LBUTTONUP)\
	{\
		func();\
	}
#define BK_LISTWND_SCROLL_LBUTTONDOWN(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_LBUTTONDOWN)\
	{\
	func();\
	}

#define BK_LISTWND_SCROLL_RBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_RBUTTONUP)\
	{\
		func();\
	}

#define BK_LISTWND_SCROLL_MOUSEHOVER(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_MOUSE_HOVER)\
	{\
		func();\
	}

#define BK_LISTWND_SCROLL_MUSELEAVE(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_MOUSE_LEAVE)\
	{\
		func();\
	}

#define BK_LISTWND_SCROLL_LBUTTON_DBCLICK(func)\
	if(uListCode == BKNM_LISTWND_SCROLL_LBUTTON_DBCLICK)\
	{\
	func();\
	}

//////////////////////////////////////////
////////////////////////////////////////////////

#define BK_LISTWND_LISTITEM_MOUSEHOVER(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_MOUSEHOVER)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_LBUTTONDOWN(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_LBUTTONDOWN)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_LBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_LBUTTONUP)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_RBUTTONDOWN(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_RBUTTONDOWN)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_RBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_RBUTTONUP)\
	{\
		func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_MOUSELEAVE(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_MOUSELEAVE)\
	{\
		func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}

#define BK_LISTWND_LISTITEM_LBUTTON_DBCLICK(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_LBUTTON_DBCLICK)\
	{\
		func(((PBKNOTIFYLISTITEM)pnmh)->nItem);\
	}
/////////////////////////////////////////////////////

#define BK_LISTWND_LISTITEM_CHILD_MOUSEHOVER(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_MOUSEHOVER)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTONDOWN(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTONDOWN)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTONUP)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_RBUTTONDOWN(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_RBUTTONDOWN)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_RBUTTONUP(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_RBUTTONUP)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_MOUSELEAVE(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK)\
	{\
		func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID);\
	}

/////////////////////////////////////////////////////
//void OnListItemChildCtrlSoftItemLButtonUp( int nListItem, UINT uItemID, LPCSTR szListItemChildClass);

#define BK_LISTWND_LISTITEM_CHILD_MOUSEHOVER_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_MOUSEHOVER)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTONDOWN_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTONDOWN)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTONUP_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTONUP)\
{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_RBUTTONDOWN_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_RBUTTONDOWN)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_RBUTTONUP_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_RBUTTONUP)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_MOUSELEAV_EXE(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}

#define BK_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK_EX(func)\
	if(uListCode == BKNM_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK)\
	{\
	func(((PBKNOTIFYLISTITEM)pnmh)->nItem, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.uCmdID, ((PBKNOTIFYLISTITEM)pnmh)->childNotify.szItemClass);\
	}
