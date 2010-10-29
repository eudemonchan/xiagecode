#include "StdAfx.h"
#include "SocketTask_XG.h"

CSocketTask_XG::CSocketTask_XG(void)
{
	for ( int i = 0; i < 4; i++ )
	{
		InitializeCriticalSection(&m_sec[i]);
	}
}


CSocketTask_XG::~CSocketTask_XG(void)
{
	for ( int i = 0; i < 4; i++ )
	{
		DeleteCriticalSection(&m_sec[i]);
	}
}

void CSocketTask_XG::Lock(int i)
{
	EnterCriticalSection(&m_sec[i]);
}

void CSocketTask_XG::Unlock(int i)
{
	LeaveCriticalSection(&m_sec[i]);
}

BOOL CSocketTask_XG::AddSocket( SOCKET sock, SOCKADDR_IN *pSockAddr)
{
	Lock();
	if ( m_dataContainer.find(sock) != m_dataContainer.end() )
	{
		return FALSE;
	}
	CSerialBuffer_T *pBuf = new CSerialBuffer_T();
	memcpy( (char*)&(pBuf->m_sockAddr), (char*)pSockAddr, sizeof(SOCKADDR_IN) );
	m_dataContainer.insert( SOCK_CONTAINER::value_type( sock, pBuf) );
	Unlock();
	return TRUE;
}
CSerialBuffer_T* CSocketTask_XG::GetSocketBuf(SOCKET sock)
{
	if ( m_dataContainer.find(sock) == m_dataContainer.end() )
	{
		return NULL;
	}
	return m_dataContainer[sock];
}
BOOL CSocketTask_XG::RemoveSocket( SOCKET sock)
{
	Lock();
	SOCK_CONTAINER::iterator iter1 = m_dataContainer.find(sock);
	if ( iter1 == m_dataContainer.end() )
	{
		return FALSE;
	}
	delete m_dataContainer[sock];
	m_dataContainer.erase(iter1);
	Unlock();
	return TRUE;
}
void CSocketTask_XG::RemoveAll()
{
	Lock();
	SOCK_CONTAINER::iterator iter1 = m_dataContainer.begin();
	for ( ; iter1 != m_dataContainer.end(); iter1++ )
	{
		delete (*iter1).second;//释放
	}
	m_dataContainer.clear();
	Unlock();
}



void CSocketTask_XG::CheckBuffer( SOCKET sock)
{
	CSerialBuffer_T *pBuf = GetSocketBuf(sock);
	if ( pBuf == NULL )
	{
		return;
	}
	//没有完整的数据包
	if ( !pBuf->IsHaveEntirePacket() )
	{
		return;
	}
	int packetLen = pBuf->FirstPacketLength();//获取包长
	SINGLE_TASK *pTask = new SINGLE_TASK;
	pTask->ClientSocket = sock;
	memcpy( (char*)&(pTask->SockAddr), (char*)&(pBuf->m_sockAddr), sizeof(SOCKADDR_IN ) );
	pTask->Data.AllocBuffer(packetLen);
	/*memcpy( pTask->Data.GetBuffer())*/
	pBuf->ReadData( (char*)( pTask->Data.GetBuffer()), packetLen);//拷贝数据
	m_taskQueue.push(pTask);
}
SINGLE_TASK *CSocketTask_XG::PopFirstTask()
{
	Lock(1);
	if ( m_taskQueue.size() == 0 )
	{
		return NULL;
	}
	SINGLE_TASK* pTask = m_taskQueue.front();
	m_taskQueue.pop();
	Unlock(1);
	return pTask;
}
BOOL CSocketTask_XG::SaveData( SOCKET sock, char *data, int len )
{
	Lock(1);
	CSerialBuffer_T *pBuf = GetSocketBuf(sock);
	if ( pBuf == NULL )
	{
		return FALSE;
	}
	pBuf->WriteData(data,len);
	CheckBuffer(sock);
	Unlock(1);
	return TRUE;
}


