#include "StdAfx.h"
#include "SerialBuffer.h"

CSerialBuffer_T::CSerialBuffer_T(void):st_(*this)
{
	buf_ = NULL;
	dataSize_ = 0;
	bufSize_ = 0;
}

CSerialBuffer_T::~CSerialBuffer_T(void)
{
	if ( bufSize_ != 0 )
	{
		delete []buf_;
	}
}

void CSerialBuffer_T::Empty()
{
	dataSize_ = 0;
}

BOOL CSerialBuffer_T::WriteData(const char *pData, int length )
{
	if ( length < 1 || pData == NULL )
	{
		return FALSE;
	}
	if ( bufSize_ != 0 )
	{
		//小于bufSize_的情况,
		if ( dataSize_ + length < bufSize_ )
		{
			memcpy( buf_+dataSize_, pData, length );//小于则直接拷贝
			dataSize_ += length;
		}
		else
		{
			bufSize_ = ( (dataSize_+length)/GROW_SIZE + 1 )*GROW_SIZE;
			if ( dataSize_ == 0 )
			{
				delete []buf_;
				buf_ = new char[bufSize_];
				memcpy( buf_, pData, length );
				dataSize_ = length;
			}
			else
			{
				//保存原来的数据
				char *tempbuf = new char[dataSize_];
				memcpy( tempbuf, buf_, dataSize_ );

				delete []buf_;
				buf_ = new char[bufSize_];
				memcpy( buf_, tempbuf, dataSize_ );
				memcpy( buf_ + dataSize_, pData, length );

				dataSize_ += length;
				delete []tempbuf;
				tempbuf = NULL;
			}
		}
	}
	else
	{
		//计算所需的bufSize
		bufSize_ = ( length/GROW_SIZE + 1 )*GROW_SIZE;
		buf_ = new char[bufSize_];
		memcpy( buf_, pData, length );
		dataSize_ += length;
	}
	return TRUE;
}

void CSerialBuffer_T::WriteToFile( LPCTSTR fileName )
{
	
}

BOOL CSerialBuffer_T::ReadData( char *pData, int length )
{
	if ( dataSize_ < length )
	{
		return FALSE;
	}

	memcpy( pData, buf_, length );
	if ( bufSize_ - ( dataSize_-length ) > GROW_SIZE )
	{
		//保存剩下数据
		char *tempbuf = new char[dataSize_-length];
		memcpy( tempbuf, buf_+length, dataSize_ - length );

		bufSize_ = ( (dataSize_-length)/GROW_SIZE + 1 )*GROW_SIZE;
		delete []buf_;
		buf_ = new char[bufSize_];
		memcpy( buf_, tempbuf, dataSize_ - length );

		dataSize_ -= length;
		delete []tempbuf;
		tempbuf = NULL;
	}
	else
	{
		dataSize_ -= length;
		memmove( buf_, buf_ + length, dataSize_ );
	}
	return TRUE;
}
CSerialBuffer_T& CSerialBuffer_T::operator<<( int nData )
{
	//保存原来的数据
	WriteData( (char*)&nData, sizeof(int) );
	return st_;
}
CSerialBuffer_T& CSerialBuffer_T::operator>>( int &nData )
{
	ReadData( (char*)&nData, sizeof(int) );
	return st_;
}

CSerialBuffer_T& CSerialBuffer_T::operator<<( short sData )
{
	WriteData( (char*)&sData, sizeof(short) );
	return st_;
}
CSerialBuffer_T& CSerialBuffer_T::operator>>( short &sData )
{
	ReadData( (char*)&sData, sizeof(short) );
	return st_;
}

//CSerialBuffer_T &CSerialBuffer_T::operator<<( vector<CString> &vecUserList )
//{
//	int nSize = (int)vecUserList.size();
//	if ( nSize == 0 )
//	{
//		return st_;
//	}
//	st_<<nSize;
//	for ( int i = 0; i < nSize; i++ )
//	{
//		st_<<vecUserList[i];
//	}
//	return st_;
//}
//CSerialBuffer_T &CSerialBuffer_T::operator>>( vector<CString> &vecUserList )
//{
//	vecUserList.clear();
//	int nSize;
//	CString strTemp;
//	st_>>nSize;
//	for ( int i = 0; i < nSize; i++ )
//	{
//		st_>>strTemp;
//		vecUserList.push_back( strTemp );
//	}
//	return st_;
//}
CSerialBuffer_T &CSerialBuffer_T::operator<<( char nChar )
{
	WriteData( (char*)&nChar, sizeof(char) );
	return st_;
}
CSerialBuffer_T &CSerialBuffer_T::operator>>( char &nChar )
{
	ReadData( (char*)&nChar, sizeof(char) );
	return st_;
}

CSerialBuffer_T &CSerialBuffer_T::operator<<( float fData )
{
	WriteData( (char*)&fData, sizeof(float) );
	return st_;
}
CSerialBuffer_T &CSerialBuffer_T::operator>>( float &fData )
{
	ReadData( (char*)&fData, sizeof(float) );
	return st_;
}

CSerialBuffer_T &CSerialBuffer_T::operator<<( double dbData )
{
	WriteData( (char*)&dbData, sizeof(double) );
	return st_;
}
CSerialBuffer_T &CSerialBuffer_T::operator>>( double &dbData )
{
	ReadData( (char*)&dbData, sizeof(double) );
	return st_;
}

char *CSerialBuffer_T::GetBuffer()
{
	return buf_;
}
int CSerialBuffer_T::Size()
{
	return dataSize_;
}
