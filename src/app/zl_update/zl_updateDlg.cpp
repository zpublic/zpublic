
// zl_updateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zl_update.h"
#include "zl_updateDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Czl_updateDlg 对话框



Czl_updateDlg::Czl_updateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Czl_updateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Czl_updateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listbox);
    DDX_Control(pDX, IDC_BUTTON1, m_btn);
}

BEGIN_MESSAGE_MAP(Czl_updateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &Czl_updateDlg::OnBnClickedButton1)
    
    ON_MESSAGE(WM_USER_ADD_STRING, OnAddString)
    ON_MESSAGE(WM_USER_ENABLE_BTN, OnEnableBtn)
END_MESSAGE_MAP()


// Czl_updateDlg 消息处理程序

BOOL Czl_updateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

    m_listbox.AddString(L"点击\"更新\"按钮进行更新！");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Czl_updateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Czl_updateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Czl_updateDlg::OnBnClickedButton1()
{
    SendMessage(WM_USER_ADD_STRING, (WPARAM)L"开始更新。。。");
    HANDLE h = (HANDLE)_beginthreadex(NULL, 0, ThreadUpdate, this, 0, NULL);
    if (h)
    {
        m_btn.EnableWindow(FALSE);
        ::CloseHandle(h);
    }
    else
    {
        SendMessage(WM_USER_ADD_STRING, (WPARAM)L"创建更新线程失败");
    }
}

unsigned int __stdcall Czl_updateDlg::ThreadUpdate(void* param)
{
    Czl_updateDlg* p = (Czl_updateDlg*)param;
    p->_ThreadUpdate();
    return 0;
}

void Czl_updateDlg::_ThreadUpdate()
{
    PostMessage(WM_USER_ADD_STRING, (WPARAM)L"下载索引文件");

    zl::http::ZLHttpClient http;
    zl::http::ZLMemWrite m;
    if (0 == http.DownloadMem(L"http://www.laorouji.com/kxlol/data.json", &m, 20000))
    {
        PostMessage(WM_USER_ADD_STRING, (WPARAM)L"下载索引文件成功");
        Json::Reader read;
        Json::Value v;
        if (read.parse(
            (const char*)m.GetData(),
            (const char*)(m.GetData() + m.GetLength() - 1),
            v))
        {


            PostMessage(WM_USER_ADD_STRING, (WPARAM)L"更新完成");
        }
        else
        {
            PostMessage(WM_USER_ADD_STRING, (WPARAM)L"解析索引文件失败");
        }
    }
    else
    {
        PostMessage(WM_USER_ADD_STRING, (WPARAM)L"下载索引文件失败");
    }
    PostMessage(WM_USER_ENABLE_BTN);
}

LRESULT Czl_updateDlg::OnAddString(WPARAM w, LPARAM l)
{
    m_listbox.AddString((LPCWSTR)(w));
    m_listbox.SetSel(m_listbox.GetCount() - 1, FALSE);
    return 0;
}

LRESULT Czl_updateDlg::OnEnableBtn(WPARAM w, LPARAM l)
{
    m_btn.EnableWindow(TRUE);
    return 0;
}
