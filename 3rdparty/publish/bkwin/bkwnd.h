//////////////////////////////////////////////////////////////////////////
//   File Name: BkWnd.h
// Description: BkWindow Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwndstyle.h"
#include <bkres/bkstringpool.h>

class CBkWindow;

// by dummyz@126.com
class IBkWindowPaintHook
{
public:
	virtual BOOL NotifyPaint(CBkWindow* win, HDC hDC, const RECT& rcWin, BOOL bBeforePaint) = 0;
};

class IBkWindowMouseHook
{
public:
	virtual BOOL NotifyMouseEvent(UINT nMsg, CBkWindow *win, const RECT &rcWin, const CPoint & pt) = 0;
};

//////////////////////////////////////////////////////////////////////////

// Not Used
#define BKWIN_DECLARE_RETURN_SETTING(before, after)     \
public:                                                 \
    virtual BOOL ReturnBeforeThisControl(void)          \
    {                                                   \
        return before;                                  \
    }                                                   \
    virtual BOOL ReturnAfterThisControl(void)           \
    {                                                   \
        return after;                                   \
    }                                                   \

//////////////////////////////////////////////////////////////////////////

// BkWindow Message Map Define
// Use WTL Message Map Ex (include atlcrack.h)
#define BKWIN_BEGIN_MSG_MAP()                                       \
protected:                                                          \
	virtual BOOL ProcessWindowMessage(                              \
        HWND hWnd, UINT uMsg, WPARAM wParam,                        \
        LPARAM lParam, LRESULT& lResult)                            \
	{                                                               \

#define BKWIN_END_MSG_MAP()                                         \
        if (!IsMsgHandled())                                        \
            return __super::ProcessWindowMessage(                   \
                hWnd, uMsg, wParam, lParam, lResult);               \
        return TRUE;                                                \
	}                                                               \

#define BKWIN_END_MSG_MAP_BASE()                                    \
        return TRUE;                                                \
	}                                                               \


//////////////////////////////////////////////////////////////////////////
// CBkWindow Handle Manager
//////////////////////////////////////////////////////////////////////////

class CBkWindow;

// BkWindow Handle
typedef DWORD HBKWND;

class BkWnds
{
public:

    BkWnds()
        : m_hNextWnd(NULL)
    {
		::InitializeCriticalSection(&ms_lockWndMap);
    }

    ~BkWnds()
    {
		::DeleteCriticalSection(&ms_lockWndMap);
    }

    // Get BkWindow pointer from handle
    static CBkWindow* GetWindow(HBKWND hBkWnd)
    {
		::EnterCriticalSection(&ms_lockWndMap);

        const _BkWndHandlePool::CPair *pairRet = _Instance()->m_mapPool.Lookup(hBkWnd);

		::LeaveCriticalSection(&ms_lockWndMap);

        if (pairRet)
        {
            return pairRet->m_value;
        }
        else
            return NULL;
    }

    // Get BkWindow pointer from command id
    static CBkWindow* GetWindow(UINT uCmdID, HWND hWndInContainer = NULL);

    // Specify a handle to a BkWindow
    static HBKWND NewWindow(CBkWindow *pBkWnd)
    {
        //static int s_n_new = 0;
        //ATLTRACE(_T("NewWindow %d\r\n"), ++ s_n_new);

        if (pBkWnd)
        {
			_Instance();

			::EnterCriticalSection(&ms_lockWndMap);

            HBKWND hBkWndNext = ++ _Instance()->m_hNextWnd;
            _Instance()->m_mapPool[hBkWndNext] = pBkWnd;

			::LeaveCriticalSection(&ms_lockWndMap);

			return hBkWndNext;
        }
        else
            return NULL;
    }

    // Destroy BkWindow
    static BOOL DestroyWindow(HBKWND hBkWnd)
    {
        //static int s_n_del = 0;
        //ATLTRACE(_T("DestroyWindow %d\r\n"), ++ s_n_del);

		::EnterCriticalSection(&ms_lockWndMap);

		BOOL bRet = (BOOL)_Instance()->m_mapPool.RemoveKey(hBkWnd);

		::LeaveCriticalSection(&ms_lockWndMap);

        return bRet;
    }

    static size_t GetCount()
    {
		::EnterCriticalSection(&ms_lockWndMap);

		size_t nRet = _Instance()->m_mapPool.GetCount();

		::LeaveCriticalSection(&ms_lockWndMap);

        return nRet;
    }

    static BOOL RegisterRadioGroup(CBkWindow *pbkWnd, LPCSTR lpszGroupName)
    {
		::EnterCriticalSection(&ms_lockWndMap);

        POSITION pos = _Instance()->m_mapRadioPool[lpszGroupName].AddTail(pbkWnd);

		::LeaveCriticalSection(&ms_lockWndMap);

        return (NULL != pos);
    }

    static void UnregisterRadioGroup(CBkWindow *pbkWnd, LPCSTR lpszGroupName)
    {
		::EnterCriticalSection(&ms_lockWndMap);

        CAtlList<CBkWindow *> &ListGroup = _Instance()->m_mapRadioPool[lpszGroupName];

        POSITION posCurrent = NULL, pos = ListGroup.GetHeadPosition();

        while (pos)
        {
            posCurrent = pos;

            if (ListGroup.GetNext(pos) != pbkWnd)
                continue;

            ListGroup.RemoveAt(posCurrent);
        }

		::LeaveCriticalSection(&ms_lockWndMap);

    }

    static void SelectRadio(CBkWindow* pbkWnd, LPCSTR lpszGroupName, CBkWindow **ppbkWndLastSelect);
//     {
//         CAtlList<CBkWindow *> &ListGroup = _Instance()->m_mapRadioPool[lpszGroupName];
// 
//         pbkWnd->ModifyState(BkWndState_Check, 0);
// 
//         POSITION posCurrent = NULL, pos = ListGroup.GetHeadPosition();
// 
//         while (pos)
//         {
//             posCurrent = pos;
//             CBkWindow *pbkWndFind = ListGroup.GetNext(pos);
// 
//             if (pbkWndFind == pbkWnd)
//                 continue;
// 
//             if (pbkWndFind->IsChecked())
//             {
//                 pbkWndFind->ModifyState(0, BkWndState_Check);
//                 if (ppbkWndLastSelect)
//                     *ppbkWndLastSelect = pbkWndFind;
//             }
//         }
//     }
	static bool IsSelectedRadio(LPCSTR lpszGroupName, CBkWindow **ppbkWndLastSelect);

protected:

