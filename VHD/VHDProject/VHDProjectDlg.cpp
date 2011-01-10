
// VHDProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VHDProject.h"
#include "VHDProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVHDProjectDlg dialog




CVHDProjectDlg::CVHDProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVHDProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVHDProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_proCtrl);
}

BEGIN_MESSAGE_MAP(CVHDProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CVHDProjectDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVHDProjectDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_CREATE, &CVHDProjectDlg::OnBnClickedBtnCreate)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVHDProjectDlg message handlers

BOOL CVHDProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_proCtrl.SetRange(0,100);
	m_proCtrl.SetStep(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVHDProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVHDProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVHDProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString GetVHDFilePath()
{
	WCHAR path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path,MAX_PATH);
	WCHAR * pItalic = wcsrchr( path, L'\\');
	if ( pItalic == NULL )
	{
		return L"";
	}
	*pItalic = L'\0';
	CString strTemp(path);
	strTemp += L"\\vhd.ini";
	if ( GetFileAttributes( (LPCTSTR)strTemp) == 0xFFFFFFFF )
	{
		return L"";
	}
	CStdioFile file;
	if ( !file.Open( strTemp, CFile::modeRead ) )
	{
		return L"";
	}
	CString strkk;
	file.ReadString(strkk);
	if ( strkk == L"" )
	{
		file.Close();
		return L"";
	}
	file.Close();
	return strkk;
}

void CVHDProjectDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code h
	//CString strPath = GetVHDFilePath();
	//if ( strPath.IsEmpty() )
	//{
	//	MessageBox(L"������vhd�ļ�·����");
	//	return;
	//}
	//if( ERROR_SUCCESS != m_vhd.Open(strPath) )
	//{
	//	MessageBox(L"Openʧ�ܣ�");
	//	return;
	//}
	//if ( ERROR_SUCCESS != m_vhd.Attach() )
	//{
	//	MessageBox(L"attachʧ�ܣ�");
	//	m_vhd.Close();
	//	return;
	//}
	//MessageBox(L"�ɹ�!");
	if( m_vhd.Open(L"d:\\1g.vhd") )
	{
		if( m_vhd.Attach() )
		{
			return;
		}
	}
	MessageBox(L"ʧ��");
}


void CVHDProjectDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	CString strPath = GetVHDFilePath();
	if ( strPath.IsEmpty() )
	{
		MessageBox(L"������vhd�ļ�·����");
		return;
	}
	if ( ERROR_SUCCESS != m_vhd.Detach() )
	{
		
		return;
	}
	MessageBox(L"�ɹ�!");
}

//DWORD WINAPI CreateProc(LPVOID lParam)
//{
//	CVHDProjectDlg *pDlg = (CVHDProjectDlg*)lParam;
//	ULONGLONG i64 = 1024*1024;
//	i64 *= 1024;
//	i64 *= 8;
//
//	pDlg->m_ol.hEvent = NULL;
//	ZeroMemory( &pDlg->m_ol, sizeof(OVERLAPPED));
//	pDlg->m_ol.Internal = STATUS_PENDING;
//	pDlg->m_ol.hEvent = ::CreateEventA( NULL, TRUE, FALSE, NULL );
//	//::CreateThread( NULL, 0, CreateProc, this, 0, NULL );
//	if(ERROR_SUCCESS == pDlg->m_vhd.CreateFixed(L"d:\\1111.vhd", i64, VIRTUAL_DISK_ACCESS_CREATE, NULL, NULL, &pDlg->m_ol ) )
//	{
//		AfxMessageBox(L"����ʧ�ܣ�");
//	}
//	else
//	{
//		CString str;
//		str.Format(L"%d", GetLastError());
//		AfxMessageBox(str);
//	}
//	pDlg->m_vhd.Close();
//	pDlg->MessageBox(L"������ϣ�");
//	return 0;
//}

void CVHDProjectDlg::OnBnClickedBtnCreate()
{
	// TODO: Add your control notification handler code here
	//ULONGLONG i64 = 1024*1024;
	//i64 *= 1024;
	//i64 *= 8;
	//
	//m_ol.hEvent = NULL;
	////::CreateThread( NULL, 0, CreateProc, this, 0, NULL );
	//m_ol.hEvent = NULL;
	//ZeroMemory( &m_ol, sizeof(OVERLAPPED));
	//m_ol.Internal = STATUS_PENDING;
	//if(ERROR_SUCCESS == m_vhd.CreateFixed(L"d:\\1111.vhd", i64, VIRTUAL_DISK_ACCESS_ALL, NULL, NULL, &m_ol ) )
	//{
	//	MessageBox(L"����ʧ�ܣ�");
	//}
	//else
	//{
	//	CString str;
	//	str.Format(L"%d", GetLastError());
	//	MessageBox(str);
	//}
	ULONGLONG i64 = 1024*1024;
	i64 *= 1024;
	i64 *= 8;

	ULARGE_INTEGER ulAvailSpace; 
	GetDiskFreeSpaceEx( L"d:\\", &ulAvailSpace, NULL, NULL );
	if ( ulAvailSpace.QuadPart < (i64 + (ULONGLONG)(1024*1024*10) ) )
	{
		MessageBox(L"���̿ռ䲻��!");
		return;
	}
	if( !m_vhd.CreateFixedAsync(L"d:\\1g.vhd",i64) )
	{
		CString str;
		str.Format(L"�������󣬴����룺%d", m_vhd.GetLastErrorCode() );
		MessageBox(L"����ʧ�ܣ�");
		return;
	}
	SetTimer(1,500, NULL);
}


void CVHDProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	/*SetDlgItemInt( IDC_STATIC_STATE, m_ol.Internal);
	SetDlgItemInt( IDC_STATIC_SECOND, m_ol.Offset);*/
	//VIRTUAL_DISK_PROGRESS process;
	//if( ERROR_SUCCESS != GetVirtualDiskOperationProgress(m_vhd.m_h, &m_ol, &process) )
	//{
	//	CString strKk;
	//	strKk.Format(L"%d", GetLastError());
	//	MessageBox(strKk);
	//}
	VIRTUAL_DISK_PROGRESS process;
	if ( m_vhd.GetProcessState(&process))
	{
		if ( process.OperationStatus == ERROR_IO_PENDING )
		{
			double kk = ((double)process.CurrentValue/(double)process.CompletionValue);
			int abc = (int)(kk*100.0);
			m_proCtrl.SetPos(abc);
		}
		else if ( process.OperationStatus == ERROR_SUCCESS )
		{
			KillTimer(1);
			m_proCtrl.SetPos(100);
			MessageBox(L"�ɹ���");
		}
	}

	//SetDlgItemInt(IDC_STATIC_STATE, process.CurrentValue);
	//SetDlgItemInt(IDC_STATIC_SECOND, process.CompletionValue);
	CDialogEx::OnTimer(nIDEvent);
}
