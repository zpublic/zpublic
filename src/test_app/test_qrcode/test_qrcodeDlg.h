
// test_qrcodeDlg.h : 头文件
//

#pragma once
#include "zqrcode_wrappage.h"
#include "z_win_utils\gdiplus.hpp"

// Ctest_qrcodeDlg 对话框
class Ctest_qrcodeDlg : public CDialogEx
{
// 构造
public:
	Ctest_qrcodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_QRCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    zl::WinUtils::ZLGdiPlusInit m_gdiplus;
    ZQRcodeWrappage             m_qrcode;
};
