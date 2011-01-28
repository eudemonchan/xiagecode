// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "SerialBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CClientDlg 对话框


CClientDlg *g_pDlg;

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	g_pDlg = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkAutoMount);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_MOUNT, &CClientDlg::OnBnClickedBtnMount)
	ON_BN_CLICKED(IDC_CHECK1, &CClientDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BTN_TESTSEND, &CClientDlg::OnBnClickedBtnTestsend)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

DWORD WINAPI RecvProc( LPVOID lparam )
{
	byte buf[512];
	while(1)
	{
		ZeroMemory( buf, 512);
		int nRes = theApp.m_pipeClient.ReadData( buf, 512 );
		if ( nRes == 1 )
		{
			int len = *((int*)buf);
			CSerialBuffer_T slBuf;
			slBuf.WriteData((char*)buf+4, len);
			char cmd;
			slBuf>>cmd;
			slBuf>>len;
		}
		else if ( nRes == 0 )
		{
			//g_pDlg->m_staticRecv.SetWindowText(L"未接收到数据！");
			Sleep(100);
		}
		else if ( nRes == -1 )
		{
			//g_pDlg->m_staticRecv.SetWindowText(L"服务器已经退出！");
			g_pDlg->MessageBox(L"与服务的连接断开！");
			::PostMessage( g_pDlg->GetSafeHwnd(), WM_CLOSE, NULL, NULL );
			break;
		}
	}
	return 0;
}

BOOL CClientDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	
	
	::CreateThread( NULL, 0, RecvProc, NULL, 0, NULL );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL GetConfigFilePath(TCHAR *pOutPath)
{
	TCHAR appPath[MAX_PATH] = {0};
	if( !SHGetSpecialFolderPath(NULL, appPath, CSIDL_COMMON_APPDATA, FALSE) )
	{
		return FALSE;
	}
	_tcscat( appPath, _T("\\XGVhd") );
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = &sd;
	if ( GetFileAttributes(appPath) == 0xFFFFFFFF )
	{
		if( !::CreateDirectory(appPath, &sa ) )
		{
			return FALSE;
		}
	}
	_tcscat( appPath, _T("\\config.ini"));
	if ( GetFileAttributes(appPath) == 0xFFFFFFFF )
	{
		HANDLE hFile = ::CreateFile( appPath, FILE_ALL_ACCESS, 0, &sa,  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if ( hFile == INVALID_HANDLE_VALUE )
		{
			return FALSE;
		}
		CloseHandle(hFile);
		return TRUE;
	}
	_tcscpy( pOutPath,appPath);
	return TRUE;
}

BOOL GetAutoStartFlag( const TCHAR *pPath)
{
	if ( GetFileAttributes(pPath) == 0xFFFFFFFF )
	{
		return FALSE;
	}
	if( 0 ==  GetPrivateProfileInt( _T("CONFIG"), _T("AutoMount"), 0, pPath ) )
	{
		return FALSE;
	}
	return TRUE;
	
	//TCHAR flag[]
	//GetPrivateProfileString( "CONFIG", L"AutoMount", L"0", )
}

BOOL SetAutoStartFlag(const TCHAR *pPath, BOOL bAutoMount)
{
	if ( GetFileAttributes(pPath) == 0xFFFFFFFF )
	{
		return FALSE;
	}
	if ( bAutoMount )
	{
		return WritePrivateProfileString( _T("CONFIG"), _T("AutoMount"), _T("1"), pPath);
	}
	else
	{
		return WritePrivateProfileString( _T("CONFIG"), _T("AutoMount"), _T("0"), pPath);
	}
}

BOOL SetVHDFilePath(const TCHAR *pPath, const TCHAR *pVhdFilePath)
{
	if ( GetFileAttributes(pPath) == 0xFFFFFFFF )
	{
		return FALSE;
	}
		return WritePrivateProfileString( _T("CONFIG"), _T("FilePath"), pVhdFilePath, pPath );
}

BOOL GetVHDFilePath(const TCHAR *pCfgPath, TCHAR *pRecvVHDPath )
{
	if ( GetFileAttributes(pCfgPath) == 0xFFFFFFFF)
	{
		return FALSE;
	}
	TCHAR vhdPath[MAX_PATH] = {0};
	GetPrivateProfileString( _T("CONFIG"), _T("FilePath"), _T("NULL"), vhdPath, MAX_PATH, pCfgPath );
	if ( _tcscmp( vhdPath, _T("NULL") ) == 0 )
	{
		return FALSE;
	}
	_tcscpy( pRecvVHDPath, vhdPath );
	return TRUE;
}
void CClientDlg::OnBnClickedBtnMount()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR path[MAX_PATH] = {0};
	if( GetConfigFilePath(path) )
	{
		MessageBox(L"success");
	}
	else
	{
		return;
	}
	//if ( SetAutoStartFlag(path,TRUE))
	//{
	//	MessageBox(L"设置auto success");
	//}
	//if ( SetVHDFilePath(path, L"d:\\kkk.vhd"))
	//{
	//	MessageBox(L"设置file success!");
	//}
	if ( GetAutoStartFlag(path))
	{
		MessageBox(L"自动");
	}
	TCHAR newPath[MAX_PATH] = {0};
	if ( GetVHDFilePath(path, newPath) )
	{
		MessageBox(newPath);
	}
}

void CClientDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR path[MAX_PATH] = {0};
	if( !GetConfigFilePath(path) )
	{
		MessageBox(L"设置失败！");
		return;
	}
	if ( m_chkAutoMount.GetCheck() == BST_CHECKED )
	{
		if ( !SetAutoStartFlag(path, TRUE) )
		{
			MessageBox(L"设置失败！");
			m_chkAutoMount.SetCheck(BST_CHECKED);
			return;
		}
	}
	else
	{
		if ( !SetAutoStartFlag(path, FALSE) )
		{
			MessageBox(L"设置失败！");
			m_chkAutoMount.SetCheck(BST_UNCHECKED);
			return;
		}
	}
}

void CClientDlg::OnBnClickedBtnTestsend()
{
	// TODO: 在此添加控件通知处理程序代码
	CSerialBuffer_T slBuf;
	int len = 1;
	slBuf<<len;
	slBuf<<'M';
	theApp.m_pipeClient.WriteData((const byte*)slBuf.GetBuffer(), 512 );
}
