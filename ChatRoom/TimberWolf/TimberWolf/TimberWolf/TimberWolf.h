// TimberWolf.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "..\..\..\..\include\global.h"

// CTimberWolfApp:
// �йش����ʵ�֣������ TimberWolf.cpp
//

class CTimberWolfApp : public CWinApp
{
public:
	CTimberWolfApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTimberWolfApp theApp;