    static BkWnds* ms_pInstance;
	static CRITICAL_SECTION ms_lockWndMap;

    static BkWnds* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkWnds;
        return ms_pInstance;
    }

//     static BkWnds& _Instance()
//     {
//         static BkWnds s_obj;
//         return s_obj;
//     }

    typedef CAtlMap<HBKWND, CBkWindow *> _BkWndHandlePool;
    typedef CAtlMap<CStringA, CAtlList<CBkWindow *>> _BkWndGroupPool;

    _BkWndHandlePool m_mapPool;
    _BkWndGroupPool m_mapRadioPool;
    HBKWND m_hNextWnd;
};

__declspec(selectany) BkWnds* BkWnds::ms_pInstance = NULL;
__declspec(selectany) CRITICAL_SECTION BkWnds::ms_lockWndMap;

//////////////////////////////////////////////////////////////////////////
// Inner Notify (BkWindows to Container)

#define BKINM_FIRST             1000
#define BKINM_LAST              1999

// Invalidate Rect Notify
#define BKINM_INVALIDATERECT    BKINM_FIRST

typedef struct _BKINMINVALIDATERECT
{
    NMHDR       hdr;
    HBKWND      hbkWnd;
    RECT        rect;
} BKINMINVALIDATERECT, *LPBKINMINVALIDATERECT;

//////////////////////////////////////////////////////////////////////////
// CBkWindow Implement
//////////////////////////////////////////////////////////////////////////

class CBkWindow : public CBkObject
{
public:
    CBkWindow()
        : m_hBkWnd(BkWnds::NewWindow(this))
        , m_hBkWndParent(NULL)
        , m_hWndContainer(NULL)
        , m_bMsgHandled(FALSE)
        , m_uCmdID(NULL)
        , m_uPositionType(0)
        , m_lSpecifyWidth(0)
        , m_lSpecifyHeight(0)
        , m_dwState(BkWndState_Normal)
        , m_uVAlign(0)
        , m_uHAlign(0)
        , m_bTransparent(FALSE)
		, m_nNotifyMsg(0)
        , m_crBg(CLR_INVALID)
        , m_crText(CLR_INVALID)
        , m_ftText(NULL)
        , m_nTextAlign(0)
		, m_pPaintHook(NULL)
		, m_pMouseHook(NULL)
		, m_nAlphaTxt(-1) //alpha不能为255，不然在xp下会出现文字异常,默认为-1（表示无效）
		, m_bHeadAcceptLBtnDownMsg(FALSE)// 对话框head部分：除去通用控件可享有头部LbtnDownMsg，其他的控件是否结束LbtnDownMsg
    {
		m_rcWindow.SetRectEmpty();
    }
    virtual ~CBkWindow()
    {
        BkWnds::DestroyWindow(m_hBkWnd);
    }

    enum {
        // Specify by "width" attribute
        SizeX_Mask          = 0x0007UL, 
        SizeX_Specify       = 0x0001UL, // width > 0
        SizeX_FitContent    = 0x0002UL, // width <= 0
        SizeX_FitParent     = 0x0004UL, // width = "full" default

        // Specify by "height" attribute
        SizeY_Mask          = 0x0070UL, 
        SizeY_Specify       = 0x0010UL, // height > 0
        SizeY_FitContent    = 0x0020UL, // height <= 0 default
        SizeY_FitParent     = 0x0040UL, // height = "full" default
        // Specify by "float" attribute

        Position_Mask       = 0x0300UL, 
        Position_Relative   = 0x0100UL, // float = 0 default
        Position_Float      = 0x0200UL, // float = 1

        // Specify by "valign" and "align" attribute, only using at float = 1 or panel control (Vert-Align)
        Align_Mask          = 0xF000UL, 
        VAlign_Top          = 0x0000UL, // valign = top
        VAlign_Middle       = 0x1000UL, // valign = middle
        VAlign_Bottom       = 0x2000UL, // valign = bottom
        Align_Left          = 0x0000UL, // align = left
        Align_Center        = 0x4000UL, // align = center
        Align_Right         = 0x8000UL, // align = right
    };

    struct BKDLG_POSITION_ITEM
    {
        BOOL bCenter;
        BOOL bMinus;
        int  nPos;
    };

    struct BKDLG_POSITION
    {
        int nCount;
        union
        {
            struct
            {
                BKDLG_POSITION_ITEM Left;
                BKDLG_POSITION_ITEM Top;
                BKDLG_POSITION_ITEM Right;
                BKDLG_POSITION_ITEM Bottom;
            };
            BKDLG_POSITION_ITEM Item[4];
        };
    };

protected:
    HBKWND m_hBkWnd;
    HBKWND m_hBkWndParent;
    HWND m_hWndContainer;
    UINT m_uCmdID;
    CRect m_rcWindow;
    UINT m_uPositionType;
    CString m_strInnerText;
    LONG m_lSpecifyWidth;
    LONG m_lSpecifyHeight;
    DWORD m_dwState;
    UINT m_uVAlign;
    UINT m_uHAlign;
    CString m_strLinkUrl;
    BOOL m_bTransparent;
    CString m_strToolTipText;
    CStringA m_strStyleName;
    COLORREF m_crBg;
    COLORREF m_crText;
    HFONT    m_ftText;
    int m_nTextAlign;
	int m_nAlphaTxt;
	BOOL m_bHeadAcceptLBtnDownMsg;

    BKDLG_POSITION m_dlgpos;
	IBkWindowPaintHook* m_pPaintHook;
	IBkWindowMouseHook* m_pMouseHook;

public:

    //////////////////////////////////////////////////////////////////////////
    // Method Define

    // Get align
    UINT GetAlign()
    {
        return m_uVAlign | m_uHAlign;
    }

    // Get position type
    DWORD GetPositionType()
    {
        return m_uPositionType;
    }

    // Set position type
    void SetPositionType(DWORD dwPosType, DWORD dwMask = 0xFFFFFFFF)
    {
        m_uPositionType = (m_uPositionType & ~dwMask) | (dwPosType & dwMask);
    }

