// testSelectServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

fd_set g_sockSet;
//fd_set g_sockSet;




void RemoveErrorSocket( fd_set errorSet )
{
	for ( int i = 0; i < errorSet.fd_count; i++ )
	{
		printf("remove a socket:%d\n",errorSet.fd_array[i] );
		FD_CLR( errorSet.fd_array[i], &g_sockSet);
	}
}


DWORD WINAPI WorkingProc( LPVOID lpParameter)
{
	fd_set setRead;
	fd_set setError;
	timeval time;
	time.tv_sec = 3;
	time.tv_usec = 0;
	while (1)
	{
		if ( g_sockSet.fd_count != 0 )
		{
			memcpy( &setRead, &g_sockSet, sizeof(fd_set) );
			memcpy( &setError, &g_sockSet, sizeof(fd_set) );
			int nRet = select( 0, &setRead, NULL, &setError, &time );
			if ( nRet == SOCKET_ERROR )
			{
				printf("select error!\n");
				break;
			}
			if ( nRet > 0 )
			{
				printf("select have detected %d socket.\n", nRet );
				printf("To read count is:%d\n", setRead.fd_count );
				printf("Error count is:%d\n", setError.fd_count );
				for ( int i = 0; i < setRead.fd_count; i++ )
				{
					char szText[10];
					int nRecvCnt = recv( setRead.fd_array[i], szText, 10, 0);
					if ( nRecvCnt == SOCKET_ERROR )
					{
						printf("recv  error!Socket is:%d\n", setRead.fd_array[i] );
						closesocket( setRead.fd_array[i] );
						FD_CLR( setRead.fd_array[i], &g_sockSet );
					}
					else
					{
						printf("Recv bytes:%d   the msg is:%s\n", nRecvCnt, szText );
					}
				}

				if ( setError.fd_count > 0 )
				{
					
					RemoveErrorSocket( setError );
				}
			}
			else
			{
				printf("do select and no socket is set!\n ");
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

int _tmain(int argc, _TCHAR* argv[])
{
	FD_ZERO(&g_sockSet);

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		printf("WSAStartup error!\n");
		return 1;
	}
                                       

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 )
	{
			printf("the dll is not support socket 2.2!\n");
			WSACleanup();
			return 1; 
	}

	SOCKET sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(6001);
	sockaddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	if( bind(sockListen,(SOCKADDR*)&sockaddr,sizeof(SOCKADDR)) != 0 )
	{
		printf("bind error!\n");
		return 0;
	}

	if ( listen( sockListen, 5) != 0 )
	{
		printf("listen error!\n");
		return 0;
	}

	//fd_set readfds, errorfds; 
	//FD_ZERO(&readfds);
	//FD_ZERO(&errorfds);
	HANDLE hThread = ::CreateThread( NULL, 0, WorkingProc, NULL, 0, NULL );
	if ( hThread == NULL )
	{
		printf("CreateThread error!\n");
	}


	SOCKADDR_IN addrAccept;
	int len = sizeof(SOCKADDR_IN);
	while (1)
	{
		SOCKET sockClient = accept( sockListen, (SOCKADDR*)&addrAccept, &len );
		if ( sockClient == INVALID_SOCKET )
		{
			printf("accept error!\n");
			break;
		}
		u_long arg = 1;
		ioctlsocket( sockClient, FIONBIO, &arg );
		FD_SET( sockClient, &g_sockSet);
		//FD_SET( sockClient, &g_sockSetError);
	}
	getchar();
	return 0;
}

