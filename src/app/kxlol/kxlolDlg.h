
// kxlolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include <string>


// CkxlolDlg 对话框
class CkxlolDlg : public CDialogEx
{
// 构造
public:
	CkxlolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
    enum { IDD = IDD_KXLOL_DIALOG_ZHUSHENKONGJIAN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void CreateNode(const wchar_t* rootName, const wchar_t* nodeName);
	void UpdateNode(const wchar_t* rootName, const wchar_t* nodeName);
	void DeleteNode(const wchar_t* rootName, const wchar_t* nodeName);
public:
    afx_msg void OnBnClickedBtnGotoDonglijian();
    afx_msg void OnBnClickedBtnGotoYuanshiyuzhou();
    afx_msg void OnBnClickedBtnNingjushenli();
    afx_msg void OnBnClickedBtnBuild();
    afx_msg void OnBnClickedBtnMake();
    afx_msg void OnBnClickedBtnPractioce();
    afx_msg void OnBnClickedBtnConnectXukong();
	afx_msg LRESULT OnUpdateGUI(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateTreeNode(WPARAM pRootName, LPARAM pNodeName);
private:
    // 当前神力值
    ULONGLONG m_ulShenli;
public:
    // 主神空间下面的消息框
    CListBox m_listMsg;
    // 凝聚神力按钮
    CButton m_btnNingjushenli;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    // 当前拥有的资源
    CTreeCtrl m_treeRes;
	
	typedef std::map<std::wstring, HTREEITEM> TREENODEHANDLE;
	typedef std::map<std::wstring, std::pair<HTREEITEM, TREENODEHANDLE>> TREEROOTHANDLE;
	TREEROOTHANDLE m_treeNodeHandles;
};
