// Connect.cpp : CConnect 的具体实现
#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"
#include "dlg_lu.h"

extern CAddInModule _AtlModule;

// CConnect
STDMETHODIMP CConnect::OnConnection(IDispatch *pApplication, ext_ConnectMode ConnectMode, IDispatch *pAddInInst, SAFEARRAY ** /*自定义*/ )
{
	HRESULT hr = S_OK;
	pApplication->QueryInterface(__uuidof(DTE2), (LPVOID*)&m_pDTE);
	pAddInInst->QueryInterface(__uuidof(EnvDTE::AddIn), (LPVOID*)&m_pAddInInstance);
	if(ConnectMode == 5) //5 == ext_cm_UISetup
	{
		HRESULT hr = S_OK;
		CComPtr<IDispatch> pDisp;
		CComQIPtr<EnvDTE::Commands> pCommands;
		CComQIPtr<Commands2> pCommands2;
		CComQIPtr<_CommandBars> pCommandBars;
		CComPtr<CommandBarControl> pCommandBarControl;
		CComPtr<EnvDTE::Command> pCreatedCommand;
		CComPtr<CommandBar> pMenuBarCommandBar;
		CComPtr<CommandBarControls> pMenuBarControls;
		CComPtr<CommandBarControl> pToolsCommandBarControl;
		CComQIPtr<CommandBar> pToolsCommandBar;
		CComQIPtr<CommandBarPopup> pToolsPopup;

		IfFailGoCheck(m_pDTE->get_Commands(&pCommands), pCommands);
		pCommands2 = pCommands;
		if(SUCCEEDED(pCommands2->AddNamedCommand2(m_pAddInInstance, CComBSTR("teapot"), CComBSTR("Teapot"), CComBSTR("Executes the command for teapot"), VARIANT_TRUE, CComVariant(59), NULL, EnvDTE::vsCommandStatusSupported+EnvDTE::vsCommandStatusEnabled, vsCommandStylePictAndText, vsCommandControlTypeButton, &pCreatedCommand)) && (pCreatedCommand))
		{
			//在“工具”菜单栏上添加一个按钮。
			IfFailGoCheck(m_pDTE->get_CommandBars(&pDisp), pDisp);
			pCommandBars = pDisp;

			//查找 MenuBar 命令栏，该命令栏是容纳所有主菜单项的顶级命令栏:
			IfFailGoCheck(pCommandBars->get_Item(CComVariant(L"MenuBar"), &pMenuBarCommandBar), pMenuBarCommandBar);
			IfFailGoCheck(pMenuBarCommandBar->get_Controls(&pMenuBarControls), pMenuBarControls);

			//在 MenuBar 命令栏上查找“工具”命令栏:
			IfFailGoCheck(pMenuBarControls->get_Item(CComVariant(L"Tools"), &pToolsCommandBarControl), pToolsCommandBarControl);
			pToolsPopup = pToolsCommandBarControl;
			IfFailGoCheck(pToolsPopup->get_CommandBar(&pToolsCommandBar), pToolsCommandBar);

			//添加控件:
			pDisp = NULL;
			IfFailGoCheck(pCreatedCommand->AddControl(pToolsCommandBar, 1, &pDisp), pDisp);
		}
		return S_OK;
	}
Error:
	return hr;
}

STDMETHODIMP CConnect::OnDisconnection(ext_DisconnectMode /*RemoveMode*/, SAFEARRAY ** /*自定义*/ )
{
	m_pDTE = NULL;
	m_pAddInInstance = NULL;
	return S_OK;
}

STDMETHODIMP CConnect::OnAddInsUpdate (SAFEARRAY ** /*自定义*/ )
{
	return S_OK;
}

STDMETHODIMP CConnect::OnStartupComplete (SAFEARRAY ** /*自定义*/ )
{
	return S_OK;
}

STDMETHODIMP CConnect::OnBeginShutdown (SAFEARRAY ** /*自定义*/ )
{
	return S_OK;
}

STDMETHODIMP CConnect::QueryStatus(BSTR bstrCmdName, EnvDTE::vsCommandStatusTextWanted NeededText, EnvDTE::vsCommandStatus *pStatusOption, VARIANT *pvarCommandText)
{
  if(NeededText == EnvDTE::vsCommandStatusTextWantedNone)
	{
	  if(!_wcsicmp(bstrCmdName, L"teapot.Connect.teapot"))
	  {
		  *pStatusOption = (EnvDTE::vsCommandStatus)(EnvDTE::vsCommandStatusEnabled+EnvDTE::vsCommandStatusSupported);
	  }
  }
	return S_OK;
}

STDMETHODIMP CConnect::Exec(BSTR bstrCmdName, EnvDTE::vsCommandExecOption ExecuteOption, VARIANT * /*pvarVariantIn*/, VARIANT * /*pvarVariantOut*/, VARIANT_BOOL *pvbHandled)
{
	*pvbHandled = VARIANT_FALSE;
	if(ExecuteOption == EnvDTE::vsCommandExecOptionDoDefault)
	{
		if(!_wcsicmp(bstrCmdName, L"teapot.Connect.teapot"))
		{
            CDlgLu dlg;
            dlg.DoModal();
			*pvbHandled = VARIANT_TRUE;
			return S_OK;
		}
	}
	return S_OK;
}