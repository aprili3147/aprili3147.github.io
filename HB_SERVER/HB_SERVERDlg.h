
// HB_SERVERDlg.h: 헤더 파일
//

#pragma once
#include "ServerSocket.h"
#include "DlgTab1.h"
#include "DlgTab2.h"


// CHBSERVERDlg 대화 상자
class CHBSERVERDlg : public CDialogEx
{
// 생성입니다.
public:
	CHBSERVERDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HB_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private:
	ServerSocket m_pServerSocket;
	ClientSocket m_pClientSocket;
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	CTabCtrl m_tab;
	afx_msg void OnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult);
	CDlgTab1* main_dlg;
	CDlgTab2* db_dlg;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnDestroy();
};
