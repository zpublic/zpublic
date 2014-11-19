
// test_qrcodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test_qrcode.h"
#include "test_qrcodeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctest_qrcodeDlg 对话框



Ctest_qrcodeDlg::Ctest_qrcodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest_qrcodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_qrcodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_qrcodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Ctest_qrcodeDlg 消息处理程序

BOOL Ctest_qrcodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_qrcode.Init(L"E:\\code\\project\\zpublic\\sln\\Debug\\qrcode.dll");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Ctest_qrcodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ctest_qrcodeDlg::OnPaint()
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
        CPaintDC dc(this); // 用于绘制的设备上下文
        char* p = "asdasd";
        ZQRcodeData* recodedata = m_qrcode.QrcodeEncode(p);
        BYTE* row = NULL;
        int nWidth = recodedata->width;
        int nHeight = nWidth;
        int x = 0;
        int y = 0;
        Gdiplus::Bitmap bitmap(500, 500, PixelFormat32bppARGB);
        Gdiplus::SolidBrush whitePen(Gdiplus::Color(255, 255, 255, 255));
        Gdiplus::SolidBrush blackPen(Gdiplus::Color(255, 0, 0, 0));
        Gdiplus::Graphics gs(&bitmap);
        Gdiplus::Graphics screen(dc);
        gs.FillRectangle(&whitePen, 0, 0, 500, 500);
        if(recodedata)
        {
            for( y = 0; y < recodedata->width; y++)
            {
                row = recodedata->data+(y * recodedata->width);
                for(x = 0; x < recodedata->width; x++)
                {
                    if(row[x] & 0x1)
                    {
                        gs.FillRectangle( &blackPen, x * nWidth, y * nHeight, nWidth, nHeight);
                    }
                }
            }
        }
        screen.DrawImage(&bitmap, Gdiplus::Point(0, 0));
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ctest_qrcodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

