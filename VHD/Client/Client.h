// Client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CClientApp:
// 有关此类的实现，请参阅 Client.cpp
//
#include "ProcessBridge.h"
class CClientApp : public CWinApp
{
public:
	CClientApp();

// 重写
	public:
	virtual BOOL InitInstance();
	CPipeClient m_pipeClient;

	void CreateKeyFile(byte *pData, int len);
	void WriteFileData( CString strFile, byte *data, int len);
	int ReadFileData(CString strFile, byte *data);
	void GetFirstMd5FromString(CString str, byte *pOutMd5 );
	void GetSecondMd5FromString(CString str, byte *pOutMd5 );

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;