// Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CClientApp:
// �йش����ʵ�֣������ Client.cpp
//
#include "ProcessBridge.h"
class CClientApp : public CWinApp
{
public:
	CClientApp();

// ��д
	public:
	virtual BOOL InitInstance();
	CPipeClient m_pipeClient;

	void CreateKeyFile(byte *pData, int len);
	void WriteFileData( CString strFile, byte *data, int len);
	int ReadFileData(CString strFile, byte *data);
	void GetFirstMd5FromString(CString str, byte *pOutMd5 );
	void GetSecondMd5FromString(CString str, byte *pOutMd5 );

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;