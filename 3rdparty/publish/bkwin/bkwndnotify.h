#pragma once

//////////////////////////////////////////////////////////////////////////
// Notify Messages For BkWin User

#define MSG_BK_NOTIFY(ctrlid) NOTIFY_ID_HANDLER_EX(ctrlid, OnBkNotify_##ctrlid)

#define BK_NOTIFY_MAP(ctrlid)                   \
    LRESULT OnBkNotify_##ctrlid(LPNMHDR pnmh)   \
    {                                           \
        UINT_PTR  uCode = pnmh->code;           \

#define BK_NOTIFY_MAP_END()                     \
        SetMsgHandled(FALSE);                   \
        return FALSE;                           \
    }                                           \

#define BK_CHAIN_NOTIFY_MAP(ChainClass, ctrlid)                 \
        if(ChainClass::OnBkNotify_##ctrlid(pnmh))               \
            return TRUE;                                        \

// Command Notify
#define BKNM_COMMAND  1

// void OnBkItemCommand()
#define BK_NOTIFY_ID_COMMAND(itemid, func)                                  \
    if (BKNM_COMMAND == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)  \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \


// void OnBkItemCommand(ULONG nID)
#define BK_NOTIFY_ID_COMMAND_RANGE(itemidStart,itemidend,func)              \
	if (BKNM_COMMAND == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
	{                                                                       \
	func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
	return TRUE;															\
	}																		\

// void OnBkCommand(UINT uItemID, LPCSTR szItemClass)
#define BK_NOTIFY_COMMAND(func)                                                     \
    if (BKNM_COMMAND == uCode)                                                      \
    {                                                                               \
        func(((LPBKNMCOMMAND)pnmh)->uItemID, ((LPBKNMCOMMAND)pnmh)->szItemClass);   \
        return TRUE;                                                                \
    }                                                                               \

typedef struct _BKNMCOMMAND
{
    NMHDR       hdr;
    UINT        uItemID;
    LPCSTR      szItemClass;
} BKNMCOMMAND, *LPBKNMCOMMAND;

// Real Window Resized Notify
#define BKNM_REALWND_RESIZED 2

// void OnBkRealWndResized(CRect rcWnd)
#define BK_NOTIFY_REALWND_RESIZED(itemid, func)                                             \
    if (BKNM_REALWND_RESIZED == uCode && itemid == ((LPBKNMREALWNDRESIZED)pnmh)->uItemID)   \
    {                                                                                       \
        func(((LPBKNMREALWNDRESIZED)pnmh)->rcWnd);                                          \
        return TRUE;                                                                        \
    }                                                                                       \

typedef struct _BKNMREALWNDRESIZED
{
    NMHDR       hdr;
    UINT        uItemID;
    CRect       rcWnd;
} BKNMREALWNDRESIZED, *LPBKNMREALWNDRESIZED;

// Mouse Hover Notify
// void OnBkMouseHover()
#define BKNM_MOUSEHOVER 3

#define BK_NOTIFY_MOUSEHOVER(func)                                          \
    if (BKNM_MOUSEHOVER == uCode)                                           \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \

#define BK_NOTIFY_MOUSEHOVER_EX(itemid, func)                                          \
	if (BKNM_MOUSEHOVER == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
}                                                                       \

// void OnBkMouseHover(ULONG nID)
#define BK_NOTIFY_MOUSEHOVER_RANGE(itemidStart,itemidend,func)              \
	if (BKNM_MOUSEHOVER == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
{                                                                       \
	func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
	return TRUE;															\
}


// Mouse Leave Notify
// void OnBkMouseLeave()
#define BKNM_MOUSELEAVE 4

#define BK_NOTIFY_MOUSELEAVE(func)                                          \
    if (BKNM_MOUSELEAVE == uCode)                                           \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \

#define BK_NOTIFY_MOUSELEAVE_EX(itemid, func)                                          \
	if (BKNM_MOUSELEAVE == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
}     

// void OnBkMouseLeave(ULONG nID)
#define BK_NOTIFY_MOUSELEAVE_RANGE(itemidStart,itemidend,func)              \
    if (BKNM_MOUSELEAVE == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
{                                                                       \
    func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
    return TRUE;															\
}		


// Tab Sel Change Notify
#define BKNM_TAB_SELCHANGE  5

typedef struct _BKNMTABSELCHANGE
{
    NMHDR       hdr;
    UINT        uTabID;
    UINT        uTabItemIDNew;
    UINT        uTabItemIDOld;
    BOOL        bCancel;
} BKNMTABSELCHANGE, *LPBKNMTABSELCHANGE;

// BOOL OnBkTabSelChange(int nTabItemIDOld, int nTabItemIDNew)
#define BK_NOTIFY_TAB_SELCHANGE(tabid, func)                                        \
    if (BKNM_TAB_SELCHANGE == uCode && tabid == ((LPBKNMTABSELCHANGE)pnmh)->uTabID) \
    {                                                                               \
        BOOL bRet = func(                                                           \
            ((LPBKNMTABSELCHANGE)pnmh)->uTabItemIDOld,                              \
            ((LPBKNMTABSELCHANGE)pnmh)->uTabItemIDNew);                             \
        if (!bRet)                                                                  \
            ((LPBKNMTABSELCHANGE)pnmh)->bCancel = TRUE;                             \
        return TRUE;                                                                \
    }                                                                               \

#define BKNM_NOTIFY 6

typedef struct _BKNMNOTIFY
	: BKNMCOMMAND
{
	UINT		uNotifyMsg;

} BKNMNOTIFY, *LPBKNMNOTIFY;

// void OnBkNotifyMsg(UINT uItemID, UINT uNotifyMsg)
#define BK_NOTIFY_MSG(func)															\
	if (BKNM_NOTIFY == uCode)														\
{																					\
	func(((LPBKNMNOTIFY)pnmh)->uItemID, ((LPBKNMNOTIFY)pnmh)->uNotifyMsg);			\
	return TRUE;																	\
}																					\

// Mouse LButtonDown Notify
// void OnBkMouseLButtonDown()
#define BKNM_MOUSELBUTTONDOWN 7

#define BK_NOTIFY_MOUSELBUTTONDWON(itemid, func)                                          \
	if (BKNM_MOUSELBUTTONDOWN == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
}                                                                       \

// void OnBkMouseLButtonDown(ULONG nID)
#define BK_NOTIFY_MOUSELBUTTONDWON_RANGE(itemidStart, itemidend, func)              \
	if (BKNM_MOUSELBUTTONDOWN == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
{                                                                       \
	func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
	return TRUE;															\
}

// Mouse LButtonDown Notify
// void OnBkMouseLButtonDblClk()
#define BKNM_MOUSELBUTTONDBLCLK 8

#define BK_NOTIFY_MOUSELBUTTONDBLCLK(itemid, func)                                          \
	if (BKNM_MOUSELBUTTONDBLCLK == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
}                                                                       \

// void OnBkMouseLButtonDblClk(ULONG nID)
#define BK_NOTIFY_MOUSELBUTTONDBLCLK_RANGE(itemidStart, itemidend, func)              \
	if (BKNM_MOUSELBUTTONDBLCLK == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
{                                                                       \
	func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
	return TRUE;															\
}


#define BKNM_MOUSERBUTTONDOWN 9

#define BK_NOTIFY_MOUSERBUTTONDWON(itemid, func)                                          \
	if (BKNM_MOUSERBUTTONDOWN == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)       \
{                                                                       \
	func();                                                             \
	return TRUE;                                                        \
}                                                                       \

// void OnBkMouseRButtonDown(ULONG nID)
#define BK_NOTIFY_MOUSERBUTTONDWON_RANGE(itemidStart, itemidend, func)              \
	if (BKNM_MOUSERBUTTONDOWN == uCode && (itemidStart <= ((LPBKNMCOMMAND)pnmh)->uItemID) && (itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID) )  \
{                                                                       \
	func(((LPBKNMCOMMAND)pnmh)->uItemID);									\
	return TRUE;															\
}

// Tab Sel Change Notify
#define BKNM_TAB_HOVERCHANGE  10

typedef struct _BKNMTABHOVERCHANGE
{
	NMHDR       hdr;
	UINT        uTabID;
	UINT        uTabItemIDNew;
	UINT        uTabItemIDOld;
} BKNMTABHOVERCHANGE, *LPBKNMTABHOVERCHANGE;

// BOOL OnBkTabSelChange(int nTabItemIDOld, int nTabItemIDNew)
#define BK_NOTIFY_TAB_HOVCHANGE(tabid, func)                                        \
	if (BKNM_TAB_HOVERCHANGE == uCode && tabid == ((LPBKNMTABHOVERCHANGE)pnmh)->uTabID) \
{                                                                               \
	func(                                                           \
	((LPBKNMTABHOVERCHANGE)pnmh)->uTabItemIDOld,                              \
	((LPBKNMTABHOVERCHANGE)pnmh)->uTabItemIDNew);                             \
	return TRUE;                                                                \
}

#define BKNM_SLIDER_CHANGING 11

typedef struct _BKNMSLIDERCHANGE
{
	NMHDR       hdr;
	UINT        uSliderID;
	UINT        uPosOld;
	UINT        uPosNew;
	BOOL		bCancel;
} BKNMSLIDERCHANGE, *LPBKNMSLIDERCHANGE;

//BOOL OnBkSliderChanging(UINT uPosOld, UINT uPosNew, BOOL &bCancel)
#define BK_NOTIFY_SLIDER_CHANGING(sliderid, func) \
	if (BKNM_SLIDER_CHANGING == uCode && sliderid == ((LPBKNMSLIDERCHANGE)pnmh)->uSliderID) \
{\
	func(((LPBKNMSLIDERCHANGE)pnmh)->uPosOld, ((LPBKNMSLIDERCHANGE)pnmh)->uPosNew, ((LPBKNMSLIDERCHANGE)pnmh)->bCancel); \
	return TRUE; \
}

#define BKNM_SLDER_CHANGED 12

//BOOL OnBkSliderChanged(UINT uPosOld, UINT uPosNew, BOOL &bCancel)
#define BK_NOTIFY_SLIDER_CHANGED(sliderid, func) \
	if (BKNM_SLDER_CHANGED == uCode && sliderid == ((LPBKNMSLIDERCHANGE)pnmh)->uSliderID) \
{\
	func(((LPBKNMSLIDERCHANGE)pnmh)->uPosOld, ((LPBKNMSLIDERCHANGE)pnmh)->uPosNew, ((LPBKNMSLIDERCHANGE)pnmh)->bCancel); \
	return TRUE; \
}