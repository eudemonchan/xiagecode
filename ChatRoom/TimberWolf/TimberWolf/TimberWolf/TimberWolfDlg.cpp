// TimberWolfDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "TimberWolfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <assert.h>

#include "FileShareDlg.h"
#include "PicListDlg.h"
#include "SettingDlg.h"

#define MEMBER_LIST_HEADER_COUNT 2
#define MEMBER_LISTHEADER_CAPTION_USERNAME  L"�û���"
#define MEMBER_LISTHEADER_CAPTION_IP        L"  IP  "



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTimberWolfDlg �Ի���




CTimberWolfDlg::CTimberWolfDlg(CWnd* pParent /*=NULL*/)
	: /*CDialog*/ClxDialog(CTimberWolfDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pdlgPictureList = NULL;
}

void CTimberWolfDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_STATIC_SEPARATOR, m_staticSeparator);
}

BEGIN_MESSAGE_MAP(CTimberWolfDlg, ClxDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CTimberWolfDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CTimberWolfDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_MAIN, &CTimberWolfDlg::OnBnClickedMain)
    ON_BN_CLICKED(IDC_FILE_SHARE, &CTimberWolfDlg::OnBnClickedFileShare)
    ON_BN_CLICKED(IDC_SEND_MSG, &CTimberWolfDlg::OnBnClickedSendMsg)
    ON_BN_CLICKED(IDC_EXIT, &CTimberWolfDlg::OnBnClickedExit)
    ON_BN_CLICKED(IDC_SEND_PIC, &CTimberWolfDlg::OnBnClickedSendPic)
    ON_BN_CLICKED(IDC_SET_FONT, &CTimberWolfDlg::OnBnClickedSetFont)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_MEMBER, &CTimberWolfDlg::OnNMRclickListMember)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(IDC_PANEL_SEPARATOR, &CTimberWolfDlg::OnBnClickedSeparator)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTimberWolfDlg ��Ϣ�������

BOOL CTimberWolfDlg::OnInitDialog()
{
	/*CDialog*/ClxDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
    if (!InitCtrl())
    {
        ::MessageBoxW(m_hWnd, L"��ʼ������ʧ�ܣ�", L"����", MB_ICONERROR | MB_TOPMOST);
        CDialog::OnCancel();        
    }

    m_pdlgPictureList = new CPicListDlg();
    m_pdlgPictureList->Create(IDD_PIC_LIST, this);
    //m_pdlgPictureList->ShowWindow(SW_HIDE);
    ::SetWindowPos(m_pdlgPictureList->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CTimberWolfDlg::InitCtrl(void)
{
    CListCtrl* plcMember = (CListCtrl*)GetDlgItem(IDC_LIST_MEMBER);
    assert(plcMember != NULL);

    WCHAR* pwzListHeader[MEMBER_LIST_HEADER_COUNT] = 
    { 
        MEMBER_LISTHEADER_CAPTION_USERNAME,
        MEMBER_LISTHEADER_CAPTION_IP 
    };

    for (int i = 0; i < MEMBER_LIST_HEADER_COUNT; i++)
    {
        plcMember->InsertColumn(i, pwzListHeader[i], LVCFMT_LEFT, 80);
    }

    // ������չ���
    DWORD dwStyle = plcMember->GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    plcMember->SetExtendedStyle(dwStyle); 

    plcMember->InsertItem(0, L"");
    plcMember->SetItemText(0, 0, L"���»�");
    plcMember->SetItemText(0, 1, L"127.0.0.1");

    plcMember->InsertItem(1, L"");
    plcMember->SetItemText(1, 0, L"������");
    plcMember->SetItemText(1, 1, L"127.0.0.1");

    plcMember->InsertItem(2, L"");
    plcMember->SetItemText(2, 0, L"�Ծ���");
    plcMember->SetItemText(2, 1, L"127.0.0.1");

    plcMember->InsertItem(3, L"");
    plcMember->SetItemText(3, 0, L"СȪ��");
    plcMember->SetItemText(3, 1, L"127.0.0.1");


    static DLGCTLINFO dcMenuGroup[] = 
    {
        { IDC_FILE_SHARE, RLT_R, 100 },
        { IDC_RICHEDIT_GROUP, RLT_LTRB, 100 },
        { IDC_RICHEDIT_PRIVATE, RLT_LRB, 100 },
        { IDC_RICHEDIT_SEND, RLT_LRB, 100 },

        { IDC_SEND_MSG, RLT_RB, 100 },
        { IDC_EXIT, RLT_RB, 100 },

        { IDC_PANEL_SEPARATOR, RLT_TRB, 100 },
        { IDC_RICHEDIT_PUBLIC, RLT_R, 100 },
        { IDC_LIST_MEMBER, RLT_TRB, 100 },

        { IDC_STATIC_PUBLIC, RLT_R, 100 },
        { IDC_STATIC_MEMBER_LIST, RLT_R, 100 },

        { IDC_STATIC_YOU_TOWARD, RLT_LB, 100 },
        { IDC_STATIC_SAY, RLT_LB, 100 },

        { IDC_COMBO_SELECT_CHAT, RLT_LB, 100 },
        { IDC_STATIC_SCROLL_AD, RLT_LB, 100 },

        { IDC_SEND_PIC, RLT_LB, 100 },
        { IDC_SET_FONT, RLT_LB, 100 }
    };

    __super::SetControlProperty(dcMenuGroup, sizeof(dcMenuGroup) / sizeof(DLGCTLINFO));
    //__super::Set

    return TRUE;
}

void CTimberWolfDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTimberWolfDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CDialog*/ClxDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTimberWolfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTimberWolfDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //OnOK();
}

void CTimberWolfDlg::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnCancel();
}

