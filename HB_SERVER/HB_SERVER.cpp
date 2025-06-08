
// HB_SERVER.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "HB_SERVER.h"
#include "HB_SERVERDlg.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHBSERVERApp

BEGIN_MESSAGE_MAP(CHBSERVERApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHBSERVERApp 생성

CHBSERVERApp::CHBSERVERApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CHBSERVERApp 개체입니다.

CHBSERVERApp theApp;


// CHBSERVERApp 초기화

BOOL CHBSERVERApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CHBSERVERDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

