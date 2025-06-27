
// OpenCVwithMFCDlg.h: 헤더 파일
//

#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"

using namespace cv;
using namespace std;

// COpenCVwithMFCDlg 대화 상자
class COpenCVwithMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	COpenCVwithMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVWITHMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


public:
	int m_matchMethod;
	Mat temple;
	Mat image;
	Mat result;
	BITMAPINFO* m_pBitmapinfo;

	void CreateBitmapInfo(int w, int h, int bpp); // Bitmap 정보를 생성하는 함수.
	void DrawImageTemplate(); // 그리는 작업을 수행하는 함수.
	void DrawImageIMG();
	void DrawImageRESULT(Mat image_result);
	void DrawImageRESULTSORT(Mat image_result, Point matchLoc, Size templateSize);
	void matchEdge(Mat image, Mat temple);
	void matchEdgeSimple(Mat image, Mat temple);
	void matchContours(Mat image, Mat temple);
	void matchOrb(Mat image, Mat temple);
	void DrawImageRESULTSORT_CONTOURS(Mat image_result, Point2d matchCenter, Size templateSize);
	void objectdetectionByyolon8(Mat image);
// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUpload();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnImgupload();
	afx_msg void OnBnClickedBtnMatch();
};