    // Get BkWindow rect(position in container)
    void GetRect(LPRECT prect)
    {
        if (prect)
        {
            prect->left     = m_rcWindow.left;
            prect->right    = m_rcWindow.right;
            prect->top      = m_rcWindow.top;
            prect->bottom   = m_rcWindow.bottom;
        }
    }

    void SetRect(LPRECT prect)
    {
        if (prect)
        {
            m_rcWindow.left =  prect->left;
            m_rcWindow.right = prect->right;
            m_rcWindow.top = prect->top;
            m_rcWindow.bottom = prect->bottom;
        }
    }

    void GetDlgPosition(BKDLG_POSITION *pPos)
    {
        if (pPos)
            memcpy(pPos, &m_dlgpos, sizeof(BKDLG_POSITION));
    }

    // Get inner text
    LPCTSTR GetInnerText()
    {
        return m_strInnerText;
    }

    // Get tooltip text
   virtual  LPCTSTR GetToolTipText()
    {
        return m_strToolTipText;
    }

   virtual BOOL IsHeadAcceptLBtnDownMsg()
   {
	   return m_bHeadAcceptLBtnDownMsg;
   }

    // Set inner text
    HRESULT SetInnerText(LPCTSTR lpszText)
    {
        m_strInnerText = lpszText;

        if ((m_uPositionType & SizeX_FitContent | SizeY_FitContent) || (CLR_INVALID == m_crBg && CLR_INVALID == GetStyle().m_crBg))
        {
            if ((m_uPositionType & (SizeX_FitContent | SizeY_FitContent)) && (4 != m_dlgpos.nCount))
            {
                SIZE sizeRet = {0, 0};

                BkSendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)&sizeRet);

                if (m_uPositionType & SizeX_FitContent)
                    m_rcWindow.right = m_rcWindow.left + sizeRet.cx;
                if (m_uPositionType & SizeY_FitContent)
                    m_rcWindow.bottom = m_rcWindow.top + sizeRet.cy;
            }

            return S_OK;
        }
        else
            return S_FALSE;
    }

    // Send a message to BkWindow
    LRESULT BkSendMessage(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
    {
        LRESULT lResult = 0;

        SetMsgHandled(FALSE);
        ProcessWindowMessage(NULL, Msg, wParam, lParam, lResult);

        return lResult;
    }

	virtual void BkSendExtendMessage(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{

	}

    // Move BkWindow to new place
    //
    // NOTICE: Dangerous
    void Move(LPRECT prect)
    {
        if (prect)
            m_rcWindow = prect;
    }

    // Set current cursor, when hover
    virtual void SetCursor()
    {
        HCURSOR hCur = ::LoadCursor(NULL, GetStyle().m_lpCursorName);
        ::SetCursor(hCur);
    }

    // Get BkWindow state
    DWORD GetState(void)
    {
        return m_dwState;
    }

    // Modify BkWindow state
    virtual DWORD ModifyState(DWORD dwStateAdd, DWORD dwStateRemove)
    {
        DWORD dwOldState = m_dwState;

        m_dwState |= dwStateAdd;
        m_dwState &= ~dwStateRemove;

        return dwOldState;
    }

    // Get Command ID
    UINT GetCmdID()
    {
        return m_uCmdID;
    }

    HBKWND GetBkHWnd()
    {
        return m_hBkWnd;
    }

    HBKWND GetParent()
    {
        return m_hBkWndParent;
    }

    void SetParent(HBKWND hBkWndParent)
    {
        m_hBkWndParent = hBkWndParent;
    }

    BOOL IsChecked()
    {
        return BkWndState_Check == (m_dwState & BkWndState_Check);
    }

    BOOL IsDisabled(BOOL bCheckParent = FALSE)
    {
        BOOL bDisable = (BkWndState_Disable == (m_dwState & BkWndState_Disable));

        if (bCheckParent && !bDisable)
        {
            CBkWindow *pWndParent = this;

            while (NULL != (pWndParent = BkWnds::GetWindow(pWndParent->GetParent())))
            {
                if (pWndParent->IsDisabled())
                    return TRUE;
            }
        }

        return bDisable;
    }

    BOOL IsVisible(BOOL bCheckParent = FALSE)
    {
        BOOL bVisible = (0 == (m_dwState & BkWndState_Invisible));

        if (bCheckParent && bVisible)
        {
            CBkWindow *pWndParent = this;

            while (NULL != (pWndParent = BkWnds::GetWindow(pWndParent->GetParent())))
            {
                if (!pWndParent->IsVisible())
                    return FALSE;
            }
        }

        return bVisible;
    }

    void SetVisible(BOOL bVisible)
    {
        if (bVisible)
            ModifyState(0, BkWndState_Invisible);
        else
            ModifyState(BkWndState_Invisible, 0);
    }

    BOOL NeedRedrawParent()
    {
        return (GetStyle().m_strSkinName.IsEmpty() && (m_crBg == CLR_INVALID) && (GetStyle().m_crBg == CLR_INVALID));
    }

//     void Invalidate()
//     {
//         m_bNeedRedraw = TRUE;
//     }
// 
//     void Validate()
//     {
//         m_bNeedRedraw = FALSE;
//     }

    LPCTSTR GetLinkUrl()
    {
        return m_strLinkUrl;
    }

    // Get container, container is a REAL window
    HWND GetContainer()
    {
        return m_hWndContainer;
    }

    BOOL IsTransparent()
    {
        return m_bTransparent;
    }

    //BkStyle m_style;

    const BkStyle& GetStyle()
    {
        return BkStyle::GetStyle(m_strStyleName);
    }

	HFONT GetFont()
	{
		return m_ftText;
	}

	void SetPaintHook(IBkWindowPaintHook* pPaintHook)
	{
		m_pPaintHook = pPaintHook;
	}

	void SetMouseHook(IBkWindowMouseHook* pMouseHook)
	{
		m_pMouseHook = pMouseHook;
	}
	
public:

    //////////////////////////////////////////////////////////////////////////
    // Virtual functions

    // Create BkWindow from xml element
    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!pTiXmlElem)
            return FALSE;

        {
            m_strInnerText = CA2T(pTiXmlElem->GetText(), CP_UTF8);
            if (!m_strInnerText.IsEmpty())
            {
                int nSubStringStart = 0, nSubStringEnd = 0;

                while (TRUE)
                {
                    nSubStringStart = m_strInnerText.Find(L"%str", nSubStringEnd);
                    if (-1 == nSubStringStart)
                        break;

                    int nID = ::StrToInt((LPCTSTR)m_strInnerText + nSubStringStart + 4);
                    if (0 >= nID)
                        break;

                    nSubStringEnd = m_strInnerText.Find(L'%', nSubStringStart + 4) + 1;
                    if (-1 == nSubStringEnd)
                        break;

                    m_strInnerText = m_strInnerText.Left(nSubStringStart)
                        + BkString::Get((UINT)nID)
                        + m_strInnerText.Mid(nSubStringEnd);
                }
            }
        }

        m_uPositionType = 0;

        __super::Load(pTiXmlElem);

        int nValue = 0;
        CStringA strValue;

        strValue = pTiXmlElem->Attribute("show", &nValue);
        if (!strValue.IsEmpty())
        {
            SetVisible(nValue);
        }

        if (2 == m_dlgpos.nCount || 4 == m_dlgpos.nCount)
            return TRUE;

        strValue = pTiXmlElem->Attribute("width", &nValue);
        if (0 == nValue && "full" == strValue)
        {
            m_rcWindow.right = 0;
            m_uPositionType = (m_uPositionType & ~SizeX_Mask) | SizeX_FitParent;
        }
        else
        {
            if (nValue > 0)
            {
                m_rcWindow.right = nValue;
                m_lSpecifyWidth = nValue;
                m_uPositionType = (m_uPositionType & ~SizeX_Mask) | SizeX_Specify;
            }
            else
            {
                m_rcWindow.right = 0;
                m_uPositionType = (m_uPositionType & ~SizeX_Mask) | SizeX_FitContent;
            }
        }

        strValue = pTiXmlElem->Attribute("height", &nValue);
        if (0 == nValue && "full" == strValue)
        {
            m_rcWindow.bottom = 0;
            m_uPositionType = (m_uPositionType & ~SizeY_Mask) | SizeY_FitParent;
        }
        else
        {
            if (nValue > 0)
            {
                m_rcWindow.bottom = nValue;
                m_lSpecifyHeight = nValue;
                m_uPositionType = (m_uPositionType & ~SizeY_Mask) | SizeY_Specify;
            }
            else
            {
                m_rcWindow.bottom = 0;
                m_uPositionType = (m_uPositionType & ~SizeY_Mask) | SizeY_FitContent;
            }
        }

        if (m_uPositionType & (SizeX_FitContent | SizeY_FitContent))
        {
            SIZE sizeRet = {0, 0};

            BkSendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)&sizeRet);

            if (m_uPositionType & SizeX_FitContent)
                m_rcWindow.right = sizeRet.cx;
            if (m_uPositionType & SizeY_FitContent)
                m_rcWindow.bottom = sizeRet.cy;
        }

        nValue = 0;
        pTiXmlElem->Attribute("float", &nValue);
        m_uPositionType |= (nValue != 0 ? Position_Float : Position_Relative);

        return TRUE;
    }

    // Set container, container is a REAL window
    virtual void SetContainer(HWND hWndContainer)
    {
        m_hWndContainer = hWndContainer;
    }

    // Get handle from point
    virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
    {
        if (m_rcWindow.PtInRect(ptHitTest))
            return m_hBkWnd;
        else
            return NULL;
    }

    virtual BOOL NeedRedrawWhenStateChange()
    {
        if (!GetStyle().m_strSkinName.IsEmpty())
        {
            CBkSkinBase* pSkin = BkSkin::GetSkin(GetStyle().m_strSkinName);
            if (pSkin && !pSkin->IgnoreState())
                return TRUE;
        }

        return (CLR_INVALID != GetStyle().m_crHoverText) || (NULL != GetStyle().m_ftHover) || (CLR_INVALID != GetStyle().m_crBgHover);
    }

