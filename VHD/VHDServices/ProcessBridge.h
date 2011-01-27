#pragma once
#include "windows.h"

#define PIPE_BUFFER_SIZE  512
typedef void (WINAPI *P_NOTIFY_CALLBACK) ( int,unsigned int);

class CPipeBase
{
public:
	CPipeBase(void);
	~CPipeBase(void);
	int ReadData( byte *pBuf, int dataLen );
	int WriteData(const byte *pData, int dataLen );
	BOOL m_bServer;
	HANDLE m_Pipe;
	BOOL m_bUserConnected;
};

class CPipeServer : public CPipeBase
{
public:
	CPipeServer(void);
	~CPipeServer(void);
	BOOL StartServer(const TCHAR *pPipeName, P_NOTIFY_CALLBACK callbackFun );
	void CloseServer();
	P_NOTIFY_CALLBACK m_pCallBack;
	BOOL m_bPipeIdle;
	BOOL m_bServerRunning;
private:
	
	HANDLE m_thread;
	static DWORD WINAPI thWaitConnect( LPVOID lpParameter );
};

class CPipeClient : public CPipeBase
{
public:
	CPipeClient(void);
	~CPipeClient(void);
	BOOL ConnectToServer(const TCHAR *pPipeName,DWORD millSecond);
	void Disconnect();
};
