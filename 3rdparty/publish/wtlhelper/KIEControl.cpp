#include "KIEControl.h"

//////////////////////////////////////////////////////////////////////////
//Dispatcher
ULONG KDispatcherBase::AddRef() 
{ 
	return m_nRef; 
}

ULONG KDispatcherBase::Release() 
{
	return m_nRef;	
}

HRESULT KDispatcherBase::QueryInterface(REFIID iid, void ** ppvObject)
{
	HRESULT hr = E_NOINTERFACE;

	if( iid == __uuidof(IUnknown) || iid == __uuidof(IDispatch) )
	{
		*ppvObject = (IDispatch*)this;
		hr = S_OK;
	}

	return hr;
}

HRESULT KDispatcherBase::GetIDsOfNames(REFIID, LPOLESTR *rgszNames, UINT, LCID, DISPID *rgDispId)
{
	if( lstrcmp(rgszNames[0], _T("CallWrapper")) == 0 )
	{
		*rgDispId = 0;
	}

	return S_OK;
}

HRESULT KDispatcherBase::Invoke(DISPID dispIdMember, REFIID, LCID, WORD, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO*, UINT*)
{
	if( 0 != dispIdMember || 2 != pDispParams->cArgs )
		return E_NOTIMPL;

	if( VT_BSTR != pDispParams->rgvarg[0].vt || VT_BSTR != pDispParams->rgvarg[1].vt || pDispParams->rgvarg[0].bstrVal == NULL || pDispParams->rgvarg[1].bstrVal == NULL )
		return E_INVALIDARG;

	BSTR bstrResult = NULL;
	MethodCall(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal, &bstrResult);

	if( pVarResult )
	{
		if( bstrResult == NULL )
			bstrResult = ::SysAllocString( _T("") );

		pVarResult->vt = VT_BSTR;
		pVarResult->bstrVal = bstrResult;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//IEControl
KIEControl::KIEControl( KDispatcherBase *pDispatcher, IIEControlNotify *pNotify, LPCTSTR lpszLoadPage, LPCTSTR lpszErrorPage ) 
	: m_pDispatcher( pDispatcher )
	, m_pNotify( pNotify )
	, m_pKsClientSite(NULL)
{
	CString strModulePath;
	::GetModuleFileName( ::GetModuleHandle(NULL), strModulePath.GetBuffer(MAX_PATH + 1), MAX_PATH);
	strModulePath.ReleaseBuffer();

	if( lpszLoadPage )
		m_strLoadPage.Format(L"res://%s/%s", strModulePath, lpszLoadPage);

	if( lpszErrorPage )
		m_strErrorPage.Format(L"res://%s/%s", strModulePath, lpszErrorPage);

    flag_ = S_OK;
}

KIEControl::~KIEControl()
{
	if( m_pKsClientSite )
		m_pKsClientSite->Release();
}

HWND KIEControl::Create( HWND hWndParent, LPRECT lpRect, UINT nID, BOOL bVisible )
{
	HWND hWnd = __super::Create(hWndParent, lpRect, NULL, WS_CHILD | WS_CLIPCHILDREN | (bVisible ? WS_VISIBLE : 0), 0, nID);
	if (!hWnd)
		return NULL;

	CreateIEControl( hWnd );

	return hWnd;
}

void KIEControl::CreateIEControl( HWND hWnd )
{
	RECT rcClient;
	::GetClientRect( hWnd, &rcClient );

	m_wndIE.Create(hWnd, rcClient, NULL, WS_CHILD | WS_CLIPCHILDREN );
	m_wndIE.CreateControl(_T("Shell.Explorer"));
	m_wndIE.SetDlgCtrlID(IDYES);
	m_wndIE.SetExternalDispatch(m_pDispatcher);

	AtlAdviseSinkMap(this, TRUE);

	HRESULT hr = E_FAIL;
	CComPtr<IAxWinAmbientDispatch> spHost;

	hr = m_wndIE.QueryHost(&spHost);
	if (SUCCEEDED(hr))
	{
		spHost->put_DocHostFlags(DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_SCROLL_NO);
	}

	CComPtr<IWebBrowser2> piWebBrowser;
	m_wndIE.QueryControl(&piWebBrowser);
	if (piWebBrowser)
	{
		piWebBrowser->put_RegisterAsDropTarget( VARIANT_FALSE );
		piWebBrowser->put_Silent(VARIANT_FALSE);

		m_pKsClientSite = new ksOleClientSite;
		if( m_pKsClientSite )
		{
			m_pKsClientSite->AddRef();
			m_pKsClientSite->Init(piWebBrowser);
            m_pKsClientSite->SetFlag(flag_);
		}
	}
}

void KIEControl::Navigate( LPCTSTR lpszUrl, BOOL bShowLoadingPage )
{
	if( bShowLoadingPage && !m_strLoadPage.IsEmpty() )
	{
		INT nLength = m_strLoadPage.GetLength();
		TCHAR *pszUrl = new TCHAR[ nLength + 1 ];
		if( pszUrl )
		{
			::memset( pszUrl, 0, (nLength + 1)*sizeof(TCHAR) );
			_tcsncpy( pszUrl, (LPCTSTR)m_strLoadPage, nLength );
			PostMessage( WM_NAVIGATE, (WPARAM)pszUrl, 0 );
		}
	}

	if( lpszUrl && _tcslen(lpszUrl) != 0 )
	{
		INT nLength = _tcslen(lpszUrl);
		TCHAR *pszUrl = new TCHAR[ nLength + 1 ];
		if( pszUrl )
		{
			::memset( pszUrl, 0, (nLength + 1)*sizeof(TCHAR) );
			_tcsncpy( pszUrl, lpszUrl, nLength );
			PostMessage( WM_NAVIGATE, (WPARAM)pszUrl, 0 );
		}
	}
}

void KIEControl::InvokeMethod( LPOLESTR lpszFunction, LPCTSTR lpszParam, BSTR * bstrResult )
{
	if( !lpszFunction )
		return;

	CComPtr<IWebBrowser2> pWebBrowser;
	CComPtr<IHTMLDocument> pDocument;
	CComPtr<IDispatch> pScript;
	HRESULT hRet = E_FAIL;
	DISPID dispid;

	hRet = m_wndIE.QueryControl(&pWebBrowser);
	if (FAILED(hRet) || !pWebBrowser)
		return;

	hRet = pWebBrowser->get_Document((IDispatch **)&pDocument);
	if (FAILED(hRet) || !pDocument)
		return;

	hRet = pDocument->get_Script((IDispatch **)&pScript);
	if (FAILED(hRet) || !pScript)
		return;

	hRet = pScript->GetIDsOfNames( IID_NULL, &lpszFunction, 1, LOCALE_SYSTEM_DEFAULT, &dispid ); 
	if (FAILED(hRet))
		return;

	VARIANT vRet = {VT_EMPTY};
	VARIANTARG vArg = {VT_BSTR, NULL};
	DISPPARAMS param = {&vArg, NULL, 1, 0};
	vArg.bstrVal = ::SysAllocString( lpszParam );
	hRet = pScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, &param, &vRet, NULL, NULL);
	if (FAILED(hRet))
		return;

	if (vArg.vt == VT_BSTR && vArg.bstrVal)
		::SysFreeString(vArg.bstrVal);

	if (vRet.vt == VT_BSTR && vRet.bstrVal)
	{
		if( bstrResult )
			*bstrResult = ::SysAllocString( vRet.bstrVal );

		::SysFreeString(vRet.bstrVal);
	}
}

void KIEControl::ExecuteMethod( LPCTSTR lpszScrpit, BSTR * bstrResult )
{
	if( !lpszScrpit || _tcslen(lpszScrpit) == 0 )
		return;

	CComPtr<IWebBrowser2> pWebBrowser;
	HRESULT hRet = m_wndIE.QueryControl(&pWebBrowser);
	if( FAILED(hRet) || !pWebBrowser )
		return;

	CComPtr<IDispatch> dispDoc;
	hRet = pWebBrowser->get_Document(&dispDoc);
	if( FAILED(hRet) || !dispDoc )
		return;

	CComQIPtr<IHTMLDocument2> pDocument(dispDoc);
	if( !pDocument )
		return;

	CComPtr<IHTMLWindow2> pWindow;
	hRet = pDocument->get_parentWindow(&pWindow);
	if( FAILED(hRet) || !pWindow )
		return;

	CComBSTR bstrScript = lpszScrpit;
	VARIANT vRet = {VT_EMPTY};
	hRet = pWindow->execScript(bstrScript, CComBSTR(_T("javascript")), &vRet);
	if( FAILED(hRet) )
		return;

	if (vRet.vt == VT_BSTR && vRet.bstrVal)
	{
		if( bstrResult )
			*bstrResult = ::SysAllocString( vRet.bstrVal );

		::SysFreeString(vRet.bstrVal);
	}
}

BOOL KIEControl::GetWebBrowser( IWebBrowser2 ** pIWebBrowser )
{
	if( !pIWebBrowser )
		return FALSE;

	CComPtr<IWebBrowser2> piWebBrowser;
	HRESULT hr = m_wndIE.QueryControl(&piWebBrowser);
	if( FAILED(hr) )
		return FALSE;

	*pIWebBrowser = piWebBrowser;
	(*pIWebBrowser)->AddRef();

	return TRUE;
}

void KIEControl::OnNavigateComplete( IDispatch* pDisp, VARIANT* URL )
{
	m_wndIE.ShowWindow( SW_SHOW );

	if( URL->vt == VT_BSTR && URL->bstrVal != NULL )
	{
		if( m_strLoadPage.CompareNoCase( URL->bstrVal ) == 0 ||
			m_strErrorPage.CompareNoCase( URL->bstrVal ) == 0 )
		{
			return;
		}
	}

	if( m_pNotify )
	{
		if( URL->vt == VT_BSTR && URL->bstrVal != NULL )
			m_pNotify->NotifyNavigateComplete( GetDlgCtrlID(), URL->bstrVal );
		else
			m_pNotify->NotifyNavigateComplete( GetDlgCtrlID(), NULL );
	}
}

void KIEControl::OnNavigateError( IDispatch* pDisp, VARIANT* pvtURL, VARIANT* pvtFrame, VARIANT* pvtStatusCode, VARIANT_BOOL* pvbCancel )
{
	if( m_pNotify )
	{
		if( pvtURL->vt == VT_BSTR && pvtURL->bstrVal != NULL )
			m_pNotify->NotifyNavigateError( GetDlgCtrlID(), pvtURL->bstrVal );
		else
			m_pNotify->NotifyNavigateError( GetDlgCtrlID(), NULL );
	}

	if( !m_strErrorPage.IsEmpty() )
		Navigate( m_strErrorPage );
}

void KIEControl::OnDocumentComplete( IDispatch *pDisp, VARIANT* URL )
{
	if( URL->vt == VT_BSTR && URL->bstrVal != NULL )
	{
		if( m_strLoadPage.CompareNoCase( URL->bstrVal ) == 0 ||
			m_strErrorPage.CompareNoCase( URL->bstrVal ) == 0 )
		{
			return;
		}
	}

	if( m_pNotify )
	{
		if( URL->vt == VT_BSTR && URL->bstrVal != NULL )
			m_pNotify->NotifyDocumentComplete( GetDlgCtrlID(), URL->bstrVal );
		else
			m_pNotify->NotifyDocumentComplete( GetDlgCtrlID(), NULL );
	}
}

void KIEControl::OnNewWindow( IDispatch **pDisp, VARIANT_BOOL *pvbCancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	if( m_pNotify && bstrUrl && _tcslen(bstrUrl) != 0 )
	{
		m_pNotify->NotifyOpenUrl( GetDlgCtrlID(), bstrUrl );
		*pvbCancel = VARIANT_TRUE;
	}
}

LRESULT KIEControl::OnDestroy( UINT, WPARAM, LPARAM, BOOL )
{
	m_wndIE.SetExternalDispatch(NULL);

	AtlAdviseSinkMap(this, FALSE);

	if (m_wndIE.IsWindow())
		m_wndIE.DestroyWindow();

	return TRUE;
}

LRESULT KIEControl::OnSize( UINT, WPARAM, LPARAM, BOOL )
{
	if (m_wndIE.IsWindow())
	{
		RECT rcClient;

		::GetClientRect( m_hWnd, &rcClient );

		m_wndIE.MoveWindow(&rcClient);
	}

	return TRUE;
}

LRESULT KIEControl::OnNavigate( UINT, WPARAM wParam, LPARAM, BOOL )
{
	LPCTSTR lpszUrl = (LPCTSTR)wParam;
	if( lpszUrl && _tcslen(lpszUrl) != 0 )
	{
		CComPtr<IWebBrowser2> piWebBrowser;
		m_wndIE.QueryControl(&piWebBrowser);
		if (piWebBrowser)
		{
			CComVariant v, vFlags = navNoHistory;
			piWebBrowser->Navigate(CComBSTR(lpszUrl), &vFlags, &v, &v, &v);
		}
	}

	if( lpszUrl )
		delete[] lpszUrl;

	return TRUE;
}

BOOL KIEControl::PreTranslateMessage( MSG* pMsg )
{
	if( (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		|| (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST))
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_BACK)
		{
			return FALSE;
		}
		if(  m_wndIE.SendMessage( WM_FORWARDMSG, 0, (LPARAM)pMsg ) != 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

static BOOL CALLBACK EnumIExplorerProc( HWND hWnd, LPARAM lParam )
{
    TCHAR szBuf[ 32 ] = { 0 };
    ::GetClassName( hWnd, szBuf, 32 );
    TCHAR *pszName = _T("Internet Explorer_Server");
    if( _wcsnicmp( szBuf, pszName, wcslen( pszName ) ) == 0 )
    {
        HWND * hWndRet = (HWND *)lParam;
        *hWndRet = hWnd;
        return FALSE;
    }

    return TRUE;
}

HWND KIEControl::GetIExplorer()
{
    HWND hWndIExplorer = NULL;
    ::EnumChildWindows( m_hWnd, EnumIExplorerProc, (LPARAM)&hWndIExplorer );
    return hWndIExplorer;
}