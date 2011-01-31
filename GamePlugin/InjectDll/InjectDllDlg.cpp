
// InjectDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InjectDll.h"
#include "InjectDllDlg.h"
#include "Tlhelp32.h"
#pragma comment(lib,"Kernel32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define InjectLib InjectLibW
#define EjectLib  EjectLibW
#else
#define InjectLib InjectLibA
#define EjectLib  EjectLibA
#endif   // !UNICODE


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


// CInjectDllDlg 对话框




CInjectDllDlg::CInjectDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInjected = FALSE;
}

void CInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DLLPATH, m_editDllPath);
	DDX_Control(pDX, IDC_EDIT_PRONAME, m_editProName);
}

BEGIN_MESSAGE_MAP(CInjectDllDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_INJECT, &CInjectDllDlg::OnBnClickedBtnInject)
	ON_BN_CLICKED(IDC_BTN_EJECT, &CInjectDllDlg::OnBnClickedBtnEject)
END_MESSAGE_MAP()


// CInjectDllDlg 消息处理程序

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_editDllPath.SetWindowText(L"E:\\MyCode\\DetTestHook\\Debug\\DetTestHook.dll");
	m_editProName.SetWindowText(L"testDetApp.exe");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInjectDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInjectDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) {

	BOOL fOk = FALSE; // Assume that the function fails
	HANDLE hProcess = NULL, hThread = NULL;
	PWSTR pszLibFileRemote = NULL;

	__try {
		// Get a handle for the target process.
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
			PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
			PROCESS_VM_WRITE,             // For WriteProcessMemory
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;

		// Calculate the number of bytes needed for the DLL's pathname
		int cch = 1 + lstrlenW(pszLibFile);
		int cb  = cch * sizeof(WCHAR);

		// Allocate space in the remote process for the pathname
		pszLibFileRemote = (PWSTR) 
			VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if (pszLibFileRemote == NULL) __leave;

		// Copy the DLL's pathname to the remote process's address space
		if (!WriteProcessMemory(hProcess, pszLibFileRemote, 
			(PVOID) pszLibFile, cb, NULL)) __leave;

		// Get the real address of LoadLibraryW in Kernel32.dll
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		if (pfnThreadRtn == NULL) __leave;

		// Create a remote thread that calls LoadLibraryW(DLLPathname)
		hThread = CreateRemoteThread(hProcess, NULL, 0, 
			pfnThreadRtn, pszLibFileRemote, 0, NULL);
		if (hThread == NULL) __leave;

		// Wait for the remote thread to terminate
		WaitForSingleObject(hThread, INFINITE);

		fOk = TRUE; // Everything executed successfully
	}
	__finally { // Now, we can clean everthing up

		// Free the remote memory that contained the DLL's pathname
		if (pszLibFileRemote != NULL) 
			VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);

		if (hThread  != NULL) 
			CloseHandle(hThread);

		if (hProcess != NULL) 
			CloseHandle(hProcess);
	}

	return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {

	// Allocate a (stack) buffer for the Unicode version of the pathname
	PWSTR pszLibFileW = (PWSTR) 
		_alloca((lstrlenA(pszLibFile) + 1) * sizeof(WCHAR));

	// Convert the ANSI pathname to its Unicode equivalent
	wsprintfW(pszLibFileW, L"%S", pszLibFile);

	// Call the Unicode version of the function to actually do the work.
	return(InjectLibW(dwProcessId, pszLibFileW));
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) {

	BOOL fOk = FALSE; // Assume that the function fails
	HANDLE hthSnapshot = NULL;
	HANDLE hProcess = NULL, hThread = NULL;

	__try {
		// Grab a new snapshot of the process
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if (hthSnapshot == NULL) __leave;

		// Get the HMODULE of the desired library
		MODULEENTRY32W me = { sizeof(me) };
		BOOL fFound = FALSE;
		BOOL fMoreMods = Module32FirstW(hthSnapshot, &me);
		for (; fMoreMods; fMoreMods = Module32NextW(hthSnapshot, &me)) {
			fFound = (lstrcmpiW(me.szModule,  pszLibFile) == 0) || 
				(lstrcmpiW(me.szExePath, pszLibFile) == 0);
			if (fFound) break;
		}
		if (!fFound) __leave;

		// Get a handle for the target process.
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD     | 
			PROCESS_VM_OPERATION,  // For CreateRemoteThread
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;

		// Get the real address of LoadLibraryW in Kernel32.dll
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
		if (pfnThreadRtn == NULL) __leave;

		// Create a remote thread that calls LoadLibraryW(DLLPathname)
		hThread = CreateRemoteThread(hProcess, NULL, 0, 
			pfnThreadRtn, me.modBaseAddr, 0, NULL);
		if (hThread == NULL) __leave;

		// Wait for the remote thread to terminate
		WaitForSingleObject(hThread, INFINITE);

		fOk = TRUE; // Everything executed successfully
	}
	__finally { // Now we can clean everything up

		if (hthSnapshot != NULL) 
			CloseHandle(hthSnapshot);

		if (hThread     != NULL) 
			CloseHandle(hThread);

		if (hProcess    != NULL) 
			CloseHandle(hProcess);
	}

	return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI EjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {

	// Allocate a (stack) buffer for the Unicode version of the pathname
	PWSTR pszLibFileW = (PWSTR) 
		_alloca((lstrlenA(pszLibFile) + 1) * sizeof(WCHAR));

	// Convert the ANSI pathname to its Unicode equivalent
	wsprintfW(pszLibFileW, L"%S", pszLibFile);

	// Call the Unicode version of the function to actually do the work.
	return(EjectLibW(dwProcessId, pszLibFileW));
}

DWORD GetProcessIDFromName(LPCTSTR processName)
{
	BOOL bLoop;
	unsigned long PID;
	HANDLE hSnapshot;
	PROCESSENTRY32 pEntry32;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pEntry32.dwSize = sizeof(PROCESSENTRY32);
	bLoop = Process32First(hSnapshot, &pEntry32);
	while (bLoop)
	{    
		PID = pEntry32.th32ProcessID;
		if ( _tcsicmp(processName, pEntry32.szExeFile) == 0 )
		{
			CloseHandle(hSnapshot);
			return PID;
		}
		bLoop = Process32Next(hSnapshot, &pEntry32);
	}
	CloseHandle(hSnapshot);
	return 0;
}

void CInjectDllDlg::OnBnClickedBtnInject()
{
	// TODO: 在此添加控件通知处理程序代码
	CString dllPath;
	m_editDllPath.GetWindowText(dllPath);
	dllPath.Trim();
	if ( dllPath.IsEmpty())
	{
		MessageBox(L"请输入dll路径！");
		return;
	}
	if ( GetFileAttributes(dllPath) == 0xFFFFFFFF )
	{
		MessageBox(L"dll文件不存在！");
		return;
	}
	CString proName;
	m_editProName.GetWindowText(proName);
	proName.Trim();
	if ( proName.IsEmpty() )
	{
		MessageBox(L"请输入进程名字！");
		return;
	}
	DWORD pid = GetProcessIDFromName(proName);
	if (pid == 0)
	{
		MessageBox(L"未找到该进程！");
		return;
	}

	if( InjectLib(pid, (LPCTSTR)dllPath) )
	{
		MessageBox(L"Inject成功！");
	}
	else
	{
		MessageBox(L"Inject失败！");
	}
}

void CInjectDllDlg::OnBnClickedBtnEject()
{
	// TODO: 在此添加控件通知处理程序代码
	CString dllPath;
	m_editDllPath.GetWindowText(dllPath);
	dllPath.Trim();
	if ( dllPath.IsEmpty())
	{
		MessageBox(L"请输入dll路径！");
		return;
	}
	if ( GetFileAttributes(dllPath) == 0xFFFFFFFF )
	{
		MessageBox(L"dll文件不存在！");
		return;
	}
	CString proName;
	m_editProName.GetWindowText(proName);
	proName.Trim();
	if ( proName.IsEmpty() )
	{
		MessageBox(L"请输入进程名字！");
		return;
	}
	DWORD pid = GetProcessIDFromName(proName);
	if (pid == 0)
	{
		MessageBox(L"未找到该进程！");
		return;
	}

	if( EjectLib(pid, (LPCTSTR)dllPath) )
	{
		MessageBox(L"Eject成功！");
	}
	else
	{
		MessageBox(L"Eject失败！");
	}
}
