#pragma once
#include "SocketTask_XG.h"
#define LISTEN_PORT (u_short)21122

class CChatServer_XG
{
private:
	fd_set m_allSockets[4];
	HANDLE m_hWorkThread[4];
	HANDLE m_hListenThread;
	SOCKET m_listenSock;
	CSocketTask_XG m_sockTask;
	HANDLE m_hEvent;
	void RemoveErrorSocket( fd_set *pErrorSet, int gpIndex );
	int GetLeastIndex();
	
public:
	CChatServer_XG(void);
	~CChatServer_XG(void);
	TCHAR m_lastError[128];
	void SetLastErrorStr( const TCHAR *errMsg);
	const TCHAR* GetLastErrorStr();
	static DWORD WINAPI thRecvProc( LPVOID lParam);
	static DWORD WINAPI thListenProc( LPVOID lParam );
	static DWORD WINAPI thSendProc( LPVOID lParam );
	BOOL StartServer();
	void CloseServer();
};
