#ifndef __ks_oleclient_site__
#define __ks_oleclient_site__
#pragma once
#include "ksIUnknown.h"
#include "docobj.h"
#include <mshtmhst.h>
#include "exdisp.h"



// {6662C8E4-FC83-48ea-8260-2E55984C8136}
static const GUID CGID_KIECONTROL_AX =  { 0x6662c8e4, 0xfc83, 0x48ea, { 0x82, 0x60, 0x2e, 0x55, 0x98, 0x4c, 0x81, 0x36 } };


class ksOleClientSite : public IOleClientSite , 
						public IOleCommandTarget,
						public IDocHostUIHandler,
						public ksIUnknown
{
public:
	IMP_BEGIN
		IMP_NONHERE(IOleClientSite)
		IMP_NONHERE(IOleCommandTarget)
		IMP_NONHERE(IDocHostUIHandler)
	IMP_END
public:
    ksOleClientSite()
    {
        flag_hr_ = S_OK;
    }
	void Init(IWebBrowser2* pIWeb);

    void SetFlag(HRESULT hr)
    {
        flag_hr_ = hr;
    }

    static bool IsKwsIEControl(IWebBrowser2* pIWeb, HRESULT flag);

	//IOleClientSite
	STDMETHOD(SaveObject)();
	STDMETHOD(GetMoniker)( /* [in] */ DWORD dwAssign,/* [in] */ DWORD dwWhichMoniker, /* [out] */ IMoniker **ppmk);
	STDMETHOD(GetContainer)(/* [out] */ IOleContainer **ppContainer);
	STDMETHOD(ShowObject)();
	STDMETHOD(OnShowWindow)(/* [in] */ BOOL fShow);
	STDMETHOD(RequestNewObjectLayout)();

	//IOleCommandTarget
	STDMETHOD (QueryStatus)( const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText);
	STDMETHOD (Exec)( const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut);
	
	//IDocHostUIHandler
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt,  IUnknown *pcmdtReserved,IDispatch *pdispReserved);
	STDMETHOD(GetHostInfo)( DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowUI)( DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc);
	STDMETHOD(HideUI)();
	STDMETHOD(UpdateUI)();
	STDMETHOD(EnableModeless)( BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(  /* [in] */ BOOL fActivate);
	STDMETHOD(OnFrameWindowActivate)( /* [in] */ BOOL fActivate);
	STDMETHOD(ResizeBorder)( /* [in] */ LPCRECT prcBorder, /* [in] */ IOleInPlaceUIWindow *pUIWindow, /* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)( /* [in] */ LPMSG lpMsg, /* [in] */ const GUID *pguidCmdGroup,/* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)( /* [out] */ LPOLESTR *pchKey, /* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)( /* [in] */ IDropTarget *pDropTarget, /* [out] */ IDropTarget **ppDropTarget);
	STDMETHOD(GetExternal)( /* [out] */ IDispatch **ppDispatch);
	STDMETHOD(TranslateUrl)( /* [in] */ DWORD dwTranslate, /* [in] */ OLECHAR *pchURLIn, /* [out] */ OLECHAR **ppchURLOut);
	STDMETHOD(FilterDataObject)( /* [in] */ IDataObject *pDO, /* [out] */ IDataObject **ppDORet);
	//

protected:
	//默认的处理器
	CComPtr<IOleClientSite>		m_spDefOleClientSite;
	CComPtr<IDocHostUIHandler>  m_spDefDocHostHandler;

    HRESULT flag_hr_;

};
#endif //