// GetPassDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "GetPassDlg.h"
#include "md5.h"
#include "Sms4c.h"


// CGetPassDlg �Ի���

IMPLEMENT_DYNAMIC(CGetPassDlg, CDialog)

CGetPassDlg::CGetPassDlg(BOOL bEncrypt,CWnd* pParent /*=NULL*/)
	: CDialog(CGetPassDlg::IDD, pParent)
{
	m_bEncrypt = bEncrypt;
}

CGetPassDlg::~CGetPassDlg()
{
}

void CGetPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editPassword1);
	DDX_Control(pDX, IDC_EDIT1, m_editPassword2);
}


BEGIN_MESSAGE_MAP(CGetPassDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGetPassDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGetPassDlg ��Ϣ�������

BOOL EncryptFile( CString filePath, byte *key, int len)
{
	CFile file;
	file.Open( filePath, CFile::modeRead | CFile::modeWrite );
	byte *pData = new byte[len];

	file.Read( pData, len );
	theApp.WriteFileData( L"data.temp", pData, len );
	CSms4 sms4;
	sms4.SetKey(key,16);
	sms4.Encrypt( pData, len );
	file.SeekToBegin();
	file.Write(pData, len);
	file.Close();
	::DeleteFile( L"data.temp");
	delete []pData;
	return TRUE;
}

BOOL DecryptFile( CString filePath, byte *key, int len)
{
	CFile file;
	file.Open( filePath, CFile::modeRead | CFile::modeWrite );
	byte *pData = new byte[len];

	file.Read( pData, len );
	theApp.WriteFileData( L"data.temp", pData, len );
	CSms4 sms4;
	sms4.SetKey(key,16);
	sms4.Decrypt( pData, len );
	file.SeekToBegin();
	file.Write(pData, len);
	file.Close();
	::DeleteFile( L"data.temp");
	delete []pData;
	return TRUE;
}

void CGetPassDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if ( m_encryptKey )
	{
		CString pass1,pass2;
		m_editPassword1.GetWindowText(pass1);
		m_editPassword2.GetWindowText(pass2);
		if ( pass1.GetLength() < 6 )
		{
			MessageBox(L"���볤�ȱ������6!");
			return;
		}
		if ( pass1 != pass2 )
		{
			MessageBox(L"������������벻һ�£�");
			return;
		}

		theApp.GetFirstMd5FromString(pass1, m_encryptKey);
		theApp.GetSecondMd5FromString(pass1, m_secKey);
	}
	else
	{
		CString pass1;
		m_editPassword1.GetWindowText(pass1);
		if ( pass1.IsEmpty() )
		{
			MessageBox(L"�������");
			return;
		}
		theApp.GetFirstMd5FromString(pass1, m_encryptKey);
		theApp.GetSecondMd5FromString(pass1, m_secKey);
	}
	OnOK();
}

BOOL CGetPassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if ( !m_bEncrypt )
	{
		m_editPassword2.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