void CTimberWolfDlg::OnBnClickedMain()
{    
    CWnd* pbtnMain = GetDlgItem(IDC_MAIN);
    assert(pbtnMain != NULL);

    RECT rcBtn = { 0 };
    pbtnMain->GetWindowRect(&rcBtn);

    // ��̬�����˵�
    CMenu menuPopup;
    menuPopup.CreatePopupMenu();

    menuPopup.AppendMenu(MF_BYPOSITION, 1, _T("ѡ�� ..."));
    menuPopup.AppendMenu(MF_BYPOSITION, 2, _T("���� ..."));
    menuPopup.AppendMenu(MF_SEPARATOR);
    menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("�˳�"));
    
    /*menuPopup.AppendMenu(MF_SEPARATOR);
    menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("cancel"));*/

    int nCmd = (int)menuPopup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, rcBtn.left, rcBtn.bottom, this);
    switch (nCmd)
    {
    case 1: // д�ɺ�
        {
            //AfxMessageBox(L"ѡ��");
            CSettingDlg dlgSetting;
            if (dlgSetting.DoModal() == IDOK)
            {
                AfxMessageBox(L"�����ѱ��棡");
            }
        }
        break;

    case 2:
        AfxMessageBox(L"����");        
        break;

    case 3:
        //AfxMessageBox(L"�˳�");
        PostQuitMessage(0);
        break;
    }
}

void CTimberWolfDlg::OnBnClickedFileShare()
{
    CFileShareDlg dlgFileShare;
    dlgFileShare.DoModal();
}

void CTimberWolfDlg::OnBnClickedSendMsg()
{
    // TODO: Add your control notification handler code here
}

void CTimberWolfDlg::OnBnClickedExit()
{
    // TODO: Add your control notification handler code here
}

void CTimberWolfDlg::OnBnClickedSendPic()
{
//     CPicListDlg dlgPicList;
//     dlgPicList.DoModal();
    assert(m_pdlgPictureList != NULL);

    RECT rcList = { 0 };
    m_pdlgPictureList->GetWindowRect(&rcList);

    CWnd* pbtn = GetDlgItem(IDC_SEND_PIC);
    assert(pbtn != NULL);

    RECT rcSendPicBtn = { 0 };
    pbtn->GetWindowRect(&rcSendPicBtn);
    ScreenToClient(&rcSendPicBtn);

    int nleft = rcSendPicBtn.left;
    int nTop = rcSendPicBtn.top - RECT_H(rcList);

    //::SetWindowPos(m_pdlgPictureList->m_hWnd, NULL, nleft, nTop, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
    m_pdlgPictureList->MoveWindow(CRect(nleft, nTop, RECT_W(rcList), RECT_H(rcList)));
    m_pdlgPictureList->ShowWindow(SW_SHOW);
}

void CTimberWolfDlg::OnBnClickedSetFont()
{
    CFontDialog dlgSetFont;
    dlgSetFont.DoModal();
}

