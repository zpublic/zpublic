#pragma  once

class IProcessMsgBack
{
public:
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
};

class CBkMsgWnd : public CWindowImpl<CBkMsgWnd, CWindow, CFrameWinTraits>
{
public:

	CBkMsgWnd()
	{
	}

	BEGIN_MSG_MAP(CBkMsgWnd)
		CHAIN_MSG_MAP_MEMBER(m_ChanMsgMap)
	END_MSG_MAP()

	void SetCallBack(IProcessMsgBack* pProcessMsg)
	{
		m_ChanMsgMap.m_pProcessMsg = pProcessMsg;
	}

protected:
	class KChainMsgMap
	{
	public:

		KChainMsgMap()
		{
			m_pProcessMsg = NULL;
		}

		BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
		{
			BOOL bHandled = FALSE;
			if (m_pProcessMsg)
			{
				bHandled = TRUE;
				lResult = m_pProcessMsg->ProcessWindowMessage(uMsg, wParam, lParam, bHandled);
			}

			return bHandled;
		}

		IProcessMsgBack* m_pProcessMsg;
	};

private:
	KChainMsgMap m_ChanMsgMap;
};