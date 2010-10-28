// FileShareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "FileShareDlg.h"

#include <assert.h>

// CFileShareDlg dialog

IMPLEMENT_DYNAMIC(CFileShareDlg, CDialog)

CFileShareDlg::CFileShareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileShareDlg::IDD, pParent)
{

}

CFileShareDlg::~CFileShareDlg()
{
}

void CFileShareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileShareDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CFileShareDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_SHARE, &CFileShareDlg::OnBnClickedShare)
    ON_BN_CLICKED(IDC_DELETE_SHARE, &CFileShareDlg::OnBnClickedDeleteShare)
END_MESSAGE_MAP()


// CFileShareDlg message handlers

BOOL CFileShareDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CListCtrl* plcFileShare = (CListCtrl*)GetDlgItem(IDC_LIST_MEMBER);
    assert(plcFileShare != NULL);

    WCHAR* pwzListHeader[3] = 
    { 
        L"文件名",
        L"路径" ,
        L"共享时间"
    };

    for (int i = 0; i < 3; i++)
    {
        int nWidth = 0;
        if (i == 1) 
            nWidth = 200;
        else 
            nWidth = 80;
        plcFileShare->InsertColumn(i, pwzListHeader[i], LVCFMT_LEFT, nWidth);
    }

    // 设置扩展风格
    DWORD dwStyle = plcFileShare->GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    plcFileShare->SetExtendedStyle(dwStyle); 

    plcFileShare->InsertItem(0, L"");
    plcFileShare->SetItemText(0, 0, L"こいずみあや性感.mp4");
    plcFileShare->SetItemText(0, 1, L"C:\\Windows\\System32\\学习资料\\VC++\\こいずみあや性感.mp4");
    plcFileShare->SetItemText(0, 2, L"2012-12-12");

    plcFileShare->InsertItem(1, L"");
    plcFileShare->SetItemText(1, 0, L"苍井空1.mp4");
    plcFileShare->SetItemText(1, 1, L"C:\\Windows\\System32\\学习资料\\VC++\\苍井空1.mp4");
    plcFileShare->SetItemText(1, 2, L"2012-12-12");

    plcFileShare->InsertItem(2, L"");
    plcFileShare->SetItemText(2, 0, L"苍井空2.mp4");
    plcFileShare->SetItemText(2, 1, L"C:\\Windows\\System32\\学习资料\\VC++\\苍井空2.mp4");
    plcFileShare->SetItemText(2, 2, L"2012-12-12");

    plcFileShare->InsertItem(3, L"");
    plcFileShare->SetItemText(3, 0, L"小泉彩.mp4");
    plcFileShare->SetItemText(3, 1, L"C:\\Windows\\System32\\学习资料\\VC++\\小泉彩.mp4");
    plcFileShare->SetItemText(3, 2, L"2012-12-12");

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileShareDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //OnOK();
    AfxMessageBox(L"下载！");
}

void CFileShareDlg::OnBnClickedShare()
{
    WCHAR szFilters[] =
        L"MyType Files (*.my)|*.my|All Files (*.*)|*.*||";

    // Create an Open dialog; the default file name extension is ".my".
    CFileDialog fileDlg (TRUE, L"mp4", L"*.mp4",
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

    // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
    // returns IDOK.
    if (fileDlg.DoModal () == IDOK)
    {
        CString pathName = fileDlg.GetPathName();

        // Implement opening and reading file in here.
            //Change the window's title to the opened file's title.
            CString fileName = fileDlg.GetFileTitle ();
            AfxMessageBox(fileName);
        //SetWindowText(fileName);
    }

}

void CFileShareDlg::OnBnClickedDeleteShare()
{
    AfxMessageBox(L"删除！");
}
