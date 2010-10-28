#pragma once


// CLogonDlg 对话框

class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogonDlg();

// 对话框数据
	enum { IDD = IDD_LOGON };

    WCHAR _wzUserAccount[128];  // Use MACRO
    WCHAR _wzUserPassword[128];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnBnClickedOk();
};
