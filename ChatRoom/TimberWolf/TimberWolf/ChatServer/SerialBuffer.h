#pragma once
#include "stdafx.h"
#include "windows.h"


#define GROW_SIZE  (int)1024

class CSerialBuffer_T
{
public:
	CSerialBuffer_T(void);
	~CSerialBuffer_T(void);
	

	 BOOL WriteData(const char *pData, int length );
	 BOOL ReadData( char *pData, int length );
	 void Empty();
	 void WriteToFile( LPCTSTR fileName );
	 BOOL IsHaveEntirePacket();
	 int FirstPacketLength();

	 char *GetBuffer();
	 int Size();
	 
	 CSerialBuffer_T &operator<<( int nData );
	 CSerialBuffer_T &operator>>( int &nData );

	 CSerialBuffer_T &operator<<( short sData );
	 CSerialBuffer_T &operator>>( short &sData );

	 CSerialBuffer_T &operator<<( char nChar );
	 CSerialBuffer_T &operator>>( char &nChar );

	 CSerialBuffer_T &operator<<( float fData );
	 CSerialBuffer_T &operator>>( float &fData );

	 CSerialBuffer_T &operator<<( double dbData );
	 CSerialBuffer_T &operator>>( double &dbData );

private:
	int  dataSize_;
	int bufSize_;
	char *buf_;
	CSerialBuffer_T &st_;
public:
	SOCKADDR_IN m_sockAddr;
};
