#include "StdAfx.h"
#include "ProcessBridge.h"


CPipeBase::CPipeBase(void)
{
	m_bUserConnected = FALSE;
	m_Pipe = NULL;
	m_bServer = FALSE;
}
CPipeBase::~CPipeBase(void)
{
}

//-1代表失败，应该关闭连接，0代表无数据，1代表成功，有数据
int CPipeBase::ReadData( byte *pBuf, int dataLen )
{
	if ( !m_bUserConnected )
	{
		return -1;
	}
	//byte buf[PIPE_BUFFER_SIZE];
	OVERLAPPED ol = {0};
	HANDLE hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	ol.hEvent = hEvent;
	DWORD totalRead = 0;
	DWORD dwSingleRead = 0;
	DWORD bufSize = PIPE_BUFFER_SIZE;
	while( totalRead < dataLen )
	{
		if ( dataLen - totalRead > PIPE_BUFFER_SIZE )
		{
			bufSize = PIPE_BUFFER_SIZE;
		}
		else
		{
			bufSize = dataLen - totalRead;
		}
		BOOL bRes = ReadFile( m_Pipe, pBuf+totalRead, bufSize, NULL, &ol );
		if ( !bRes )
		{
			DWORD errCode = GetLastError();
			if ( ERROR_BROKEN_PIPE == errCode )
			{
				CloseHandle(hEvent);
				return -1;
			}
		/*	else if ( ERROR_NO_DATA == errCode )
			{
				CloseHandle(hEvent);
				return 0;
			}*/
		}
		
		WaitForSingleObject( ol.hEvent, INFINITE );
		
		if( !GetOverlappedResult( m_Pipe, &ol, &dwSingleRead, FALSE ) )
		{
			DWORD res = GetLastError();
			if ( res == ERROR_IO_INCOMPLETE )
			{
				CloseHandle(hEvent);
				return 0;
			}
			else if ( res == ERROR_BROKEN_PIPE )
			{
				CloseHandle(hEvent);
				return -1;
			}
			CloseHandle(hEvent);
			return -1;
		}
		else
		{
			if ( dwSingleRead <= 0 )
			{
				CloseHandle(hEvent);
				return 0;
			}
		}
		totalRead += dwSingleRead;
	}
	CloseHandle(hEvent);
	return 1;
}
//-1为对方管道已经关闭,0代表对方未接收数据,1代表发送成功
int CPipeBase::WriteData( const byte *pData, int dataLen )
{
	if ( !m_bUserConnected )
	{
		return FALSE;
	}
	DWORD totalWrite = 0;
	DWORD dwSingleWrite = 0;
	DWORD bufSize = PIPE_BUFFER_SIZE;
	OVERLAPPED ol = {0};
	HANDLE hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	ol.hEvent = hEvent;
	while( totalWrite < dataLen )
	{
		if ( dataLen - totalWrite > PIPE_BUFFER_SIZE )
		{
			bufSize = PIPE_BUFFER_SIZE;
		}
		else
		{
			bufSize = dataLen - totalWrite;
		}
		BOOL bRes = WriteFile( m_Pipe, pData+totalWrite, bufSize, NULL, &ol );
		if ( !bRes )
		{
			if ( ERROR_BROKEN_PIPE == GetLastError() )
			{
				CloseHandle(hEvent);
				return -1;
			}
		}
		WaitForSingleObject( ol.hEvent, 1000 );
		if ( !GetOverlappedResult( m_Pipe, &ol, &dwSingleWrite, FALSE) )
		{
			DWORD nRet = GetLastError();
			CloseHandle(hEvent);
			return -1;
		}
		else
		{
			if ( dwSingleWrite <= 0 )
			{
				CloseHandle(hEvent);
				return 0;
			}
		}
		totalWrite += dwSingleWrite;
	}
	CloseHandle(hEvent);
	return 1;
}

//CPipeServer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CPipeServer *g_pPipeServer = NULL;

CPipeServer::CPipeServer(void)
{
	g_pPipeServer = this;
	m_thread = NULL;
	m_bPipeIdle = FALSE;
	m_bServerRunning = FALSE;
}

CPipeServer::~CPipeServer(void)
{
}

