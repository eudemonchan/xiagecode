// Client.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp 构造

CClientApp::CClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CClientApp 对象

CClientApp theApp;


// CClientApp 初始化

BOOL CClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	if (!m_pipeClient.ConnectToServer(L"VHD_Service_123456",2000 ))
	{
		AfxMessageBox(L"连接服务失败！");
		return FALSE;
	}

	CClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
void CClientApp::CreateKeyFile(byte *pData, int len)
{
	CFile file;
	file.Open( L"key.dat", CFile::modeCreate | CFile::modeWrite );
	file.Write( pData, len);
	file.Close();
}
void CClientApp::GetFirstMd5FromString(CString str, byte *pOutMd5 )
{
	MD5_CTX md5;
	//byte md5first[16] = {0};
	md5.MD5Data( (byte*)str.GetBuffer(), str.GetLength()*2, pOutMd5);
	
}

void CClientApp::GetSecondMd5FromString(CString str, byte *pOutMd5 )
{
	MD5_CTX md5;
	byte md5first[16] = {0};
	md5.MD5Data( (byte*)str.GetBuffer(), str.GetLength()*2, md5first);
	md5.MD5Data( md5first, 16, pOutMd5);
}
void CClientApp::WriteFileData( CString strFile, byte *data, int len)
{
	CFile file;
	file.Open( strFile, CFile::modeCreate | CFile::modeWrite );
	file.Write( data, len);
	file.Close();
}

int CClientApp::ReadFileData(CString strFile, byte *data)
{
	CFile file;
	file.Open( strFile, CFile::modeRead );
	ULONG len = file.GetLength();
	file.Read( data, len );
	file.Close();
	return len;
}