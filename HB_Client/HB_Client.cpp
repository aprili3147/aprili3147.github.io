
// HB_Client.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "HB_Client.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHBClientApp

BEGIN_MESSAGE_MAP(CHBClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHBClientApp 생성

CHBClientApp::CHBClientApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CHBClientApp 개체입니다.

CHBClientApp theApp;


// CHBClientApp 초기화

BOOL CHBClientApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CDlgMain dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