void CTimberWolfDlg::OnNMRclickListMember(NMHDR *pNMHDR, LRESULT *pResult)
{
//     // The pointer to my list view control.
//     extern CListCtrl* pmyListCtrl;
//     // The pointer where the mouse was clicked.
//     extern CPoint myPoint;

    CListCtrl* plcMember = (CListCtrl*)GetDlgItem(IDC_LIST_MEMBER);
    assert(plcMember != NULL);

    POINT ptCurr = { 0 };
    GetCursorPos(&ptCurr);

    POINT ptClient = ptCurr;
    plcMember->ScreenToClient(&ptClient);

    // Select the item the user clicked on.
    UINT uFlags;
    int nItem = plcMember->HitTest(ptClient, &uFlags);
    if (nItem == -1)
    {
        *pResult = 0;
        return;
    }

    if (uFlags & LVHT_ONITEMLABEL)
    {
        plcMember->SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
            LVIS_SELECTED, 0);

        CMenu menuPopup;
        menuPopup.CreatePopupMenu();

        menuPopup.AppendMenu(MF_BYPOSITION, 1, _T("˽��"));
        menuPopup.AppendMenu(MF_SEPARATOR);
        menuPopup.AppendMenu(MF_BYPOSITION, 2, _T("�����ļ�"));
        menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("�����ļ���"));

        int nCmd = (int)menuPopup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptCurr.x, ptCurr.y, this);
        switch (nCmd)
        {
        case 1: // д�ɺ�
            AfxMessageBox(L"˽��");
            break;

        case 2:
            AfxMessageBox(L"�����ļ�");        
            break;

        case 3:
            AfxMessageBox(L"�����ļ���");
            break;
        }
    }

    *pResult = 0;
}

static BOOL s_bExpanded = TRUE;

void CTimberWolfDlg::OnSize(UINT nType, int cx, int cy)
{
    ClxDialog::OnSize(nType, cx, cy);
}

BOOL CTimberWolfDlg::OnEraseBkgnd(CDC* pDC)
{    
    return ClxDialog::OnEraseBkgnd(pDC);
}

void CTimberWolfDlg::OnBnClickedSeparator()
{
    static int s_nOffset = 0;


    /*
    if (s_nOffset == 0)
    {
        RECT rcDlg = { 0 };
        GetClientRect(&rcDlg);

        CWnd* pbtnSeparator = GetDlgItem(IDC_PANEL_SEPARATOR);
        assert(pbtnSeparator != NULL);

        RECT rcSeparator = { 0 };
        pbtnSeparator->GetWindowRect(&rcSeparator);
        ScreenToClient(&rcSeparator);
        
        s_nOffset = rcDlg.right - rcSeparator.right;
    }

//      ModifyStyle(WS_SIZEBOX, 1);
// 
//     LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
//     if (lStyle & WS_THICKFRAME) 
//         SetWindowLong(m_hWnd, GWL_STYLE, lStyle & ~WS_THICKFRAME);

    WINDOWPLACEMENT lpwndpl = { 0 };
    GetWindowPlacement(&lpwndpl); 

    if (s_bExpanded)
    {
        lpwndpl.rcNormalPosition.right -= s_nOffset;
        s_bExpanded = FALSE;
    }
    else
    {
        lpwndpl.rcNormalPosition.right += s_nOffset;        
        s_bExpanded = TRUE;
    }

    SetWindowPlacement(&lpwndpl);

    return;

//      ModifyStyle(1, WS_SIZEBOX);
//     lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
//     if (!(lStyle & WS_THICKFRAME))
//         SetWindowLong(m_hWnd, GWL_STYLE, lStyle | WS_THICKFRAME);
    */

}

HBRUSH CTimberWolfDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = ClxDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    switch (pWnd->GetDlgCtrlID()) 
    { 
    case IDC_STATIC_YOU_TOWARD:
    case IDC_STATIC_SAY:
    case IDC_STATIC_SCROLL_AD:
    case IDC_STATIC_PUBLIC:
    case IDC_STATIC_MEMBER_LIST:
//         pDC->SetBkMode(TRANSPARENT); 
//         return (HBRUSH)GetStockObject(HOLLOW_BRUSH); 

    default: 
        break; 
    } 

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}
