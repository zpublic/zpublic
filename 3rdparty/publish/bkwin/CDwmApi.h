#pragma  once

#include <dwmapi.h>

typedef HRESULT  (__stdcall * pFunDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* pMarInset);
typedef HRESULT  (__stdcall * pFunDwmIsCompositionEnabled)(__out BOOL* pfEnabled);
typedef HRESULT  (__stdcall * pFunDwmEnableComposition)(UINT uCompositionAction);
typedef HRESULT (__stdcall * pFunDwmEnableBlurBehindWindow) (
	HWND hWnd, 
	__in const DWM_BLURBEHIND* pBlurBehind
	);


class CDwmApi
{
public:

	enum enumSystemVersion
	{
		enumSystemVersionNone = 0,
		enumSystemVersionWin9X,
		enumSystemVersionWinMe,
		enumSystemVersionWin2000,
		enumSystemVersionWinNT,
		enumSystemVersionWin2003,
		enumSystemVersionWinXp,
		enumSystemVersionVista,
		enumSystemVersionWin7,
		enumSystemVersionWin2008,
		enumSystemVersionWin8,
	};

	static enumSystemVersion GetSystemVersion()
	{
		static enumSystemVersion OsPlatform = enumSystemVersionNone;
		if (OsPlatform == enumSystemVersionNone)
			OsPlatform = GetSystemVersionImpl();

		return OsPlatform;
	}

	static enumSystemVersion GetSystemVersionImpl()
	{
		int nRetCode = false;
		enumSystemVersion OsPlatform = enumSystemVersionNone;

		OSVERSIONINFOEX osvi;  
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		nRetCode = ::GetVersionEx((OSVERSIONINFO *)&osvi);
		if (FALSE == nRetCode)
		{
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			nRetCode = ::GetVersionEx((OSVERSIONINFO *)&osvi);
		}

		if (nRetCode == FALSE)
			goto Exit0;

		switch(osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
				OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? enumSystemVersionWin7 : enumSystemVersionWin2008;
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
				OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? enumSystemVersionVista : enumSystemVersionWin2008;
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				OsPlatform = enumSystemVersionWin2003;
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				OsPlatform = enumSystemVersionWinXp;
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				OsPlatform = enumSystemVersionWin2000;
			else if (osvi.dwMajorVersion <= 4)
				OsPlatform = enumSystemVersionWinNT;
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 && osvi.wProductType == VER_NT_WORKSTATION)
				OsPlatform = enumSystemVersionWin8;
			break;
		case VER_PLATFORM_WIN32_WINDOWS:
			if (((osvi.dwBuildNumber >> 16) & 0x0000FFFF) < 0x045A)
				OsPlatform = enumSystemVersionWin9X;
			else 
				OsPlatform = enumSystemVersionWinMe;
			break;
		default:
			OsPlatform = enumSystemVersionNone;
			break;
		}

Exit0:
		return OsPlatform;
	}

	HRESULT DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS* pMarInset)const
	{
		HRESULT hRet = E_FAIL;

		if (m_FuncExtend)
		{
			hRet = m_FuncExtend(hWnd, pMarInset);
		}

		return hRet;
	}

	HRESULT DwmIsCompositionEnabled(__out BOOL* pfEnabled) const
	{
		HRESULT hRet = E_FAIL;

		if (m_FuncIsCompsite && pfEnabled)
		{
			hRet = m_FuncIsCompsite(pfEnabled);
		}

		return hRet;
	}

	HRESULT DwmEnableComposition(UINT uCompositionAction)
	{
		HRESULT hRet = E_FAIL;

		if (m_FuncEnableCompsite)
		{
			hRet = m_FuncEnableCompsite(uCompositionAction);
		}

		return hRet;
	}

	HRESULT DwmEnableBlurBehindWindow(HWND hWnd, HRGN hRgn)
	{
		HRESULT hRet = E_FAIL;

		if (m_FuncEnableBlurWindow)
		{
			DWM_BLURBEHIND bb = {0};
			bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;

			bb.fEnable = (NULL == hRgn ? FALSE : TRUE);
			bb.hRgnBlur = hRgn;

			if (m_FuncEnableBlurWindow)
			{ 
				hRet = m_FuncEnableBlurWindow(hWnd, &bb); 
			}
		}

		return hRet;
	}


	static CDwmApi& Instance()
	{
		return m_stInst;
	}

protected:

	static CDwmApi m_stInst;

protected:

		CDwmApi()
		{
			m_FuncExtend = NULL;
			m_bInit = FALSE;
			m_hModule = NULL;
			m_FuncIsCompsite = NULL;
			m_FuncEnableCompsite = NULL;
			m_FuncEnableBlurWindow = NULL;

			Init();
		}

		~CDwmApi()
		{
			Unit();
		}

		BOOL Init()
		{
			if (m_bInit)
			{
				return TRUE;
			}

			if (m_hModule == NULL)
			{
				m_hModule = LoadLibrary(L"dwmapi");
			}

			if (m_hModule == NULL)
			{
				return FALSE;
			}

			if (m_FuncExtend == NULL)
			{
				m_FuncExtend = (pFunDwmExtendFrameIntoClientArea)GetProcAddress(m_hModule, "DwmExtendFrameIntoClientArea");
			}

			if (m_FuncIsCompsite == NULL)
			{
				m_FuncIsCompsite = (pFunDwmIsCompositionEnabled)GetProcAddress(m_hModule, "DwmIsCompositionEnabled");
			}

			if (m_FuncEnableCompsite == NULL)
			{
				m_FuncEnableCompsite = (pFunDwmEnableComposition)GetProcAddress(m_hModule, "DwmEnableComposition");
			}

			if (m_FuncEnableBlurWindow == NULL)
			{
				m_FuncEnableBlurWindow = (pFunDwmEnableBlurBehindWindow)GetProcAddress(m_hModule, "DwmEnableBlurBehindWindow");
			}


			m_bInit = ((m_hModule && m_FuncExtend && m_FuncIsCompsite && m_FuncEnableCompsite && m_FuncEnableBlurWindow) ? TRUE : FALSE);

			return m_bInit;
		}

		void Unit()
		{
			if (m_hModule)
			{
				FreeLibrary(m_hModule);
				m_hModule = FALSE;
			}

			m_FuncExtend = NULL;
			m_FuncIsCompsite = NULL;
			m_FuncEnableCompsite = NULL;
			m_FuncEnableBlurWindow = NULL;
			m_FuncEnableBlurWindow = NULL;
		}


private:

	HMODULE m_hModule;
	BOOL	m_bInit;

	pFunDwmExtendFrameIntoClientArea m_FuncExtend;
	pFunDwmIsCompositionEnabled	  m_FuncIsCompsite;
	pFunDwmEnableComposition	  m_FuncEnableCompsite;
	pFunDwmEnableBlurBehindWindow m_FuncEnableBlurWindow;
};

__declspec(selectany) CDwmApi CDwmApi::m_stInst;

