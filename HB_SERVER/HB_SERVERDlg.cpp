
// HB_SERVERDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "HB_SERVER.h"
#include "HB_SERVERDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHBSERVERDlg 대화 상자
CHBSERVERDlg::CHBSERVERDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HB_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHBSERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CHBSERVERDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CHBSERVERDlg::OnSelchangeMainTab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHBSERVERDlg 메시지 처리기

BOOL CHBSERVERDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CRect rect;
	m_tab.GetWindowRect(&rect);

	m_tab.InsertItem(0, L"Main");
	m_tab.InsertItem(1, L"DB");

	//메인 TAB
	main_dlg = new CDlgTab1(this);
	main_dlg->Create(IDD_MAIN_DIALOG, &m_tab);
	main_dlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	main_dlg->ShowWindow(SW_SHOW);

	//DB TAB
	db_dlg = new CDlgTab2(this);
	db_dlg->Create(IDD_DB_DIALOG, &m_tab);
	db_dlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	db_dlg->ShowWindow(SW_HIDE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHBSERVERDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHBSERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHBSERVERDlg::OnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int select = m_tab.GetCurSel();

	switch (select) {
	case 0:
		main_dlg->ShowWindow(SW_SHOW);
		db_dlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		main_dlg->ShowWindow(SW_HIDE);
		db_dlg->ShowWindow(SW_SHOW);

		HWND hWndTab = db_dlg->GetSafeHwnd();
		m_pClientSocket.SetWnd(hWndTab);
		db_dlg->DrawList();
		break;
	}
	*pResult = 0;
}



BOOL CHBSERVERDlg::PreTranslateMessage(MSG* pMsg)
{
	//엔터 종료 막기
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			// ESC 키 이벤트에 대한 처리 추가
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			// Enter 키 이벤트에 대한 처리 추가
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CHBSERVERDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLogger::GetInstance().Log(L"Shutting down the server programme", LOG_LEVEL_INFO);
}