//     virtual BOOL NeedRedraw()
//     {
//         return m_bNeedRedraw;
//     }

    virtual BOOL IsContainer()
    {
        return FALSE;
    }

    virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
    {
        if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
        {
            DrawBkgnd(dc);

			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, TRUE) )
			{
				return TRUE;
			}

			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);

			if ( m_pPaintHook != NULL )
			{
				m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, FALSE);
			}
	
            return TRUE;
        }

        return FALSE;
    }

    void NotifyInvalidate()
    {
        NotifyInvalidateRect(m_rcWindow);
    }

    void NotifyInvalidateRect(LPRECT lprect)
    {
        if (lprect)
        {
            CRect rect = *lprect;
            NotifyInvalidateRect(rect);
        }
    }

    void NotifyInvalidateRect(const CRect& rect)
    {
        if (IsVisible(TRUE) && m_hWndContainer && ::IsWindow(m_hWndContainer))
        {
            BKINMINVALIDATERECT nms;

            nms.hdr.code        = BKINM_INVALIDATERECT;
            nms.hdr.hwndFrom    = NULL;
            nms.hdr.idFrom      = m_uCmdID;
            nms.hbkWnd          = m_hBkWnd;
            nms.rect            = rect;

            ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
        }
    }

public:

    //////////////////////////////////////////////////////////////////////////
    // Message Handler
    
    void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        m_rcWindow.MoveToXY(lpWndPos->x, lpWndPos->y);
        SIZE sizeRet = {lpWndPos->cx, lpWndPos->cy};

        BkSendMessage(WM_NCCALCSIZE, TRUE, (LPARAM)&sizeRet);

        m_rcWindow.SetRect(lpWndPos->x, lpWndPos->y, lpWndPos->x + sizeRet.cx, lpWndPos->y + sizeRet.cy);

        if (GetPositionType() & SizeX_FitParent)
            m_rcWindow.right = max(lpWndPos->x + lpWndPos->cx, m_rcWindow.left);
        else if (GetPositionType() & SizeX_Specify)
            m_rcWindow.right = m_rcWindow.left + m_lSpecifyWidth;

        if (GetPositionType() & SizeY_FitParent)
            m_rcWindow.bottom = max(lpWndPos->y + lpWndPos->cy, m_rcWindow.top);
        else if (GetPositionType() & SizeY_Specify)
            m_rcWindow.bottom = m_rcWindow.top + m_lSpecifyHeight;
    }

    int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
    {
        return TRUE;
    }

    void OnDestroy()
    {
    }

    // Draw background default
    virtual void DrawBkgnd(CDCHandle& dc)
    {
        const BkStyle& theStyle = GetStyle();
        if (theStyle.m_strSkinName.IsEmpty())
        {
            COLORREF crBg = (CLR_INVALID != m_crBg) ? m_crBg : theStyle.m_crBg;

            if (BkWndState_Hover == (GetState() & BkWndState_Hover) && CLR_INVALID != theStyle.m_crBgHover)
            {
                crBg = theStyle.m_crBgHover;
            }

			if (CLR_INVALID != crBg)
			{
				if (theStyle.m_bCrBgGdiPlus)
				{
					Gdiplus::Graphics		graphics(dc);
					Gdiplus::SolidBrush		brush(Gdiplus::Color(theStyle.m_nAlphaBg, GetRValue(m_crBg), GetGValue(m_crBg), GetBValue(m_crBg)));

					graphics.FillRectangle(&brush, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
				}
				else
				{
					dc.FillSolidRect(m_rcWindow, crBg);
				}
			}
        }
        else
        {
            CBkSkinBase* pSkin = BkSkin::GetSkin(theStyle.m_strSkinName);
            if (pSkin)
            {
                pSkin->Draw(dc, m_rcWindow, m_dwState);
            }
        }
    }

    struct BkDCPaint 
    {
        BkDCPaint()
            : bBkModeChanged(FALSE)
            , nOldBkMode(OPAQUE)
            , bFontChanged(FALSE)
            , hftOld(NULL)
            , bTextColorChanged(FALSE)
            , crOld(CLR_INVALID)
            , bBgColorChanged(FALSE)
            , crOldBg(CLR_INVALID)
        {
        }
        BOOL     bBkModeChanged;
        int      nOldBkMode;
        BOOL     bFontChanged;
        HFONT    hftOld;
        BOOL     bTextColorChanged;
        COLORREF crOld;
        BOOL     bBgColorChanged;
        COLORREF crOldBg;
        CRect    rcClient;
    };

    void BeforePaint(CDCHandle &dc, BkDCPaint &BkDC)
    {
        HFONT /*hftOld = NULL, */hftDraw = NULL;
        COLORREF /*crOld = CLR_INVALID, crOldBg = CLR_INVALID, */crDraw = CLR_INVALID;
        /*int nOldBkMode = OPAQUE;*/
        const BkStyle& theStyle = GetStyle();

        BkDC.rcClient = m_rcWindow;
        BkDC.rcClient.DeflateRect(theStyle.m_nMarginX, theStyle.m_nMarginY);


        if (!theStyle.m_strSkinName.IsEmpty())
        {
            BkDC.bBkModeChanged = TRUE;
            BkDC.nOldBkMode = dc.SetBkMode(TRANSPARENT);
        }
        if (CLR_INVALID != m_crBg)
        {
            BkDC.bBgColorChanged = TRUE;
            BkDC.crOldBg = dc.SetBkColor(m_crBg);
        }
        else if (CLR_INVALID != theStyle.m_crBg)
        {
            BkDC.bBgColorChanged = TRUE;
            BkDC.crOldBg = dc.SetBkColor(theStyle.m_crBg);
        }

        if (m_ftText)
            hftDraw = m_ftText;
        else if (theStyle.m_ftText)
            hftDraw = theStyle.m_ftText;
		else
			hftDraw = BkFontPool::GetFont(BKF_DEFAULTFONT);

        if (m_crText != CLR_INVALID)
            crDraw = m_crText;
        else if (theStyle.m_crText != CLR_INVALID)
            crDraw = theStyle.m_crText;

        if (IsDisabled(TRUE)/*BkWndState_Disable == (GetState() & BkWndState_Disable)*/)
        {
            if (theStyle.m_crDisabledText != CLR_INVALID)
                crDraw = theStyle.m_crDisabledText;
        }
        else if (BkWndState_Hover == (GetState() & BkWndState_Hover))
        {
            if (theStyle.m_ftHover)
                hftDraw = theStyle.m_ftHover;

            if (theStyle.m_crHoverText != CLR_INVALID)
                crDraw = theStyle.m_crHoverText;
        }

        if (hftDraw)
        {
            BkDC.bFontChanged = TRUE;
            BkDC.hftOld = dc.SelectFont(hftDraw);
        }

        if (crDraw != CLR_INVALID)
        {
            BkDC.bTextColorChanged = TRUE;
            BkDC.crOld = dc.SetTextColor(crDraw);
        }
    }

    void AfterPaint(CDCHandle &dc, BkDCPaint &BkDC)
    {
        if (BkDC.bFontChanged)
            dc.SelectFont(BkDC.hftOld);

        if (BkDC.bTextColorChanged)
            dc.SetTextColor(BkDC.crOld);

        if (BkDC.bBkModeChanged)
            dc.SetBkMode(BkDC.nOldBkMode);

        if (BkDC.bBgColorChanged)
            dc.SetBkColor(BkDC.crOldBg);
    }

    // Draw inner text default
    void OnPaint(CDCHandle dc)
    {
		if (m_strInnerText.IsEmpty())
			return;
			
		BkDCPaint BkDC;
        BeforePaint(dc, BkDC);


		if (0 == m_nTextAlign) 
			m_nTextAlign = GetStyle().m_nTextAlign;

		if (BkFontPool::IsYaHei() && TRUE != GetStyle().m_bDisableYaheiResize)
		{
			int nAddSize = BkFontPool::GetFontSizeAdding(dc.GetCurrentFont());

			if ((m_nTextAlign & DT_BOTTOM) == DT_BOTTOM)
			{
				int nTop = BkDC.rcClient.top;
				BkDC.rcClient.top -= 2 * (nAddSize + 1); 
				BkDC.rcClient.bottom -= 2 * (nAddSize + 1);
				int nMinHeight = BkFontPool::GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
				if (BkDC.rcClient.Height() < nMinHeight)
					BkDC.rcClient.bottom += nMinHeight - BkDC.rcClient.Height();


				if (BkDC.rcClient.bottom - 3 - (BkFontPool::GetDefaultFontSize() + nAddSize) < nTop)
					BkDC.rcClient.bottom = nTop + BkFontPool::GetDefaultFontSize() + nAddSize + 3;
			}
			else if ((m_nTextAlign & DT_VCENTER) == DT_VCENTER)
			{
				BkDC.rcClient.top -= 0; 
				BkDC.rcClient.bottom -= 0;
			}
			else
			{
				BkDC.rcClient.top -= 3 * (nAddSize + 1); 
				BkDC.rcClient.bottom -= 3 * (nAddSize + 1);
			}
		}

		 const BkStyle& theStyle = GetStyle();

		if (theStyle.m_nGdiplus)
		{
			Gdiplus::Graphics graphics( dc );
			CString strFaceName;
			LONG lSize;
			BkFontPool::GetFontInfo( strFaceName, lSize );

			LOGFONT lf = {0};
			GetObject(dc.GetCurrentFont(), sizeof (LOGFONT), &lf);

			Gdiplus::Font myFont(dc, &lf);
			Gdiplus::SolidBrush blackBrush(Gdiplus::Color(GetTextAlpha(), GetRValue(dc.GetTextColor()), GetGValue(dc.GetTextColor()), GetBValue(dc.GetTextColor())));

			Gdiplus::StringFormat format;
			if ((m_nTextAlign & DT_CENTER) == DT_CENTER)
				format.SetAlignment(Gdiplus::StringAlignmentCenter);

			if ((m_nTextAlign & DT_VCENTER) == DT_VCENTER)
				format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		/*	if ((m_nTextAlign & DT_SINGLELINE) == DT_SINGLELINE)
				format.SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);*/
			
			if ((m_nTextAlign & DT_END_ELLIPSIS) == DT_END_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			if ((m_nTextAlign & DT_WORD_ELLIPSIS) == DT_WORD_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
			if ((m_nTextAlign & DT_PATH_ELLIPSIS) == DT_PATH_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);

			Gdiplus::RectF layoutRect(BkDC.rcClient.left, BkDC.rcClient.top, BkDC.rcClient.Width(), BkDC.rcClient.Height());

			int nTxtRndHit = theStyle.m_nTxtRndHit;
			graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint)nTxtRndHit);

			graphics.DrawString(
				m_strInnerText,
				-1,
				&myFont,
				layoutRect,
				&format,
				&blackBrush);

		
		}
		else
		{
			int nRet = 0;
			if (GetStyle().m_nShadow != 0)
			{
				nRet = dc.DrawShadowText(
					m_strInnerText, 
					m_strInnerText.GetLength(), 
					BkDC.rcClient, 
					m_nTextAlign, 
					dc.GetTextColor(), 
					GetStyle().m_crShadow, 
					2, 
					2);
			}

			if (0 == nRet)
				dc.DrawText(
				m_strInnerText, m_strInnerText.GetLength(), 
				BkDC.rcClient, 
				m_nTextAlign);
		}


        AfterPaint(dc, BkDC);
    }

    // Calc BkWindow size
    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        LPSIZE pSize = (LPSIZE)lParam;

        if (4 == m_dlgpos.nCount)
        {
            CRect rect;

            if (m_dlgpos.Left.bCenter)
                rect.left = pSize->cx / 2 + (m_dlgpos.Left.bMinus ? -1 : 1) *  m_dlgpos.Left.nPos;
            else if (m_dlgpos.Left.bMinus)
                rect.left = pSize->cx - m_dlgpos.Left.nPos;
            else
                rect.left = m_dlgpos.Left.nPos;

            if (m_dlgpos.Right.bCenter)
                rect.right = pSize->cx / 2 + (m_dlgpos.Right.bMinus ? -1 : 1) *  m_dlgpos.Right.nPos;
            else if (m_dlgpos.Right.bMinus)
                rect.right = pSize->cx - m_dlgpos.Right.nPos;
            else
                rect.right = m_dlgpos.Right.nPos;

            if (m_dlgpos.Top.bCenter)
                rect.top = pSize->cy / 2 + (m_dlgpos.Top.bMinus ? -1 : 1) *  m_dlgpos.Top.nPos;
            else if (m_dlgpos.Top.bMinus)
                rect.top = pSize->cy - m_dlgpos.Top.nPos;
            else
                rect.top = m_dlgpos.Top.nPos;

            if (m_dlgpos.Bottom.bCenter)
                rect.bottom = pSize->cy / 2 + (m_dlgpos.Bottom.bMinus ? -1 : 1) *  m_dlgpos.Bottom.nPos;
            else if (m_dlgpos.Bottom.bMinus)
                rect.bottom = pSize->cy - m_dlgpos.Bottom.nPos;
            else
                rect.bottom = m_dlgpos.Bottom.nPos;

            *pSize = rect.Size();
        }
        else
        {
            CDCHandle dcDesktop = ::GetDC(::GetDesktopWindow());
            CDC dcTest;
            HFONT hftOld = NULL, hftTest = NULL;
            CRect rcTest = m_rcWindow;
            const BkStyle& theStyle = GetStyle();

            dcTest.CreateCompatibleDC(dcDesktop);

            if (bCalcValidRects)
            {
                rcTest.right = rcTest.left + pSize->cx;
                rcTest.bottom = rcTest.top + pSize->cy;
            }

            if (m_ftText)
                hftTest = m_ftText;
            else if (theStyle.m_ftText)
                hftTest = theStyle.m_ftText;
            else
            {
                CBkWindow* pWnd = this;
                HBKWND hWndParent = NULL;

                while (pWnd && NULL != (hWndParent = pWnd->GetParent()))
                {
                    pWnd = BkWnds::GetWindow(hWndParent);
                    if (pWnd && pWnd->m_ftText)
                    {
                        hftTest = pWnd->m_ftText;
                        break;
                    }
                    else if (pWnd && pWnd->GetStyle().m_ftText)
                    {
                        hftTest = pWnd->GetStyle().m_ftText;
                        break;
                    }
                }

            }

            if (BkWndState_Hover == (GetState() & BkWndState_Hover))
            {
                if (theStyle.m_ftHover)
                    hftTest = theStyle.m_ftHover;
                else
                {
                    CBkWindow* pWnd = this;
                    HBKWND hWndParent = NULL;

                    while (pWnd && NULL != (hWndParent = pWnd->GetParent()))
                    {
                        pWnd = BkWnds::GetWindow(hWndParent);
                        if (pWnd && pWnd->GetStyle().m_ftHover)
                        {
                            hftTest = pWnd->GetStyle().m_ftHover;
                            break;
                        }
                    }
                }
            }

            if (NULL == hftTest)
                hftTest = BkFontPool::GetFont(BKF_DEFAULTFONT);

            hftOld = dcTest.SelectFont(hftTest);

            rcTest.DeflateRect(theStyle.m_nMarginX, theStyle.m_nMarginY);

            int nTestDrawMode = theStyle.m_nTextAlign & ~(DT_CENTER | DT_RIGHT | DT_VCENTER | DT_BOTTOM);

            if (nTestDrawMode & DT_WORDBREAK)
                rcTest.bottom = 0x7FFF;

			if (0 != m_strInnerText.GetLength())
			{
				if (theStyle.m_bGdiplusCalcTxt)
				{
					Gdiplus::StringFormat format;
					Gdiplus::Graphics graphics(dcTest);

					LONG	lSize;
					CString strFaceName;
					LOGFONT lf = {0};

					BkFontPool::GetFontInfo(strFaceName, lSize );
					GetObject(hftTest, sizeof (LOGFONT), &lf);

					Gdiplus::Font font(dcTest,  &lf);
					Gdiplus::SizeF layoutSize(rcTest.Width(), rcTest.Height());

					CBkWindow* pParentWnd = BkWnds::GetWindow(GetParent());
					if (pParentWnd)
					{
						CRect rcParentWnd;

						pParentWnd->GetRect(rcParentWnd);

						layoutSize.Width = rcParentWnd.Width();
						layoutSize.Height = rcParentWnd.Height();
					}

					format.SetAlignment(Gdiplus::StringAlignmentNear);

					Gdiplus::SizeF stringSize;
					graphics.MeasureString(m_strInnerText, m_strInnerText.GetLength(), &font, layoutSize, &format, &stringSize);

					if (stringSize.Width - (int(stringSize.Width)) > 0)
					{
						stringSize.Width += 1;
					}

					if (stringSize.Height - (int(stringSize.Height)) > 0)
					{
						stringSize.Height += 1;
					}

					rcTest.right = rcTest.left + stringSize.Width;
					rcTest.bottom = rcTest.top + stringSize.Height;
				}
				else
				{
					dcTest.DrawText(m_strInnerText, m_strInnerText.GetLength(), rcTest, nTestDrawMode | DT_CALCRECT);
				}
			}
			else
			{
				rcTest.right = rcTest.left;
				rcTest.bottom = rcTest.top;
			}

            pSize->cx = rcTest.Width() + 2 * theStyle.m_nMarginX;
            pSize->cy = rcTest.Height() + 2 * theStyle.m_nMarginY;

            dcTest.SelectFont(hftOld);

            ::ReleaseDC(::GetDesktopWindow(), dcDesktop);
        }

        return 0;
    }

    void OnShowWindow(BOOL bShow, UINT nStatus)
    {
        if (bShow)
            ModifyState(0, BkWndState_Invisible);
        else
            ModifyState(BkWndState_Invisible, 0);
    }

    HRESULT OnAttributePosChange(CStringA& strValue, BOOL bLoading)
    {
        if (!strValue.IsEmpty())
        {
            LPCSTR lpszValue = strValue;
            int nPos = 0, nPosPrev = 0;

            for (m_dlgpos.nCount = 0; m_dlgpos.nCount < 4; m_dlgpos.nCount ++)
            {
                nPosPrev = nPos;
                nPos = strValue.Find(',', nPosPrev) + 1;

                m_dlgpos.Item[m_dlgpos.nCount].bCenter = ('|' == lpszValue[nPosPrev]);
                if (m_dlgpos.Item[m_dlgpos.nCount].bCenter)
                    nPosPrev ++;
                m_dlgpos.Item[m_dlgpos.nCount].bMinus = ('-' == lpszValue[nPosPrev]);
                if (m_dlgpos.Item[m_dlgpos.nCount].bMinus)
                    nPosPrev ++;

                if (0 == nPos)
                {
                    m_dlgpos.Item[m_dlgpos.nCount].nPos = ::StrToIntA(strValue.Mid(nPosPrev));
                    m_dlgpos.nCount ++;
                    break;
                }

                m_dlgpos.Item[m_dlgpos.nCount].nPos = ::StrToIntA(strValue.Mid(nPosPrev, nPos - nPosPrev));
            }

            if (2 == m_dlgpos.nCount || 4 == m_dlgpos.nCount)
            {
                m_uPositionType = (m_uPositionType & ~SizeX_Mask) | SizeX_FitContent;
                m_uPositionType = (m_uPositionType & ~SizeY_Mask) | SizeY_FitContent;
            }
            else
                m_dlgpos.nCount = 0;

            if (!bLoading)
            {
                _RepositionSelf();
            }
        }

        return S_FALSE;
    }

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		// 传出MouseMove消息。LoveHM By ZC. 2010-11-18.
		if (m_nNotifyMsg & 0x01)
		{
			BKNMNOTIFY nms;
			nms.hdr.code = BKNM_NOTIFY;
			nms.hdr.hwndFrom = m_hWndContainer;
			nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);;
			nms.uItemID = GetCmdID();
			nms.szItemClass = "wnd";
			nms.uNotifyMsg = BKNM_MOUSEHOVER;

			LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}

		if( m_pMouseHook )
		{
			m_pMouseHook->NotifyMouseEvent(WM_MOUSEMOVE, this, m_rcWindow, point );
		}
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if( m_pMouseHook )
		{
			m_pMouseHook->NotifyMouseEvent(WM_LBUTTONDOWN, this, m_rcWindow, point );
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if( m_pMouseHook )
		{
			m_pMouseHook->NotifyMouseEvent(WM_LBUTTONUP, this, m_rcWindow, point );
		}
	}

	void OnMouseLeave()
	{
		// 传出MouseLeave消息。LoveHM By ZC. 2010-11-18.
		if (m_nNotifyMsg & 0x02)
		{
			BKNMNOTIFY nms;
			nms.hdr.code = BKNM_NOTIFY;
			nms.hdr.hwndFrom = m_hWndContainer;
			nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);;
			nms.uItemID = GetCmdID();
			nms.szItemClass = "wnd";
			nms.uNotifyMsg = BKNM_MOUSELEAVE;

			LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}

		if( m_pMouseHook )
		{
			CPoint point;
			point.x = point.y = 0;
			m_pMouseHook->NotifyMouseEvent(WM_MOUSELEAVE, this, m_rcWindow, point );
		}
	}

    void _RepositionSelf();

	BYTE GetTextAlpha()
	{
		BYTE byAlphaTxt = 0;

		if (m_nAlphaTxt != -1)
		{
			byAlphaTxt = m_nAlphaTxt;
		}
		else
		{
			byAlphaTxt = GetStyle().m_nAlphaTxt;
		}

		if (byAlphaTxt == 255)
			byAlphaTxt = 254;

		return byAlphaTxt;
	}

