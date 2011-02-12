// ClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;


// CClientDlg 对话框
class CClientDlg : public CDialog
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

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
public:
	afx_msg void OnBnClickedBtnMount();
public:
	afx_msg void OnBnClickedCheck1();
public:
	CButton m_chkAutoMount;
	TCHAR m_cfgPath[MAX_PATH];
	afx_msg void OnBnClickedButtonSelect();
	CButton m_btnMount;
	CButton m_btnSelect;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};
