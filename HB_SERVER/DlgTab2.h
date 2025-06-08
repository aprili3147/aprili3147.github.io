#pragma once
#include "afxdialogex.h"
#include "MySQLConn.h"


// CDlgTab2 대화 상자

class CDlgTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab2)

public:
	CDlgTab2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DB_DIALOG };
#endif
private:
	CMySQLConn db;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CListCtrl m_db_List;
	LRESULT OnUpdateDb(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	CStatic m_beforePic;
	CStatic m_afterPic;
	CEdit m_logText;

	void DrawList();
	afx_msg void OnLvnItemchangedDBList(NMHDR* pNMHDR, LRESULT* pResult);
	void DisplayData(HBSData& data);
	void DisplayImage(CStatic& pic, cv::Mat& img);
	void DisplayLog(const CString& logText);
	afx_msg void OnBnClickedDeleteBtn();
};
