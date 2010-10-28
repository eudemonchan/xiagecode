// TimberWolfDlg.cpp : 实现文件
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

#define MEMBER_LIST_HEADER_COUNT 2
#define MEMBER_LISTHEADER_CAPTION_USERNAME  L"用户名"
#define MEMBER_LISTHEADER_CAPTION_IP        L"  IP  "



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTimberWolfDlg 对话框




CTimberWolfDlg::CTimberWolfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimberWolfDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimberWolfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTimberWolfDlg, CDialog)
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
END_MESSAGE_MAP()


// CTimberWolfDlg 消息处理程序

BOOL CTimberWolfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
    if (!InitCtrl())
    {
        ::MessageBoxW(m_hWnd, L"初始化界面失败！", L"错误", MB_ICONERROR | MB_TOPMOST);
        CDialog::OnCancel();        
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

    // 设置扩展风格
    DWORD dwStyle = plcMember->GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    plcMember->SetExtendedStyle(dwStyle); 

    plcMember->InsertItem(0, L"");
    plcMember->SetItemText(0, 0, L"刘德华");
    plcMember->SetItemText(0, 1, L"127.0.0.1");

    plcMember->InsertItem(1, L"");
    plcMember->SetItemText(1, 0, L"李连杰");
    plcMember->SetItemText(1, 1, L"127.0.0.1");

    plcMember->InsertItem(2, L"");
   plcMember->SetItemText(2, 0, L"苍井空");
    plcMember->SetItemText(2, 1, L"127.0.0.1");

    plcMember->InsertItem(3, L"");
   plcMember->SetItemText(3, 0, L"小泉彩");
    plcMember->SetItemText(3, 1, L"127.0.0.1");

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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimberWolfDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTimberWolfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTimberWolfDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    //OnOK();
}

void CTimberWolfDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    OnCancel();
}

void CTimberWolfDlg::OnBnClickedMain()
{    
    CWnd* pbtnMain = GetDlgItem(IDC_MAIN);
    assert(pbtnMain != NULL);

    RECT rcBtn = { 0 };
    pbtnMain->GetWindowRect(&rcBtn);

    // 动态创建菜单
    CMenu menuPopup;
    menuPopup.CreatePopupMenu();

    menuPopup.AppendMenu(MF_BYPOSITION, 1, _T("选项 ..."));
    menuPopup.AppendMenu(MF_BYPOSITION, 2, _T("帮助 ..."));
    menuPopup.AppendMenu(MF_SEPARATOR);
    menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("退出"));
    
    /*menuPopup.AppendMenu(MF_SEPARATOR);
    menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("cancel"));*/

    int nCmd = (int)menuPopup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, rcBtn.left, rcBtn.bottom, this);
    switch (nCmd)
    {
    case 1: // 写成宏
        AfxMessageBox(L"选项");
        break;

    case 2:
        AfxMessageBox(L"帮助");        
        break;

    case 3:
        AfxMessageBox(L"退出");
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
    CPicListDlg dlgPicList;
    dlgPicList.DoModal();
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

        menuPopup.AppendMenu(MF_BYPOSITION, 1, _T("私聊"));
        menuPopup.AppendMenu(MF_SEPARATOR);
        menuPopup.AppendMenu(MF_BYPOSITION, 2, _T("发送文件"));
        menuPopup.AppendMenu(MF_BYPOSITION, 3, _T("发送文件夹"));

        int nCmd = (int)menuPopup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptCurr.x, ptCurr.y, this);
        switch (nCmd)
        {
        case 1: // 写成宏
            AfxMessageBox(L"私聊");
            break;

        case 2:
            AfxMessageBox(L"发送文件");        
            break;

        case 3:
            AfxMessageBox(L"发送文件夹");
            break;
        }
    }

    *pResult = 0;
}
