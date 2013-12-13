#ifndef _KIECONTROL_H
#define _KIECONTROL_H

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <windows.h>
#include <tchar.h>
#include "3rdparty/wtl/atlapp.h"
#include "ksOleClientSite.h"
#define NAVIGATECOMPLETE			 252
#define NAVIGATEERROR                271
#define DOCUMENTCOMPLETE             259
#define NEWWINDOW					 273

#define WM_NAVIGATE					 WM_USER + 2013

/*
* @brief：实现一个IDispatch接口，外围调用CallWrapper。
* @example：
*     window.external.CallWrapper('function','param=test'); 调用名字是function，传递的参数是param=test
*/
#define BEGIN_JS_EVENT() \
	void MethodCall(LPCTSTR lpszMethod, LPCTSTR lpszParam, BSTR * bstrOut) \
	{ \
		if(0){}\
	
#define JS_EVENT_ITEM(name, method) \
		else if( _tcscmp( lpszMethod, name ) == 0 ) \
		{ \
			method( lpszParam, bstrOut ); \
		} \

#define END_JS_EVENT() \
	} \

class KDispatcherBase : public IDispatch
{
public:
    KDispatcherBase() : m_nRef(0) {}

protected:
    virtual void MethodCall(LPCTSTR lpszMethod, LPCTSTR lpszParam, BSTR * bstrOut){}

private:
	LONG m_nRef;
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT*) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo**) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID, LPOLESTR *, UINT, LCID, DISPID *);
    HRESULT STDMETHODCALLTYPE Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);
};

class IIEControlNotify
{
public:
	virtual void NotifyNavigateComplete( UINT nCtrlID, LPCTSTR lpszUrl ){}
	virtual void NotifyNavigateError( UINT nCtrlID, LPCTSTR lpszUrl ){}
	virtual void NotifyDocumentComplete( UINT nCtrlID, LPCTSTR lpszUrl ){}
	virtual void NotifyOpenUrl( UINT nCtrlID, LPCTSTR lpszUrl ){}
};

/*
* @brief：实现一个IE控件的壳。
* @example：
*	  KIEControl m_IE;
*	  m_IE.InvokeMethod( "jsTest", "param=test", bstrRet ); 调用页面的jsTest函数，传递参数param=test, 返回值放在bstrRet中。
*/
class KIEControl
    : public CWindowImpl<KIEControl>
    , public IDispEventImpl<IDYES, KIEControl>
	, public CMessageFilter
{
public:
    KIEControl( KDispatcherBase *pDispatcher = NULL, IIEControlNotify * pNotify = NULL, LPCTSTR lpszLoadPage = NULL, LPCTSTR lpszErrorPage = NULL );
	~KIEControl();

	HWND Create( HWND hWndParent, LPRECT lpRect, UINT nID, BOOL bVisible = TRUE );
	void Navigate( LPCTSTR lpszUrl, BOOL bShowLoaddingPage = FALSE );
	void InvokeMethod( LPOLESTR lpszFunction, LPCTSTR lpszParam, BSTR * bstrResult );
	void ExecuteMethod( LPCTSTR lpszScrpit, BSTR * bstrResult );
	HWND GetAxWnd(){ return m_wndIE.m_hWnd; }
	BOOL GetWebBrowser( IWebBrowser2 ** pIWebBrowser );
    HWND GetIExplorer();
	
public:
	BEGIN_MSG_MAP(KIEControl)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_NAVIGATE, OnNavigate)
	END_MSG_MAP()

	BEGIN_SINK_MAP(KIEControl)
		SINK_ENTRY(IDYES, NAVIGATECOMPLETE, OnNavigateComplete)
		SINK_ENTRY(IDYES, NAVIGATEERROR, OnNavigateError)
		SINK_ENTRY(IDYES, DOCUMENTCOMPLETE, OnDocumentComplete)
		SINK_ENTRY(IDYES, NEWWINDOW, OnNewWindow)
	END_SINK_MAP()

    void SetFlag(HRESULT flag)
    {
        flag_ = flag;
    }

protected:
	void CreateIEControl( HWND hWnd );
	virtual BOOL PreTranslateMessage( MSG* pMsg );

protected:
    LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL );
    LRESULT OnSize( UINT, WPARAM, LPARAM, BOOL );
	LRESULT OnEraseBkgnd( UINT, WPARAM, LPARAM, BOOL ){ return FALSE; }
	LRESULT OnNavigate( UINT, WPARAM, LPARAM, BOOL );

protected:
	void WINAPI OnNavigateComplete( IDispatch* pDisp, VARIANT* URL );
	void WINAPI OnNavigateError( IDispatch* pDisp, VARIANT* pvtURL, VARIANT* pvtFrame, VARIANT* pvtStatusCode, VARIANT_BOOL* pvbCancel );
	void WINAPI OnDocumentComplete( IDispatch *pDisp, VARIANT* URL );
	void WINAPI OnNewWindow( IDispatch **pDisp, VARIANT_BOOL *pvbCancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl );

protected:
	KDispatcherBase *m_pDispatcher;
	IIEControlNotify *m_pNotify;

	CString m_strLoadPage;
	CString m_strErrorPage;

	CAxWindow m_wndIE;

	ksOleClientSite *m_pKsClientSite;

    HRESULT flag_;

    
};

#endif