protected:
    BOOL m_bMsgHandled;

    BOOL IsMsgHandled() const
    {
        return m_bMsgHandled;
    }
    void SetMsgHandled(BOOL bHandled)
    {
        m_bMsgHandled = bHandled;
    }

    BKWIN_DECLARE_RETURN_SETTING(FALSE, FALSE)

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_CREATE(OnCreate)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
        MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
    BKWIN_END_MSG_MAP_BASE()
// 
//     enum {
//         BKWND_INNERTEXT = 1, 
//     };
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("class", m_strStyleName, TRUE)
        BKWIN_INT_ATTRIBUTE("id", m_uCmdID, FALSE)//, 0)
		BKWIN_INT_ATTRIBUTE("alphatxt", m_nAlphaTxt, TRUE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crtext", m_crText, FALSE)
        BKWIN_FONT_ATTRIBUTE("font", m_ftText, FALSE)
        BKWIN_TSTRING_ATTRIBUTE("href", m_strLinkUrl, FALSE)
        BKWIN_TSTRING_ATTRIBUTE("tip", m_strToolTipText, FALSE)
        BKWIN_ENUM_ATTRIBUTE("valign", UINT, TRUE)
            BKWIN_ENUM_VALUE("top", VAlign_Top)
            BKWIN_ENUM_VALUE("middle", VAlign_Middle)
            BKWIN_ENUM_VALUE("bottom", VAlign_Bottom)
        BKWIN_ENUM_END(m_uVAlign)
        BKWIN_ENUM_ATTRIBUTE("align", UINT, TRUE)
            BKWIN_ENUM_VALUE("left", Align_Left)
            BKWIN_ENUM_VALUE("center", Align_Center)
            BKWIN_ENUM_VALUE("right", Align_Right)
        BKWIN_ENUM_END(m_uHAlign)
        BKWIN_CUSTOM_ATTRIBUTE("pos", OnAttributePosChange)
        BKWIN_INT_ATTRIBUTE("transparent", m_bTransparent, FALSE)//, 0)
		BKWIN_INT_ATTRIBUTE("headacceptlbtndownmsg", m_bHeadAcceptLBtnDownMsg, FALSE)//, 0)
		BKWIN_UINT_ATTRIBUTE("notify", m_nNotifyMsg, 0)
    BKWIN_DECLARE_ATTRIBUTES_END()
protected:
	int m_nNotifyMsg;
};

