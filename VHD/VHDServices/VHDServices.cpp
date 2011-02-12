// VHDServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VHDManager.h"
#include "ProcessBridge.h"
#include "SerialBuffer.h"
#include "shlobj.h"
#pragma comment(lib,"shell32.lib")
using namespace std;

VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv );
VOID WINAPI HandlerProc( DWORD fdwControl );
SERVICE_STATUS_HANDLE g_hServiceStatus;
SERVICE_STATUS g_status;

HINSTANCE g_hInst;								// current instance
int InstallService(TCHAR * pszServicePath, TCHAR * pszServiceName,TCHAR * pszDisplayName);
int UninstallService(TCHAR * pszServiceName);
//卸载驱动程序  
BOOL UnloadNTDriver( TCHAR * szSvrName );
TCHAR szServiceName[] = L"XGVHD";
TCHAR szDisplayName[] = L"VHDMountService";
CVHDManager g_manager;

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

BOOL IsMounted(const TCHAR *pPath )
{
	if ( GetFileAttributes(pPath) == 0xFFFFFFFF )
	{
		return FALSE;
	}
	if( 0 ==  GetPrivateProfileInt( _T("CONFIG"), _T("IsMounted"), 0, pPath ) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SetMounted(const TCHAR *pPath, BOOL bMounted )
{
	if ( GetFileAttributes(pPath) == 0xFFFFFFFF )
	{
		return FALSE;
	}
	if ( bMounted )
	{
		return WritePrivateProfileString( _T("CONFIG"), _T("IsMounted"), _T("1"), pPath );
	}
	else
	{
		return WritePrivateProfileString( _T("CONFIG"), _T("IsMounted"), _T("0"), pPath );
	}
	return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//unsigned char kk[16];
	//for ( int i = 0; i < 16; i++ )
	//{
	//	kk[i] = i*4;
	//}
	//WriteRegNote(kk);
	//if( ReadRegNote(kk) )
	//{
	//	printf("success");
	//}
	//
	//return 0;
	if ( argc > 2 )
	{
		return 0;
	}
	// 检测是否是安装或者卸载
	if ( argc == 2 )
	{
		TCHAR szModuleName[MAX_PATH] = {0};
		//_tcscpy( szModuleName, argv[0] );
		GetModuleFileName( NULL, szModuleName, MAX_PATH);
		if (_tcsicmp( argv[1], _T("/install")) == 0)
		{
			InstallService(szModuleName,szServiceName, szDisplayName);
		}
		else if (_tcsicmp( argv[1], _T("/uninstall")) == 0)
		{
			//UninstallService(szServiceName);
			UnloadNTDriver(szServiceName);
		}
		return 0;
	}
	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	g_status.dwCurrentState = SERVICE_STOPPED;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	SERVICE_TABLE_ENTRY st[] = {{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },{ NULL, NULL }};

	if (!::StartServiceCtrlDispatcher(st))
	{
		return -1;
	}
	return 0;
}

DWORD WINAPI RecvProc( LPVOID lparam )
{
	//OutputDebugString(L"RecvProc runn");
	byte buf[512];
	CPipeServer *pServer = (CPipeServer*)lparam;
	TCHAR path[MAX_PATH] = {0};
	int kk = -1;
	while(1)
	{
		ZeroMemory( buf, 512);
		int nRes = pServer->ReadData( buf, 512 );
		if ( nRes == 1 )
		{
			//OutputDebugString(L"收到数据！ ");
			//接收成功
			int len = *((int*)buf);
			CSerialBuffer_T serBuffer;
			serBuffer.WriteData((char*)buf + 4, len);
			char cmd;
			serBuffer>>cmd;
			switch(cmd)
			{
			case 'M':
				{
					//OutputDebugString(L"收到挂载消息！");
					ZeroMemory(path,sizeof(TCHAR));
					if ( !GetConfigFilePath(path) )
					{
						//OutputDebugString(L"GetConfigFilePath fail");
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -1;
						sendBuf<<len;
						sendBuf<<'R';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					else
					{
						//OutputDebugString(L"GetConfigFilePath suc");
					}

					if ( IsMounted(path) )
					{
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -6;
						sendBuf<<len;
						sendBuf<<'R';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					TCHAR vhdPath[MAX_PATH] = {0};
					if( !GetVHDFilePath(path, vhdPath) )
					{
						//OutputDebugString(L"GetVHDFilePath fail");
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -2;
						sendBuf<<len;
						sendBuf<<'R';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					//OutputDebugString(L"GetVHDFilePath suc");
					if ( GetFileAttributes(vhdPath) == 0xFFFFFFFF )
					{
						//OutputDebugString(L"vhdPath fail");
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -3;
						sendBuf<<len;
						sendBuf<<'R';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					//g_manager.UnMountDisk();
					//g_manager.ReleaseService();
					if ( g_manager.ConnectDiskService() )
					{
						if ( g_manager.MountDisk(vhdPath))
						{
							g_manager.SetMultipleInterface();
							//OutputDebugString(L"mount succeed!");

							SetMounted(path,TRUE);
							CSerialBuffer_T sendBuf;
							int len = 5;
							int mRes = 1;
							sendBuf<<len;
							sendBuf<<'R';
							sendBuf<<mRes;
							pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
							break;

						}
						else
						{
							//OutputDebugString(L"manager.Attach() fail!");
							g_manager.ReleaseService();
							CSerialBuffer_T sendBuf;
							int len = 5;
							int mRes = -5;
							sendBuf<<len;
							sendBuf<<'R';
							sendBuf<<mRes;
							pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
							break;
						}
					}
					else
					{
						//OutputDebugString(L"manager.Open() fail");
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -4;
						sendBuf<<len;
						sendBuf<<'R';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					
				}
				break;
			case 'U':
				{

					if( g_manager.UnMountDisk() )
					{
						//OutputDebugString(L"卸载成功！");
						g_manager.ReleaseService();
						if( GetConfigFilePath(path) )
						{
							SetMounted(path, FALSE);
						}
						
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = 1;
						sendBuf<<len;
						sendBuf<<'S';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
					else
					{
						//OutputDebugString(L"卸载失败！");
						CSerialBuffer_T sendBuf;
						int len = 5;
						int mRes = -1;
						sendBuf<<len;
						sendBuf<<'S';
						sendBuf<<mRes;
						pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
						break;
					}
				//	//OutputDebugString(L"收到卸载消息！");
				//	ZeroMemory(path,sizeof(TCHAR));
				//	if ( !GetConfigFilePath(path) )
				//	{
				//		//OutputDebugString(L"GetConfigFilePath fail");
				//		CSerialBuffer_T sendBuf;
				//		int len = 5;
				//		int mRes = -1;
				//		sendBuf<<len;
				//		sendBuf<<'S';
				//		sendBuf<<mRes;
				//		pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
				//		break;
				//	}
				///*	CVHDManager manager2;
				//	TCHAR vhdPath[MAX_PATH] = {0};
				//	GetVHDFilePath(path,vhdPath);
				//	manager2.Open(vhdPath);*/
				//	if ( ERROR_SUCCESS != manager.Detach() )
				//	{
				//		//OutputDebugString(L"卸载失败！");
				//		CSerialBuffer_T sendBuf;
				//		int len = 5;
				//		int mRes = -2;
				//		sendBuf<<len;
				//		sendBuf<<'S';
				//		sendBuf<<mRes;
				//		pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
				//		break;
				//	}
				//	//OutputDebugString(L"卸载成功！");
				//	SetMounted(path, FALSE);
				//	manager.Close();
				//	CSerialBuffer_T sendBuf;
				//	int len = 5;
				//	int mRes = 1;
				//	sendBuf<<len;
				//	sendBuf<<'S';
				//	sendBuf<<mRes;
				//	pServer->WriteData((const byte*)sendBuf.GetBuffer(), 512);
				//	break;
				}
				
				break;
			}
			//解包
		}
		else if ( nRes == 0 )
		{
			//未收到数据
			Sleep(100);
			kk++;
			if ( kk%30 == 0 )
			{
				//OutputDebugString(L"未收到数据");
			}
		}
		else
		{
			//OutputDebugString(L"连接断开！");
			break;
		}
	}
	return 0;
}
void WINAPI Notify( int code, unsigned int param1)
{
	if ( code == 1 )
	{	
		//OutputDebugString(L"NotifyRunning");
		::CreateThread( NULL, 0, RecvProc, (LPVOID)param1, 0, NULL );
	}
}

VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv )
{
	g_hServiceStatus = RegisterServiceCtrlHandler(szServiceName, HandlerProc);

	//OutputDebugString(L"进入ServiceMain");
	if(g_hServiceStatus == NULL)
	{
		return;
	}
	CPipeServer pipeServer;
	if ( !pipeServer.StartServer(L"VHD_Service_123456", Notify))
	{
		//OutputDebugString(L"启动管道监听服务失败！");
		return;
	}

	g_status.dwCurrentState = SERVICE_RUNNING;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_status.dwWin32ExitCode = 0;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;
	SetServiceStatus(g_hServiceStatus, &g_status);


	/*CVHDManager manager;

	if( manager.Open(L"H:\\201119\\myvhd.vhd") == ERROR_SUCCESS )
	{
		//OutputDebugString(L"open成功！");
		if(manager.Attach()==ERROR_SUCCESS)
		{
			//OutputDebugString(L"attach成功！");
		}
	}
	//OutputDebugString(L"111111111111");*/
	TCHAR cfgPath[MAX_PATH] = {0};
	if ( GetConfigFilePath(cfgPath) )
	{
		if ( GetAutoStartFlag(cfgPath) )
		{
			TCHAR vhdPath[MAX_PATH] = {0};
			if ( GetVHDFilePath(cfgPath, vhdPath ) )
			{
				if ( GetFileAttributes(vhdPath) != 0xFFFFFFFF )
				{
					/*CVHDManager manager;
					if ( ERROR_SUCCESS == manager.Open(vhdPath) )
					{
						if ( ERROR_SUCCESS == manager.Attach() )
						{
							SetMounted(cfgPath,TRUE);
						}
						else
						{
							SetMounted(cfgPath,FALSE);
						}
					}*/
					if ( g_manager.ConnectDiskService() )
					{
						if ( g_manager.MountDisk(vhdPath))
						{
							g_manager.SetMultipleInterface();
							SetMounted(cfgPath, TRUE);
						}
					}
				}
			}
		}
	}
	int kk = 0;
	while ( g_status.dwCurrentState != SERVICE_STOPPED )
	{
		if ( g_status.dwCurrentState == SERVICE_PAUSED )
		{
			Sleep(2000);
			continue;
		}
		kk++;
		if ( kk%10 == 0)
		{
			//OutputDebugString(L"keep alive!");
		}

		
		
		Sleep(1000);
	}
	/*//OutputDebugString(L"ddddddddd");
	if( manager.Detach() == ERROR_SUCCESS )
	{
		//OutputDebugString(L"detach成功！");
	}
	else
	{
		TCHAR errorinfo[100] = {0};
		wsprintf( errorinfo, L"detach错误码：%d", GetLastError());
		//OutputDebugString(errorinfo);
	}
	manager.Close();*/
	g_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hServiceStatus, &g_status);
}

VOID WINAPI HandlerProc(DWORD fdwControl)
{
	TCHAR path[MAX_PATH] = {0};
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		g_status.dwCurrentState = SERVICE_STOPPED;
		//OutputDebugString(L"服务停止！");
		if( GetConfigFilePath(path) )
		{
			SetMounted(path, FALSE);
			g_manager.UnMountDisk();
			g_manager.ReleaseService();
		}
		SetServiceStatus(g_hServiceStatus, &g_status);
		break;
	case SERVICE_CONTROL_CONTINUE:
		g_status.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(g_hServiceStatus, &g_status);
		break;
	case SERVICE_CONTROL_PAUSE:
		g_status.dwCurrentState = SERVICE_PAUSED;
		SetServiceStatus(g_hServiceStatus, &g_status);
		break;
	}
}


int InstallService(TCHAR * pszServicePath, TCHAR * pszServiceName,TCHAR * pszDisplayName)
{
	printf("服务文件路径：%ws\n", pszServicePath );
	int nErrCode = 0;

	if(!pszServicePath)	return  ERROR_FILE_NOT_FOUND;
	if(!pszServiceName)	return ERROR_BAD_ARGUMENTS;
	if(!pszDisplayName)	return ERROR_BAD_ARGUMENTS;


	SC_HANDLE hSCM = NULL;
	hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(!hSCM)
	{
		printf("打开服务管理器失败！\n");
		return ERROR_SUCCESS;
	}

	SC_HANDLE hService = ::CreateService(hSCM, pszServiceName,pszDisplayName,
		SC_MANAGER_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
		pszServicePath,
		NULL,
		NULL, 
		_T(""), 
		NULL, 
		NULL);

	if (hService == NULL)
	{
		nErrCode = GetLastError();
		::CloseServiceHandle(hSCM);

		TCHAR Prompt[128] = {0};

		printf("创建服务失败！Error is:%d\n", nErrCode );
		return ERROR_INVALID_HANDLE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	printf("安装服务成功！\n");
	return ERROR_SUCCESS;
}

// 卸载服务
int  UninstallService(TCHAR * pszServiceName)
{
	SERVICE_STATUS status;

	if(!pszServiceName)
		return ERROR_FILE_NOT_FOUND;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hSCM)
	{
		printf("打开服务管理器失败！\n");
		return ERROR_INVALID_HANDLE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, pszServiceName, SERVICE_STOP|DELETE);

	if (!hService)
	{
		printf("打开服务失败！\n");
		::CloseServiceHandle(hSCM);
		return ERROR_INVALID_HANDLE;
	}

	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	if(!DeleteService(hService))
	{
		printf("删除服务失败！错误码：%d", GetLastError());
		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return ERROR_ACCESS_DENIED;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	printf("删除服务成功！");
	return ERROR_SUCCESS;
}

//卸载驱动程序  
BOOL UnloadNTDriver( TCHAR * szSvrName )  
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄
	SERVICE_STATUS SvrSta;
	//打开SCM管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( hServiceMgr == NULL )  
	{
		//带开SCM管理器失败
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{
		//带开SCM管理器失败成功
		printf( "OpenSCManager() ok ! \n" );  
	}
	//打开驱动所对应的服务
	hServiceDDK = OpenService( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS );  

	if( hServiceDDK == NULL )  
	{
		//打开驱动所对应的服务失败
		printf( "OpenService() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{  
		printf( "OpenService() ok ! \n" );  
	}  
	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。  
	if( !ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )  
	{  
		printf( "ControlService() Faild %d !\n", GetLastError() );  
	}  
	else  
	{
		//打开驱动所对应的失败
		printf( "ControlService() ok !\n" );  
	}  
	//动态卸载驱动程序。  
	if( !DeleteService( hServiceDDK ) )  
	{
		//卸载失败
		printf( "DeleteSrevice() Faild %d !\n", GetLastError() );  
	}  
	else  
	{  
		//卸载成功
		printf( "DelServer:eleteSrevice() ok !\n" );  
	}  
	bRet = TRUE;
BeforeLeave:
	//离开前关闭打开的句柄
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;	
} 

