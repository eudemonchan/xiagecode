// LogonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "LogonDlg.h"


// CLogonDlg �Ի���

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


// CLogonDlg ��Ϣ�������


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

    // ��ʼΪ�ö�����
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, nLeft, nTop, RECT_W(rcDlg), RECT_H(rcDlg), SWP_HIDEWINDOW);
    */

    // Load user config...

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CLogonDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialog::OnClose();
}

void CLogonDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnOK();
}
