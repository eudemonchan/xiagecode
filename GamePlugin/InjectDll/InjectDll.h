
// InjectDll.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInjectDllApp:
// �йش����ʵ�֣������ InjectDll.cpp
//

class CInjectDllApp : public CWinAppEx
{
public:
	CInjectDllApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInjectDllApp theApp;