//#include "stdafx.h"

#pragma once
#ifdef _WINDOWS
#include <Windows.h>
#endif

#define EC_SUCCESS			0
#define EC_KEY_LEN_ERROR			0X00010001		// 密钥长度小于等于0
#define EC_ENCRYPTED_DATA_LEN_ERROR	0X00010002		// 被加密的数据长度小于等于0
#define EC_DECRYPTED_DATA_LEN_ERROR	0X00010003		// 被解密的数据长度小于等于0

#ifndef _WINDOWS
#define DWORD unsigned long
#define PDWORD DWORD*
#define BYTE unsigned char
#define TRUE true
#define FALSE false
#define ZeroMemory(x, y) memset(x, '\0', y)
#define _lrotl(x, y) (x<<y | x>>(32-y))
#endif

class CSms4
{
public:
	CSms4();
	virtual ~CSms4(void);

	bool SetKey(BYTE byKey[], int nLen);
	bool Encrypt(BYTE byData[], DWORD dwLen);
	bool Decrypt(BYTE byData[], DWORD dwLen);

	bool Encrypt(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen);
	bool Decrypt(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen);

	bool EncryptEx(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen, bool final);
	bool DecryptEx(const unsigned char *in, const unsigned int inlen, unsigned char **out, unsigned int *outlen, bool final);

	DWORD GetLastError(void);
	bool EncryptOrDecryptFile( const wchar_t *filePath,bool bIsEncrypt=true );
	DWORD m_fileLength;
	LONGLONG *m_pReadLength;
private:
	static BYTE m_bySbox[256];
	static DWORD m_dwFK[4];
	static DWORD m_dwCK[32];
	DWORD m_dwKey[4];
	DWORD m_dwRKey[32];
	DWORD m_dwErrorCode;

	void CalculateRKey(void);

	void EncryptOrDecrypt(BYTE byData[], DWORD dwLen, bool bEncrypt);
	void EncryptOrDecrypt(DWORD dwData[], bool bEncrypt);

// ********************试验算法的可实现性时使用，可删除*************************----begin
	void SetKey(DWORD dwKey[]);
	void Encrypt(DWORD dwData[]);
	void Decrypt(DWORD dwData[]);
// ********************试验算法的可实现性时使用，可删除*************************----end

	DWORD TransT(DWORD dwData);
	DWORD TransTEx(DWORD dwData);
	DWORD TransTau(DWORD dwData);
	DWORD TransL(DWORD dwData);
	DWORD TransLEx(DWORD dwData);

	// ********************RotateLeftMove()此方法可删除，用_lrotl()代替*************************
	DWORD RotateLeftMove(DWORD dwSrc, int nBit);
};
