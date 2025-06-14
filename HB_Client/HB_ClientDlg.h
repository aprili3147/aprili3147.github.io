﻿
// HB_ClientDlg.h: 헤더 파일
//

#pragma once
#include "ClientSocket.h"
#include "DlgMain.h"

// CHBClientDlg 대화 상자
class CHBClientDlg : public CDialogEx
{
// 생성입니다.
private:
	ClientSocket m_client;
public:
	CHBClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HB_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private:

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnectBtn();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
