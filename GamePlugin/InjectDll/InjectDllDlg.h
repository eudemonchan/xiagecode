
// InjectDllDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CInjectDllDlg �Ի���
class CInjectDllDlg : public CDialog
{
// ����
public:
	CInjectDllDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INJECTDLL_DIALOG };

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
	afx_msg void OnBnClickedBtnInject();
	CEdit m_editDllPath;
	CEdit m_editProName;
	afx_msg void OnBnClickedBtnEject();
	BOOL m_bInjected;
};
