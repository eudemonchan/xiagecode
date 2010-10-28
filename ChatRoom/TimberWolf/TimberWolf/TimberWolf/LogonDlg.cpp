// LogonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "LogonDlg.h"


// CLogonDlg 对话框

IMPLEMENT_DYNAMIC(CLogonDlg, CDialog)


CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{

}

CLogonDlg::~CLogonDlg()
{
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDOK, &CLogonDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogonDlg 消息处理程序


BOOL CLogonDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    /*
    RECT rcParentWnd = { 0 };
    GetParent()->GetClientRect(&rcParentWnd);

    RECT rcDlg = { 0 };
    GetClientRect(&rcDlg);

    int nLeft = rcParentWnd.left + RECT_W(rcParentWnd) / 2 - RECT_W(rcDlg) / 2;
    int nTop = rcParentWnd.top + RECT_H(rcParentWnd) / 2 - RECT_H(rcDlg) / 2;

    // 初始为置顶隐藏
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, nLeft, nTop, RECT_W(rcDlg), RECT_H(rcDlg), SWP_HIDEWINDOW);
    */

    // Load user config...

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CLogonDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnClose();
}

void CLogonDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}
