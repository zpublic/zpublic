// CkxlolDlgBuild.cpp : 实现文件
//

#include "stdafx.h"
#include "kxlol.h"
#include "CkxlolDlgBuild.h"
#include "afxdialogex.h"


// CkxlolDlgBuild 对话框

IMPLEMENT_DYNAMIC(CkxlolDlgBuild, CDialogEx)

CkxlolDlgBuild::CkxlolDlgBuild(CWnd* pParent /*=NULL*/)
    : CDialogEx(CkxlolDlgBuild::IDD, pParent)
{

}

CkxlolDlgBuild::~CkxlolDlgBuild()
{
}

void CkxlolDlgBuild::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CkxlolDlgBuild, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BUILD_SHENMINXINQIU, &CkxlolDlgBuild::OnBnClickedBtnBuildShenminxinqiu)
    ON_BN_CLICKED(IDC_LUNHUISHENDIAN, &CkxlolDlgBuild::OnBnClickedLunhuishendian)
END_MESSAGE_MAP()


// CkxlolDlgBuild 消息处理程序


void CkxlolDlgBuild::OnBnClickedBtnBuildShenminxinqiu()
{
    GAME.Logic().Build().Build(L"shenminxinqiu");
}


void CkxlolDlgBuild::OnBnClickedLunhuishendian()
{
    GAME.Logic().Build().Build(L"lunhuishendian");
    // TODO: 在此添加控件通知处理程序代码
}