DWORD WINAPI CPipeServer::thWaitConnect( LPVOID lpParameter )
{
	HANDLE l_hPipe = (HANDLE)lpParameter;
	BOOL bRes;
	OVERLAPPED ol = {0};
	HANDLE hEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	if ( hEvent == NULL )
	{
		return 0;
	}
	ol.hEvent = hEvent;
	while (g_pPipeServer->m_bServerRunning)
	{
		bRes = ConnectNamedPipe( l_hPipe, &ol );
		//WaitForSingleObject(ol.hEvent, INFINITE);
		if ( !bRes )
		{
			int errCode = GetLastError();
			if ( errCode == ERROR_NO_DATA )
			{
				DisconnectNamedPipe( l_hPipe );
				g_pPipeServer->m_bUserConnected = FALSE;
			}
			else if ( errCode == ERROR_PIPE_CONNECTED )
			{
				if ( g_pPipeServer->m_pCallBack != NULL && !(g_pPipeServer->m_bUserConnected) )
				{
					g_pPipeServer->m_bUserConnected = TRUE;
					g_pPipeServer->m_pCallBack(1);
				}
			}
			else if ( errCode == ERROR_PIPE_LISTENING )
			{
				g_pPipeServer->m_bUserConnected = FALSE;
			}
			/*else if ( errCode == ERROR_IO_PENDING )
			{
				WaitForSingleObject(ol.hEvent, INFINITE);
			}*/
		}
		Sleep(200);
	}
	CloseHandle(hEvent);
	g_pPipeServer->m_thread = NULL;
	return 0;
}



BOOL CPipeServer::StartServer(const  TCHAR *pPipeName, P_NOTIFY_CALLBACK callbackFun )
{
	m_bServer = TRUE;
	if ( m_Pipe != NULL || m_bServerRunning )
	{
		return FALSE;
	}
	m_pCallBack = callbackFun;
	HANDLE SvrHandle;
	TCHAR pipenamestr[MAX_PATH] = {0};
	_tcscpy( pipenamestr, _T("\\\\.\\pipe\\") );
	_tcscat( pipenamestr, pPipeName );
	SvrHandle = CreateNamedPipe( pipenamestr, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_NOWAIT | PIPE_TYPE_BYTE , 1, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, 1000, NULL );
	if ( SvrHandle == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	else
	{
		m_Pipe = SvrHandle;
	}
	if ( !::CreateThread( NULL, 0, thWaitConnect, (LPVOID)m_Pipe, 0, NULL ) )
	{
		CloseHandle(SvrHandle);
		m_Pipe = NULL;
		return FALSE;
	}
	m_bServerRunning = TRUE;
	return TRUE;
}
void CPipeServer::CloseServer()
{
	if ( m_Pipe == NULL || !m_bServerRunning )
	{
		return;
	}
	
	if ( m_bUserConnected && m_Pipe != NULL )
	{
		DisconnectNamedPipe( m_Pipe );
	}

	m_bServerRunning = FALSE;
	if ( m_thread != NULL )
	{
		WaitForSingleObject( m_thread, 50 );
		if ( m_thread != NULL )
		{
			::TerminateThread( m_thread, 0 );
			m_thread = NULL;
		}
	}
	CloseHandle( m_Pipe );
	m_Pipe = NULL;
}

//CPipeClient
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPipeClient::CPipeClient()
{

}
CPipeClient::~CPipeClient()
{

}
BOOL CPipeClient::ConnectToServer(const TCHAR *pPipeName,DWORD millSecond)
{
	if ( m_bUserConnected )
	{
		return FALSE;
	}
	TCHAR pipenamestr[MAX_PATH] = {0};
	_tcscpy( pipenamestr, _T("\\\\.\\pipe\\") );
	_tcscat( pipenamestr, pPipeName );
	//if ( WaitNamedPipe( pipenamestr, millSecond ) == FALSE )
	//{
	//	return FALSE;
	//}
	HANDLE CltHandle = NULL;
	CltHandle = CreateFile( pipenamestr, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE,NULL, 
		OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_OVERLAPPED, NULL );
	if ( CltHandle == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	m_Pipe = CltHandle;
	m_bUserConnected = TRUE;
	return TRUE;
}
void CPipeClient::Disconnect()
{
	if ( !m_bUserConnected || m_Pipe == NULL )
	{
		return;
	}
	DisconnectNamedPipe(m_Pipe);
	CloseHandle(m_Pipe);
	m_bUserConnected = FALSE;
	m_Pipe = NULL;
}