#include "stdafx.h"
#include "ksOleClientSite.h"
#include "exdisp.h"
#include <atlcomcli.h>

// *** IOleClientSite ***
HRESULT STDMETHODCALLTYPE ksOleClientSite::SaveObject()
{
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->SaveObject();
	}
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER *ppmk)
{
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
	}
	else
	{
		if (! IsBadWritePtr(ppmk, sizeof(*ppmk)))
			*ppmk = NULL;
		return E_NOTIMPL;
	}
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetContainer(LPOLECONTAINER *ppContainer)
{	
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->GetContainer(ppContainer);
	}
	else
	{
		if (! IsBadWritePtr(ppContainer, sizeof(*ppContainer)))
			*ppContainer = NULL;

		return E_NOINTERFACE;
	}
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::ShowObject()
{
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->ShowObject();
	}
	return S_OK;		
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::OnShowWindow(BOOL fShow)
{
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->OnShowWindow(fShow);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::RequestNewObjectLayout()
{
	if (m_spDefOleClientSite != NULL)
	{
		return m_spDefOleClientSite->RequestNewObjectLayout();
	}

	return E_NOTIMPL;
}


//IOleCommandTarget
HRESULT STDMETHODCALLTYPE ksOleClientSite::QueryStatus( const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText)
{

    if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_KIECONTROL_AX))
    {
        return flag_hr_;
    }

	return OLECMDERR_E_NOTSUPPORTED;

}
HRESULT STDMETHODCALLTYPE ksOleClientSite::Exec( const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut)
{
	HRESULT hr = S_OK;

	if ( pguidCmdGroup != NULL && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler) )
	{
		switch (nCmdID) 
		{
		case OLECMDID_SHOWSCRIPTERROR:
			//去掉脚本错误显示
			hr = S_OK;
			break;

	/*	case OLECMDID_ADDTRAVELENTRY:
			hr = OLECMD_AddTravelEntry(pvaIn, pvaOut);
			break;

		case OLECMDID_UPDATETRAVELENTRY:
			hr = OLECMD_UpdateTravelEntry(pvaIn, pvaOut);
			break;

		case OLECMDID_UPDATEBACKFORWARDSTATE:
			hr = OLECMD_UpdateBackForwardState(pvaIn, pvaOut);
			break;*/

		default:
			hr = OLECMDERR_E_NOTSUPPORTED;
			break;
		}
	}
	else
	{
		hr = OLECMDERR_E_UNKNOWNGROUP;
	}

	return (hr);

}

//IDocHostUIHandler
HRESULT STDMETHODCALLTYPE ksOleClientSite::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	if (m_spDefDocHostHandler)
	{
		return m_spDefDocHostHandler->ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->GetHostInfo(pInfo);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::ShowUI(DWORD dwID,  IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
	}

	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::HideUI()
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->HideUI();
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::UpdateUI()
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->UpdateUI();
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::EnableModeless(BOOL fEnable)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->EnableModeless(fEnable);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::OnDocWindowActivate(BOOL fActivate)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->OnDocWindowActivate(fActivate);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::OnFrameWindowActivate(BOOL fActivate)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->OnFrameWindowActivate(fActivate);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::ResizeBorder(LPCRECT prcBorder,IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::TranslateAccelerator(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
	}	
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetOptionKeyPath( LPOLESTR *pchKey, DWORD dw)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->GetOptionKeyPath(pchKey, dw);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetDropTarget(IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
{
	if (m_spDefDocHostHandler != NULL)
	{
		HRESULT result = m_spDefDocHostHandler->GetDropTarget(pDropTarget, ppDropTarget);

		//Returning S_FALSE seems to disable DragNDrop, while DragNDrop is by default on.
		//Changing return code to E_FAIL seems to fix things. 
		if (result == S_FALSE)
			result = E_FAIL;

		return result;
	}
	return E_NOTIMPL;	
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::GetExternal(IDispatch **ppDispatch)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->GetExternal(ppDispatch);
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::TranslateUrl( DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
	}
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE ksOleClientSite::FilterDataObject( IDataObject *pDO, IDataObject  **ppDORet)
{
	if (m_spDefDocHostHandler != NULL)
	{
		return m_spDefDocHostHandler->FilterDataObject(pDO, ppDORet);
	}
	return S_FALSE;
}


//
void ksOleClientSite::Init(IWebBrowser2* pIWeb)
{
	if (pIWeb == NULL)
	{
		return ;
	}

	CComQIPtr<IOleObject> spOleObject = pIWeb;
	if (spOleObject == NULL)
	{
		return ;
	}
	CComPtr<IOleClientSite> spClientSite;
	spOleObject->GetClientSite(&spClientSite);

	if (spClientSite)
	{
		m_spDefOleClientSite = spClientSite;		
		spClientSite->QueryInterface(IID_IDocHostUIHandler, (void**)&m_spDefDocHostHandler);		
	}

	spOleObject->SetClientSite(this);

}
//判断是不是自己的控件
bool ksOleClientSite::IsKwsIEControl(IWebBrowser2* pIWeb, HRESULT flag)
{
    if (NULL == pIWeb)
    {
        return false;
    }
    CComQIPtr<IOleObject> spOleObject = pIWeb;
    if (spOleObject)
    {
        CComPtr<IOleClientSite> spClientSite;
        spOleObject->GetClientSite(&spClientSite);
        if (spClientSite)
        {
            CComQIPtr<IOleCommandTarget> spTarget = spClientSite;
            if (spTarget)
            {
                HRESULT hr = spTarget->QueryStatus(&CGID_KIECONTROL_AX, 0, NULL, NULL);
                if (hr == flag)
                {
                    return true;
                }
            }
        }
    }

    return false;
}