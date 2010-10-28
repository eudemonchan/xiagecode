#pragma once


// CPicListDlg dialog

class CPicListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicListDlg)

public:
	CPicListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicListDlg();

// Dialog Data
	enum { IDD = IDD_PIC_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
public:
    afx_msg void OnBnClickedCancel();
public:
    afx_msg void OnBnClickedButton1();
};
