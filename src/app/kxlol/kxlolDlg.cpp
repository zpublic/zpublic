
// kxlolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kxlol.h"
#include "kxlolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CkxlolDlg 对话框



CkxlolDlg::CkxlolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CkxlolDlg::IDD, pParent)
    , m_ulShenli(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkxlolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_CURRENT_SHENLI, m_ulShenli);
    DDX_Control(pDX, IDC_LIST_MSG, m_listMsg);
    DDX_Control(pDX, IDC_BTN_NINGJUSHENLI, m_btnNingjushenli);
    DDX_Control(pDX, IDC_TREE_RES, m_treeRes);
}

BEGIN_MESSAGE_MAP(CkxlolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_GOTO_DONGLIJIAN, &CkxlolDlg::OnBnClickedBtnGotoDonglijian)
    ON_BN_CLICKED(IDC_BTN_GOTO_YUANSHIYUZHOU, &CkxlolDlg::OnBnClickedBtnGotoYuanshiyuzhou)
    ON_BN_CLICKED(IDC_BTN_NINGJUSHENLI, &CkxlolDlg::OnBnClickedBtnNingjushenli)
    ON_BN_CLICKED(IDC_BTN_BUILD, &CkxlolDlg::OnBnClickedBtnBuild)
    ON_BN_CLICKED(IDC_BTN_MAKE, &CkxlolDlg::OnBnClickedBtnMake)
    ON_BN_CLICKED(IDC_BTN_PRACTIOCE, &CkxlolDlg::OnBnClickedBtnPractioce)
    ON_BN_CLICKED(IDC_BTN_CONNECT_XUKONG, &CkxlolDlg::OnBnClickedBtnConnectXukong)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CkxlolDlg 消息处理程序

BOOL CkxlolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    HTREEITEM h1 = m_treeRes.InsertItem(L"原始位面");
    m_treeRes.InsertItem(L"星球：1", h1);

    HTREEITEM h2 = m_treeRes.InsertItem(L"部下");
    m_treeRes.InsertItem(L"食人魔：3", h2);
    m_treeRes.InsertItem(L"位面破灭者：1", h2);

    HTREEITEM h3 = m_treeRes.InsertItem(L"材料");
    m_treeRes.InsertItem(L"泡面：2", h3);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CkxlolDlg::OnPaint()
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
HCURSOR CkxlolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CkxlolDlg::OnBnClickedBtnGotoDonglijian()
{
    MessageBox(L"动力茧还没做。。。");
}


void CkxlolDlg::OnBnClickedBtnGotoYuanshiyuzhou()
{
    MessageBox(L"原始位面还没做。。。");
}


void CkxlolDlg::OnBnClickedBtnNingjushenli()
{
    m_listMsg.AddString(L"开始凝聚神力...");
    m_btnNingjushenli.EnableWindow(FALSE);
    m_btnNingjushenli.SetWindowText(L"正在凝聚神力");
    SetTimer(1, 3000, NULL);
}


void CkxlolDlg::OnBnClickedBtnBuild()
{
    // TODO:  在此添加控件通知处理程序代码
}


void CkxlolDlg::OnBnClickedBtnMake()
{
    // TODO:  在此添加控件通知处理程序代码
}


void CkxlolDlg::OnBnClickedBtnPractioce()
{
    // TODO:  在此添加控件通知处理程序代码
}


void CkxlolDlg::OnBnClickedBtnConnectXukong()
{
    // TODO:  在此添加控件通知处理程序代码
}


void CkxlolDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 1:
        KillTimer(1);
        m_listMsg.AddString(L"凝聚神力完成，增加1点神力");
        m_btnNingjushenli.EnableWindow(TRUE);
        m_btnNingjushenli.SetWindowText(L"凝聚神力");
        m_ulShenli++;
        UpdateData(FALSE);
        break;
    default:
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}
