#pragma once
#include "afxwin.h"


// CGetPassDlg �Ի���

class CGetPassDlg : public CDialog
{
	DECLARE_DYNAMIC(CGetPassDlg)

public:
	CGetPassDlg(BOOL bEncrypt,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetPassDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CEdit m_editPassword1;
public:
	CEdit m_editPassword2;
	byte m_encryptKey[16];
	byte m_secKey[16];

	BOOL m_bEncrypt;
public:
	virtual BOOL OnInitDialog();
};
