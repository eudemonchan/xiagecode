// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;


// CClientDlg �Ի���
class CClientDlg : public CDialog
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
