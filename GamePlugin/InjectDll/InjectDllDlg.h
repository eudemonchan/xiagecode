
// InjectDllDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CInjectDllDlg 对话框
class CInjectDllDlg : public CDialog
{
// 构造
public:
	CInjectDllDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INJECTDLL_DIALOG };

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
	afx_msg void OnBnClickedBtnInject();
	CEdit m_editDllPath;
	CEdit m_editProName;
	afx_msg void OnBnClickedBtnEject();
	BOOL m_bInjected;
};
