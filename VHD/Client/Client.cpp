// Client.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp ����

CClientApp::CClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClientApp ����

CClientApp theApp;


// CClientApp ��ʼ��

BOOL CClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	if (!m_pipeClient.ConnectToServer(L"VHD_Service_123456",2000 ))
	{
		AfxMessageBox(L"���ӷ���ʧ�ܣ�");
		return FALSE;
	}

	CClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
void CClientApp::CreateKeyFile(byte *pData, int len)
{
	CFile file;
	file.Open( L"key.dat", CFile::modeCreate | CFile::modeWrite );
	file.Write( pData, len);
	file.Close();
}
void CClientApp::GetFirstMd5FromString(CString str, byte *pOutMd5 )
{
	MD5_CTX md5;
	//byte md5first[16] = {0};
	md5.MD5Data( (byte*)str.GetBuffer(), str.GetLength()*2, pOutMd5);
	
}

void CClientApp::GetSecondMd5FromString(CString str, byte *pOutMd5 )
{
	MD5_CTX md5;
	byte md5first[16] = {0};
	md5.MD5Data( (byte*)str.GetBuffer(), str.GetLength()*2, md5first);
	md5.MD5Data( md5first, 16, pOutMd5);
}
void CClientApp::WriteFileData( CString strFile, byte *data, int len)
{
	CFile file;
	file.Open( strFile, CFile::modeCreate | CFile::modeWrite );
	file.Write( data, len);
	file.Close();
}

int CClientApp::ReadFileData(CString strFile, byte *data)
{
	CFile file;
	file.Open( strFile, CFile::modeRead );
	ULONG len = file.GetLength();
	file.Read( data, len );
	file.Close();
	return len;
}