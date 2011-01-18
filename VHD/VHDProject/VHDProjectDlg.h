
// VHDProjectDlg.h : header file
//

#pragma once

#include "VHDManager.h"
#include "afxcmn.h"
// CVHDProjectDlg dialog
class CVHDProjectDlg : public CDialogEx
{
// Construction
public:
	CVHDProjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VHDPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	CVHDManager m_vhd;
	afx_msg void OnBnClickedBtnCreate();
	OVERLAPPED m_ol;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_proCtrl;
	afx_msg void OnBnClickedBtnStopdisk();
	afx_msg void OnBnClickedBtnMount();
};
