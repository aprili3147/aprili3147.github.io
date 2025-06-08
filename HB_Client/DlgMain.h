#pragma once

#include "afxdialogex.h"
#include "ClientSocket.h"
#include "CameraManager.h"
#include "MarkDetector.h"


using namespace cv;

// CDlgMain 대화 상자

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)


private:
	HICON m_hIcon;
	ClientSocket m_client;
	CameraManager m_cameraManager;
	CMarkDetector m_md;
	Mat m_tempMat;
	Mat m_beforeMat;
	Mat m_afterMat;
	Mat m_latestFrame;
	int m_selOptionIndex;
	//Mat m_loadedImage;
	bool m_bCameraStreaming = false; //카메라 영상 스트리밍 상태 플래그
	HANDLE m_hSerial = NULL;              // 시리얼 포트 핸들
	cv::VideoCapture m_cap;               // 카메라 캡처 객체 (히스토그램 매칭용)
	cv::Mat m_templateHist;               // 템플릿 히스토그램
	int m_prevBrightness = 0;             // 밝기 초기값 저장 (복원용)
	int m_bestBrightness = 0;             // 최적 밝기값 저장
	int m_crruentBrightness;
	double m_maxSimilarity = -1.0;        // 유사도 최고값 저장


public:
	CDlgMain(CWnd* pParent = nullptr);
	virtual ~CDlgMain();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	LRESULT OnReceiveTemp(WPARAM wParam, LPARAM lParam);
	LRESULT OnServerDisconnect(WPARAM wParam, LPARAM lParam);
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	void DisplayImage(CStatic& ctrl, const Mat& img);
	void DisplayHistogram(Mat& frame, CStatic& targetPicCtrl);  // 히스토그램 표시 함수
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_templatePic;
	CStatic m_cameraPic;
	CStatic m_afterPic;
	CStatic m_camera_histoPic;     // 히스토그램 Picture Control
	CStatic m_template_histoPic;
	CEdit m_logText;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedSaveBtn();
	//pylon
	afx_msg void OnCameraOnClicked();
	afx_msg void OnCameraOffClicked();
	afx_msg void OnLightStartClicked();
	afx_msg void OnLightSettingsClicked();
	afx_msg void OnLightResetClicked();
	void AdjustLight(bool increase);
	//opencv
	afx_msg void OnMatchContours();
	afx_msg void OnMatchEdgeSimple();
	afx_msg void OnMatchOrb();
	afx_msg void OnDetectYolo();
	afx_msg void OnDetectYoloTarget();
	afx_msg void OnBeforeUpload();
	afx_msg void OnMatchStart();

	//afx_msg LRESULT OnDisplayFrame(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeltaposLightSpin(NMHDR* pNMHDR, LRESULT* pResult);
};
