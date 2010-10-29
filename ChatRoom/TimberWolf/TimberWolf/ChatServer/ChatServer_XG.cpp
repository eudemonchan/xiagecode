#include "StdAfx.h"
#include "ChatServer_XG.h"
CChatServer_XG *g_pServer = NULL;

CChatServer_XG::CChatServer_XG(void)
{
	g_pServer = this;
	m_hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
}

CChatServer_XG::~CChatServer_XG(void)
{
}
void CChatServer_XG::SetLastErrorStr(const TCHAR *errMsg)
{
	ZeroMemory( m_lastError, sizeof(TCHAR)*MAX_PATH );
	_tcscpy( m_lastError, errMsg );
}

const TCHAR* CChatServer_XG::GetLastErrorStr()
{
	return (const TCHAR*)m_lastError;
}

BOOL CChatServer_XG::StartServer()
{
	for ( int i = 0; i < 4; i++ )
	{
		FD_ZERO(&m_allSockets[i]);
	}

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		SetLastErrorStr(L"WSAStartup失败！");
		return FALSE;
	}


	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 )
	{
		SetLastErrorStr(L"套接字库2.2初始化失败！");
		WSACleanup();
		return FALSE;
	}

	m_listenSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_listenSock == INVALID_SOCKET )
	{
		SetLastErrorStr(L"创建监听套接字失败！");
		WSACleanup();
		return FALSE;
	}

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(LISTEN_PORT);
	sockaddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	if( bind(m_listenSock,(SOCKADDR*)&sockaddr,sizeof(SOCKADDR)) != 0 )
	{
		SetLastErrorStr(L"绑定监听套接字失败！");
		closesocket(m_listenSock);
		m_listenSock = NULL;
		WSACleanup();
		return FALSE;
	}

	if ( listen( m_listenSock, 50) != 0 )
	{
		SetLastErrorStr(L"Listen失败！");
		closesocket(m_listenSock);
		m_listenSock = NULL;
		WSACleanup();
		return FALSE;
	}

	ZeroMemory(m_hWorkThread, sizeof(HANDLE)*4 );

	for ( int i = 0; i < 4; i++ )
	{
		m_hWorkThread[i] = ::CreateThread( NULL, 0, thRecvProc, (LPVOID)i, 0, NULL );
		if ( m_hWorkThread[i] == NULL )
		{
			for ( int j = 0; j < 4; j++ )
			{
				if ( m_hWorkThread[j] != NULL )
				{
					TerminateThread(m_hWorkThread[j], 0);
					m_hWorkThread[j] = NULL;
				}
			}

			SetLastErrorStr(L"创建工作线程失败！");
			closesocket(m_listenSock);
			m_listenSock = NULL;
			WSACleanup();
			return FALSE;
		}
	}

	m_hListenThread = ::CreateThread( NULL, 0, thListenProc, NULL, 0, NULL );
	if ( m_hListenThread == NULL )
	{
		SetLastErrorStr(L"创建监听线程失败！");
		for ( int j = 0; j < 4; j++ )
		{
			if ( m_hWorkThread[j] != NULL )
			{
				TerminateThread(m_hWorkThread[j], 0);
				m_hWorkThread[j] = NULL;
			}
		}
		closesocket(m_listenSock);
		m_listenSock = NULL;
		WSACleanup();

		return FALSE;
	}
	return TRUE;
}


DWORD WINAPI CChatServer_XG::thRecvProc( LPVOID lParam)
{
	int index = (int)lParam;
	fd_set setRead;
	fd_set setError;
	timeval time;
	time.tv_sec = 3;
	time.tv_usec = 0;
	char recvBuf[2048] = {0};
	fd_set *pSockSet =  &(g_pServer->m_allSockets[index]);
	while (1)
	{
		if ( pSockSet->fd_count != 0 )
		{
			memcpy( &setRead, pSockSet, sizeof(fd_set) );
			memcpy( &setError, pSockSet, sizeof(fd_set) );
			int nRet = select( 0, &setRead, NULL, &setError, &time );
			if ( nRet == SOCKET_ERROR )
			{
				g_pServer->SetLastErrorStr(L"select函数错误！");
				break;
			}
			if ( nRet > 0 )
			{
				printf("select have detected %d socket.\n", nRet );
				printf("To read count is:%d\n", setRead.fd_count );
				printf("Error count is:%d\n", setError.fd_count );
				for ( int i = 0; i < setRead.fd_count; i++ )
				{
					int nRecvCnt = recv( setRead.fd_array[i], recvBuf, 2048, 0);
					if ( nRecvCnt > 0 )
					{
						g_pServer->m_sockTask.SaveData( setRead.fd_array[i], recvBuf, nRecvCnt );//保存每个socket的数据
						SetEvent(g_pServer->m_hEvent);//发出信号，让处理线程立即处理
					}
				}

				if ( setError.fd_count > 0 )
				{
					for ( int i = 0; i < setError.fd_count; i++ )
					{
						g_pServer->m_sockTask.RemoveSocket(setError.fd_array[i]);
					}
					g_pServer->RemoveErrorSocket( &setError, index);
				}
			}
		}
		else
		{
			printf("there is no clients!\n");
			Sleep(3000);
		}
	}
	return 1;
}
 DWORD WINAPI CChatServer_XG::thListenProc( LPVOID lParam )
 {
	 SOCKADDR_IN addrAccept;
	 int len = sizeof(SOCKADDR_IN);
	 while (1)
	 {
		 SOCKET sockClient = accept( g_pServer->m_listenSock, (SOCKADDR*)&addrAccept, &len );
		 if ( sockClient == INVALID_SOCKET )
		 {
			 continue;
		 }
		 u_long arg = 1;
		 ioctlsocket( sockClient, FIONBIO, &arg );
		 int least = g_pServer->GetLeastIndex();
		 FD_SET( sockClient, &(g_pServer->m_allSockets[least]));//添加到集合里
		 g_pServer->m_sockTask.AddSocket(sockClient, &addrAccept);//分配数据栈
	 }
 }

 DWORD WINAPI CChatServer_XG::thSendProc( LPVOID lParam )
 {
	 while(1)
	 {
		 SINGLE_TASK *pSingleTask = g_pServer->m_sockTask.PopFirstTask();
		 if ( pSingleTask == NULL )
		 {
			 ResetEvent(g_pServer->m_hEvent);
			 WaitForSingleObject(g_pServer->m_hEvent, 500);//无数据则等待500ms再找，防止CPU占用过高
			 continue;
		 }
	 }
 }

void CChatServer_XG::CloseServer()
{

}
void CChatServer_XG::RemoveErrorSocket( fd_set *pErrorSet, int gpIndex )
{
	for ( int i = 0; i < pErrorSet->fd_count; i++ )
	{
		FD_CLR( pErrorSet->fd_array[i], &m_allSockets[gpIndex]);
	}
}
int CChatServer_XG::GetLeastIndex()
{
	for ( int i = 1; i < 4; i++ )
	{
		if ( m_allSockets[i].fd_count < m_allSockets[i-1].fd_count )
		{
			return i;
		}
	}
	return 0;
}
