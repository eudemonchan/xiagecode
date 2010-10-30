// TimberWolfDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

//#include "ResizableDialog.h"
#include "xDialog.h"
#include "SeparatorStatic.h"

// CTimberWolfDlg 对话框
class CTimberWolfDlg : public ClxDialog/*CDialog*/
{
// 构造
public:
	CTimberWolfDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TIMBERWOLF_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
    BOOL InitCtrl(void);

protected:
    CSeparatorStatic m_staticSeparator;

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
    // 自定义消息。。。

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedMain();
    afx_msg void OnBnClickedFileShare();
    afx_msg void OnBnClickedSendMsg();
    afx_msg void OnBnClickedExit();
    afx_msg void OnBnClickedSendPic();
    afx_msg void OnBnClickedSetFont();
    afx_msg void OnNMRclickListMember(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
    afx_msg void OnBnClickedSeparator();
};
