// PicListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "PicListDlg.h"


// CPicListDlg dialog

IMPLEMENT_DYNAMIC(CPicListDlg, CDialog)

CPicListDlg::CPicListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicListDlg::IDD, pParent)
{

}

CPicListDlg::~CPicListDlg()
{
}

void CPicListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicListDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CPicListDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CPicListDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, &CPicListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPicListDlg message handlers

void CPicListDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //OnOK();
}

void CPicListDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    //OnCancel();
}

void CPicListDlg::OnBnClickedButton1()
{
    OnOK();
}
