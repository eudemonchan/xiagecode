#pragma once


// CFileShareDlg dialog

class CFileShareDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileShareDlg)

public:
	CFileShareDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileShareDlg();

// Dialog Data
	enum { IDD = IDD_FILE_SHARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedShare();
public:
    afx_msg void OnBnClickedDeleteShare();
};
