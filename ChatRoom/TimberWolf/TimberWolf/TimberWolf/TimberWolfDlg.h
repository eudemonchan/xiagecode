// TimberWolfDlg.h : ͷ�ļ�
//

#pragma once


// CTimberWolfDlg �Ի���
class CTimberWolfDlg : public CDialog
{
// ����
public:
	CTimberWolfDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TIMBERWOLF_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
    BOOL InitCtrl(void);

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
    // �Զ�����Ϣ������

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedMain();
    afx_msg void OnBnClickedFileShare();
    afx_msg void OnBnClickedSendMsg();
    afx_msg void OnBnClickedExit();
    afx_msg void OnBnClickedSendPic();
    afx_msg void OnBnClickedSetFont();
public:
    afx_msg void OnNMRclickListMember(NMHDR *pNMHDR, LRESULT *pResult);
};
