// testDetApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestDetAppApp:
// �йش����ʵ�֣������ testDetApp.cpp
//

class CtestDetAppApp : public CWinApp
{
public:
	CtestDetAppApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestDetAppApp theApp;