// 已废弃
inline CBkWindow* BkWnds::GetWindow(UINT uCmdID, HWND hWndInContainer/* = NULL*/)
{
	CBkWindow* pWndRet = NULL;

	::EnterCriticalSection(&ms_lockWndMap);

    POSITION pos = _Instance()->m_mapPool.GetStartPosition();

    while (pos)
    {
        const _BkWndHandlePool::CPair *pairRet = _Instance()->m_mapPool.GetNext(pos);

        if (pairRet->m_value->GetCmdID() == uCmdID)
        {
            if (NULL == hWndInContainer || hWndInContainer == pairRet->m_value->GetContainer())
			{
				pWndRet = pairRet->m_value;
				break;
			}
        }
    }

	::LeaveCriticalSection(&ms_lockWndMap);

    return pWndRet;
}


inline void BkWnds::SelectRadio(CBkWindow* pbkWnd, LPCSTR lpszGroupName, CBkWindow **ppbkWndLastSelect)
{
	::EnterCriticalSection(&ms_lockWndMap);

    CAtlList<CBkWindow *> &ListGroup = _Instance()->m_mapRadioPool[lpszGroupName];

    pbkWnd->ModifyState(BkWndState_Check, 0);

    POSITION posCurrent = NULL, pos = ListGroup.GetHeadPosition();

    while (pos)
    {
        posCurrent = pos;
        CBkWindow *pbkWndFind = ListGroup.GetNext(pos);

        if (pbkWndFind == pbkWnd)
            continue;

        if (pbkWndFind->IsChecked())
        {
            pbkWndFind->ModifyState(0, BkWndState_Check);
            pbkWndFind->NotifyInvalidate();

            if (ppbkWndLastSelect)
                *ppbkWndLastSelect = pbkWndFind;
        }
    }

	::LeaveCriticalSection(&ms_lockWndMap);
}

