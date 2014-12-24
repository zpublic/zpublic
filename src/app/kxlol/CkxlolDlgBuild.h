#pragma once


// CkxlolDlgBuild 对话框

class CkxlolDlgBuild : public CDialogEx
{
	DECLARE_DYNAMIC(CkxlolDlgBuild)

public:
	CkxlolDlgBuild(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CkxlolDlgBuild();

// 对话框数据
	enum { IDD = IDD_KXLOL_DIALOG_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnBuildShenminxinqiu();
};
