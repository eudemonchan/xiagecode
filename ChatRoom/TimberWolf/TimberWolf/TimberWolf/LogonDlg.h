#pragma once


// CLogonDlg �Ի���

class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogonDlg();

// �Ի�������
	enum { IDD = IDD_LOGON };

    WCHAR _wzUserAccount[128];  // Use MACRO
    WCHAR _wzUserPassword[128];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnBnClickedOk();
};