inline bool BkWnds::IsSelectedRadio(LPCSTR lpszGroupName, CBkWindow **ppbkWndLastSelect)
{
	::EnterCriticalSection(&ms_lockWndMap);

	CAtlList<CBkWindow *> &ListGroup = _Instance()->m_mapRadioPool[lpszGroupName];


	POSITION posCurrent = NULL, pos = ListGroup.GetHeadPosition();
	bool bSelect = false;
	while (pos)
	{
		posCurrent = pos;
		CBkWindow *pbkWndFind = ListGroup.GetNext(pos);


		if (pbkWndFind->IsChecked())
		{
			bSelect = true;
			if (ppbkWndLastSelect)
				*ppbkWndLastSelect = pbkWndFind;
			break;
		}
	}

	::LeaveCriticalSection(&ms_lockWndMap);

	return bSelect;
}

class CBkContainerWnd : public CBkWindow
{
public:
    virtual CBkWindow* FindChildByCmdID(UINT uCmdID)
    {
        ATLASSERT(FALSE);

        return NULL;
    }

    virtual void RepositionChilds()
    {
    }

    virtual void RepositionChild(CBkWindow *pBkWndChild)
    {
    }

    BOOL IsContainer()
    {
        return TRUE;
    }
};

inline void CBkWindow::_RepositionSelf()
{
    CBkWindow *pWndParent = BkWnds::GetWindow(GetParent());
    if (pWndParent && pWndParent->IsContainer())
    {
        CBkContainerWnd *pWndParentContainer = (CBkContainerWnd *)pWndParent;
        pWndParentContainer->RepositionChild(this);
    }
}
