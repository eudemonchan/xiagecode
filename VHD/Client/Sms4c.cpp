//#include "StdAfx.h"
//#include <afx.h>
#include "stdafx.h"
#include "Sms4c.h"
#include <string.h>

//#include "ErrorDef.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// S盒常量
BYTE CSms4::m_bySbox[] = {
	0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
	0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
	0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
	0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
	0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
	0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
	0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
	0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
	0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
	0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
	0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
	0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
	0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
	0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
	0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
	0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48};

// 系统参数常量
DWORD CSms4::m_dwFK[] = {0xA3B1BAC6, 0x56AA3350, 0x677D9197,0xB27022DC};

// 固定参数常量
DWORD CSms4::m_dwCK[] = {
	0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
	0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
	0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
	0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
	0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
	0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
	0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
	0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

CSms4::CSms4()
{
	m_dwErrorCode = EC_SUCCESS;
	m_fileLength = 1;
	m_pReadLength = NULL;
}

CSms4::~CSms4(void)
{
}

// 设置密钥（密钥的最大长度为16BYTE，要确保设置的密钥不超过该长度）
bool CSms4::SetKey(BYTE byKey[], int nLen)
{
	if(nLen <= 0 || nLen > 16)
	{
		m_dwErrorCode = EC_KEY_LEN_ERROR;
		return FALSE;
	}

	ZeroMemory(m_dwKey, 4*sizeof(&m_dwKey[0]));
	memcpy(m_dwKey, byKey, nLen);

	// 计算轮密钥
	CalculateRKey();

	return TRUE;
}

bool CSms4::Encrypt(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen)
{
	return EncryptEx(in, inlen, out, outlen, true);
}

bool CSms4::Decrypt(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen)
{
	return DecryptEx(in, inlen, out, outlen, true);
}

bool CSms4::EncryptEx(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen, bool final)
{
	if(inlen <= 0)
	{
	}

	if(!final && inlen % 16 != 0)
	{
		m_dwErrorCode = EC_ENCRYPTED_DATA_LEN_ERROR;
		return FALSE;
	}

	int padding;
	if(final)
		padding = 16 - inlen % 16;
	else
		padding = 0;
	*outlen = inlen + padding;
	*out = new unsigned char[*outlen];
	memcpy(*out, in, inlen);
	memset(*out + inlen, padding, padding);

	return Encrypt(*out, *outlen);
}

bool CSms4::DecryptEx(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen, bool final)
{
	if(inlen <= 0 || inlen % 16 != 0)
	{
		m_dwErrorCode = EC_DECRYPTED_DATA_LEN_ERROR;
		return FALSE;
	}

	*out = new unsigned char[inlen];
	memcpy(*out, in, inlen);
	if(!Decrypt(*out, inlen))
	{
		delete[] out;
		*outlen = 0;
		return FALSE;
	}

	int padding;
	if(final)
		padding = (*out)[inlen - 1];
	else
		padding = 0;
	*outlen = inlen - padding;
	memset(*out + *outlen, '\0', padding);

	return TRUE;
}

// 加密数据（数据的长度必须为16的整数倍，否则最后不足16的部分加密后无法解密）
bool CSms4::Encrypt(BYTE byData[], DWORD dwLen)
{
	if(dwLen <= 0)
	{
		m_dwErrorCode = EC_ENCRYPTED_DATA_LEN_ERROR;
		return FALSE;
	}

	EncryptOrDecrypt(byData, dwLen, TRUE);
	return TRUE;
}



// 解密数据（数据的长度必须为16的整数倍，否则最后不足16的部分无法解密）
bool CSms4::Decrypt(BYTE byData[], DWORD dwLen)
{
	if(dwLen <= 0)
	{
		m_dwErrorCode = EC_DECRYPTED_DATA_LEN_ERROR;
		return FALSE;
	}
	EncryptOrDecrypt(byData, dwLen, FALSE);
	return TRUE;
}

// 加解密数据（数据的长度必须为16的整数倍，否则最后不足16的部分加密后无法解密）
void CSms4::EncryptOrDecrypt(BYTE byData[], DWORD dwLen, bool bEncrypt)
{
	DWORD dwPos = 0;
	while(dwPos < dwLen)
	{
		if(dwPos+16 <= dwLen)
		{
			EncryptOrDecrypt((PDWORD)(&(byData[dwPos])), bEncrypt);
			dwPos += 16;
		}
		else
		{
			break;
			//BYTE byTempBuf[16] = {0};
			//::memcpy(byTempBuf, &(byData[dwPos]), dwLen-dwPos);
			//::memset(byTempBuf+dwLen-dwPos, 16-(dwLen-dwPos), 16-(dwLen-dwPos));
			//EncryptOrDecrypt((PDWORD)byTempBuf, bEncrypt);
			//::memcpy(&(byData[dwPos]), byTempBuf, sizeof(byTempBuf));
			//break;
		}
	}
}

// 设置密钥（密钥长度为128位，4个DWORD长度）
void CSms4::SetKey(DWORD dwKey[])
{
	memcpy(m_dwKey, dwKey, 4*sizeof(m_dwKey[0]));
	CalculateRKey();
}

// 加密数据（数据长度位为128位，4个DWORD长度）
void CSms4::Encrypt(DWORD dwData[])
{
	EncryptOrDecrypt(dwData, TRUE);
}

// 解密数据（数据长度位为128位，4个DWORD长度）
void CSms4::Decrypt(DWORD dwData[])
{
	EncryptOrDecrypt(dwData, FALSE);
}

// 加密/解密数据（数据长度位为128位，4个DWORD长度）
void CSms4::EncryptOrDecrypt(DWORD dwData[], bool bEncrypt)
{
	// 复制输入数据到dwX[]
	DWORD dwX[4] = {0};
	for(int i=0; i<4; i++)
	{
		dwX[i] = dwData[i];
	}

	// 进行T转换
	for(int i=0; i<32; i++)
	{
		dwX[i%4] ^= TransT(dwX[(i+1)%4] ^ dwX[(i+2)%4] ^ dwX[(i+3)%4] ^ (bEncrypt ? m_dwRKey[i] : m_dwRKey[31-i]));
	}

	// 返回加密/解密后的数据
	for(int i=0; i<4; i++)
	{
		dwData[i] = dwX[3-i];
	}
}

// 计算轮密钥
void CSms4::CalculateRKey(void)
{
	// 计算初始的K[0]-K[4]
	DWORD dwK[4];
	for(int i=0; i<4; i++)
	{
		dwK[i] = m_dwKey[i] ^ m_dwFK[i];
	}

	// 通过密钥扩展算法计算轮密钥
	for(int i=0; i<32; i++)
	{
		dwK[i%4] ^= TransTEx(dwK[(i+1)%4] ^ dwK[(i+2)%4] ^ dwK[(i+3)%4] ^ m_dwCK[i]);
		m_dwRKey[i] = dwK[i%4];
	}
}

// T转换
DWORD CSms4::TransT(DWORD dwData)
{
	return TransL(TransTau(dwData));
}

// T'转换（密钥扩展算法）
DWORD CSms4::TransTEx(DWORD dwData)
{
	return TransLEx(TransTau(dwData));
}

// τ转换
DWORD CSms4::TransTau(DWORD dwData)
{
	BYTE byA[4] = {0};
	BYTE byB[4] = {0};
	DWORD dwB = 0;

	for(int i=0; i<4; i++)
	{
		byA[i] = (BYTE)((dwData>>(i*8)) & 0xff);
		byB[i] = m_bySbox[byA[i]];
		dwB |= (byB[i]<<(i*8));
	}

	return dwB;
}

// L转换
DWORD CSms4::TransL(DWORD dwData)
{
	return (dwData ^ _lrotl(dwData,2) ^ _lrotl(dwData,10) ^ _lrotl(dwData,18) ^ _lrotl(dwData,24));
	//return (dwData ^ RotateLeftMove(dwData,2) ^ RotateLeftMove(dwData,10) ^ RotateLeftMove(dwData,18) ^ RotateLeftMove(dwData,24));
}

// L'转换
DWORD CSms4::TransLEx(DWORD dwData)
{
	return (dwData ^ /*RotateLeftMove*/_lrotl(dwData,13) ^ /*RotateLeftMove*/_lrotl(dwData,23));
}

// 循环左移位操作（dwSrc为要移位的数据，nBit是要移动的位数，返回值为移位后的数据）
DWORD CSms4::RotateLeftMove(DWORD dwSrc, int nBit)
{
	return (dwSrc<<nBit | dwSrc>>(32-nBit));
}
bool CSms4::EncryptOrDecryptFile( const wchar_t *filePath, bool bIsEncrypt /*= true*/ )
{
	unsigned char buf[2048];
	FILE *hFile = _wfopen( filePath, L"rb+" );
	if ( hFile == NULL )
	{
		return false;
	}
	int nBuflength = 0;
	int nTotalRead = 0;
	if ( 0 != fseek( hFile, 0, SEEK_END ) )
	{
		return false;
	}

	int fileLength = ftell(hFile);
	m_fileLength = fileLength;
	if ( 0 != fseek( hFile, 0, SEEK_SET ) )
	{
		return false;
	}
	int a = ftell(hFile);
	if ( fileLength == -1 )
	{
		return false;
	}
	while( nTotalRead < fileLength )
	{
		if ( fileLength - nTotalRead > 2048 )
		{
			nBuflength = 2048;
		}
		else
		{
			nBuflength = fileLength - nTotalRead;
		}
		int nRead = 0;
		while( nRead < nBuflength )
		{
			int nRet = fread( buf+nRead, 1, nBuflength-nRead, hFile );
			nRead += nRet;
			
			if ( ferror(hFile) != 0 )
			{
				return false;
			}
		}

		if ( bIsEncrypt )
		{
			if( !Encrypt( buf, nBuflength ) )
			{
				return false;
			}
		}
		else
		{
			if( !Decrypt( buf, nBuflength ) )
			{
				return false;
			}
		}
		

		//前移nBufLength个字节，以便写入加密数据
		if( 0 != fseek( hFile, nTotalRead, SEEK_SET ) )
		{
			return false;
		}
		int nWrite = 0;
		while( nWrite < nBuflength )
		{
			int nRet = fwrite( buf+nWrite, 1, nBuflength-nWrite, hFile );
			nWrite += nRet;
		}

		nTotalRead += nBuflength;
		(*m_pReadLength) = nTotalRead;
		if( 0 != fseek( hFile, nTotalRead, SEEK_SET ) )
		{
			return false;
		}

	}
	fclose(hFile);
	return true;
}

