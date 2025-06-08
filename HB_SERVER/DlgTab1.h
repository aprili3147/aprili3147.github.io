#pragma once

#include "afxdialogex.h"
#include "ServerSocket.h"

#define WM_CLIENT_CONNECTED (WM_USER + 1)

// CDlgTab1 대화 상자
class CDlgTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab1)
private:
	ServerSocket m_server;
	ClientSocket m_client;
public:
	CDlgTab1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif
private:
	CString clientLogStr;
	Mat m_templateMat;
	Mat m_beforeMat;
	Mat m_afterMat;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientDisconnect(WPARAM wParam, LPARAM lParam);
	LRESULT OnReceiveSocket(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_templatePic;
	CStatic m_beforePic;
	CStatic m_afterPic;
	CComboBox m_optionCombo;
	CListBox m_list_client;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedConnectBtn();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSelectBtn();
	afx_msg void OnBnClickedSendBtn();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSaveBtn();
};
