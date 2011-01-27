// VHDServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VHDManager.h"
#include "ProcessBridge.h"
#include "SerialBuffer.h"

using namespace std;

VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv );
VOID WINAPI HandlerProc( DWORD fdwControl );
SERVICE_STATUS_HANDLE g_hServiceStatus;
SERVICE_STATUS g_status;

HINSTANCE g_hInst;								// current instance
int InstallService(TCHAR * pszServicePath, TCHAR * pszServiceName,TCHAR * pszDisplayName);
int UninstallService(TCHAR * pszServiceName);
//ж����������  
BOOL UnloadNTDriver( TCHAR * szSvrName );
TCHAR szServiceName[] = L"XiageSendIPService";
TCHAR szDisplayName[] = L"IpSend";


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
	// ����Ƿ��ǰ�װ����ж��
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
	byte buf[512];
	CPipeServer *pServer = (CPipeServer*)lparam;
	CVHDManager manager;
	while(1)
	{
		ZeroMemory( buf, 128);
		int nRes = pServer->ReadData( buf, 128 );
		if ( nRes == 1 )
		{
			//���ճɹ�
			int len = *((int*)buf);
			CSerialBuffer_T serBuffer;
			serBuffer.WriteData((char*)buf + 4, len);
			char cmd;
			serBuffer>>cmd;
			switch(cmd)
			{
				
			}
			//���
		}
		else if ( nRes == 0 )
		{
			//δ�յ�����
			Sleep(100);
		}
		else
		{
			break;
		}
	}
	return 0;
}
void WINAPI Notify( int code, unsigned int param1)
{
	if ( code == 1 )
	{
		::CreateThread( NULL, 0, RecvProc, (LPVOID)param1, 0, NULL );
	}
}

VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv )
{
	g_hServiceStatus = RegisterServiceCtrlHandler(szServiceName, HandlerProc);

	OutputDebugString(L"����ServiceMain");
	if(g_hServiceStatus == NULL)
	{
		return;
	}
	CPipeServer pipeServer;
	if ( !pipeServer.StartServer(L"VHD_Service_123456", Notify))
	{
		OutputDebugString(L"�����ܵ���������ʧ�ܣ�");
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
		OutputDebugString(L"open�ɹ���");
		if(manager.Attach()==ERROR_SUCCESS)
		{
			OutputDebugString(L"attach�ɹ���");
		}
	}
	OutputDebugString(L"111111111111");*/
	while ( g_status.dwCurrentState != SERVICE_STOPPED )
	{
		if ( g_status.dwCurrentState == SERVICE_PAUSED )
		{
			Sleep(2000);
			continue;
		}
		OutputDebugString(L"keep alive!");
		Sleep(1000);
	}
	/*OutputDebugString(L"ddddddddd");
	if( manager.Detach() == ERROR_SUCCESS )
	{
		OutputDebugString(L"detach�ɹ���");
	}
	else
	{
		TCHAR errorinfo[100] = {0};
		wsprintf( errorinfo, L"detach�����룺%d", GetLastError());
		OutputDebugString(errorinfo);
	}
	manager.Close();*/
	g_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hServiceStatus, &g_status);
}

VOID WINAPI HandlerProc(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		g_status.dwCurrentState = SERVICE_STOPPED;
		OutputDebugString(L"����ֹͣ��");
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
	printf("�����ļ�·����%ws\n", pszServicePath );
	int nErrCode = 0;

	if(!pszServicePath)	return  ERROR_FILE_NOT_FOUND;
	if(!pszServiceName)	return ERROR_BAD_ARGUMENTS;
	if(!pszDisplayName)	return ERROR_BAD_ARGUMENTS;


	SC_HANDLE hSCM = NULL;
	hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(!hSCM)
	{
		printf("�򿪷��������ʧ�ܣ�\n");
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

		printf("��������ʧ�ܣ�Error is:%d\n", nErrCode );
		return ERROR_INVALID_HANDLE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	printf("��װ����ɹ���\n");
	return ERROR_SUCCESS;
}

// ж�ط���
int  UninstallService(TCHAR * pszServiceName)
{
	SERVICE_STATUS status;

	if(!pszServiceName)
		return ERROR_FILE_NOT_FOUND;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hSCM)
	{
		printf("�򿪷��������ʧ�ܣ�\n");
		return ERROR_INVALID_HANDLE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, pszServiceName, SERVICE_STOP|DELETE);

	if (!hService)
	{
		printf("�򿪷���ʧ�ܣ�\n");
		::CloseServiceHandle(hSCM);
		return ERROR_INVALID_HANDLE;
	}

	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	if(!DeleteService(hService))
	{
		printf("ɾ������ʧ�ܣ������룺%d", GetLastError());
		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return ERROR_ACCESS_DENIED;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	printf("ɾ������ɹ���");
	return ERROR_SUCCESS;
}

//ж����������  
BOOL UnloadNTDriver( TCHAR * szSvrName )  
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;//SCM�������ľ��
	SC_HANDLE hServiceDDK=NULL;//NT��������ķ�����
	SERVICE_STATUS SvrSta;
	//��SCM������
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( hServiceMgr == NULL )  
	{
		//����SCM������ʧ��
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{
		//����SCM������ʧ�ܳɹ�
		printf( "OpenSCManager() ok ! \n" );  
	}
	//����������Ӧ�ķ���
	hServiceDDK = OpenService( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS );  

	if( hServiceDDK == NULL )  
	{
		//����������Ӧ�ķ���ʧ��
		printf( "OpenService() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{  
		printf( "OpenService() ok ! \n" );  
	}  
	//ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�  
	if( !ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )  
	{  
		printf( "ControlService() Faild %d !\n", GetLastError() );  
	}  
	else  
	{
		//����������Ӧ��ʧ��
		printf( "ControlService() ok !\n" );  
	}  
	//��̬ж����������  
	if( !DeleteService( hServiceDDK ) )  
	{
		//ж��ʧ��
		printf( "DeleteSrevice() Faild %d !\n", GetLastError() );  
	}  
	else  
	{  
		//ж�سɹ�
		printf( "DelServer:eleteSrevice() ok !\n" );  
	}  
	bRet = TRUE;
BeforeLeave:
	//�뿪ǰ�رմ򿪵ľ��
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

