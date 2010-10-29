#pragma once
#include <map>
#include <queue>
#include "winsock2.h"
#include "SerialBuffer.h"
#include "SR_Buf.h"
using namespace std;

typedef map<SOCKET, CSerialBuffer_T*> SOCK_CONTAINER;

typedef struct _SINGLE_TASK
{
	SOCKET ClientSocket;
	SOCKADDR_IN SockAddr;
	SR_Buf Data;
}SINGLE_TASK, *PSINGLE_TASK;

typedef queue<SINGLE_TASK*> TASK_QUEUE;

class CSocketTask_XG
{
public:
	CSocketTask_XG(void);
	~CSocketTask_XG(void);
private:
	SOCK_CONTAINER m_dataContainer;
	void Lock(int i = 0);
	void Unlock(int i = 0);
	void CheckBuffer( SOCKET sock);

public:
	TASK_QUEUE m_taskQueue;
	BOOL AddSocket( SOCKET sock, SOCKADDR_IN *pSockAddr);
	CSerialBuffer_T* GetSocketBuf(SOCKET sock);
	BOOL RemoveSocket( SOCKET sock);
	void RemoveAll();
	
	SINGLE_TASK *PopFirstTask();
	BOOL SaveData( SOCKET sock, char *data, int len );
	CRITICAL_SECTION m_sec[4];
};
