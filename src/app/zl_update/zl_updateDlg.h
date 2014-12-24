
// zl_updateDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define WM_USER_ADD_STRING      WM_USER+1
#define WM_USER_ENABLE_BTN      WM_USER+2

// Czl_updateDlg 对话框
class Czl_updateDlg : public CDialogEx
{
// 构造
public:
	Czl_updateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZL_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

    static unsigned int __stdcall ThreadUpdate(void* param);
    void _ThreadUpdate();

    LRESULT OnAddString(WPARAM w, LPARAM l);
    LRESULT OnEnableBtn(WPARAM w, LPARAM l);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    CListBox m_listbox;
    afx_msg void OnBnClickedButton1();
    CButton m_btn;
};
