
// OpenCVwithMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OpenCVwithMFC.h"
#include "OpenCVwithMFCDlg.h"
#include "afxdialogex.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenCVwithMFCDlg 대화 상자



COpenCVwithMFCDlg::COpenCVwithMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVWITHMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCVwithMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MATCH_EDGE, m_matchMethod);
}

BEGIN_MESSAGE_MAP(COpenCVwithMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &COpenCVwithMFCDlg::OnBnClickedBtnUpload)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_IMGUPLOAD, &COpenCVwithMFCDlg::OnBnClickedBtnImgupload)
	ON_BN_CLICKED(IDC_BTN_MATCH, &COpenCVwithMFCDlg::OnBnClickedBtnMatch)
END_MESSAGE_MAP()


// COpenCVwithMFCDlg 메시지 처리기

BOOL COpenCVwithMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COpenCVwithMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COpenCVwithMFCDlg::OnPaint()
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
HCURSOR COpenCVwithMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//템플릿 업로드 함수
void COpenCVwithMFCDlg::OnBnClickedBtnUpload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		CT2CA pszString(path);
		string strPath(pszString);

		temple = imread(strPath, IMREAD_UNCHANGED);

		CreateBitmapInfo(temple.cols, temple.rows, temple.channels() * 8);
		DrawImageTemplate();
	}
	else
	{
		AfxMessageBox(_T("템플릿 로드 실패"));
		return;
	}
}

//이미지 업로드 함수
void COpenCVwithMFCDlg::OnBnClickedBtnImgupload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		CT2CA pszString(path);
		string strPath(pszString);

		image = imread(strPath, IMREAD_UNCHANGED);

		CreateBitmapInfo(image.cols, image.rows, image.channels() * 8);
		DrawImageIMG();
	}
	else
	{
		AfxMessageBox(_T("이미지 로드 실패"));
		return;
	}
}

//비트맵 함수
void COpenCVwithMFCDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapinfo != NULL)
	{
		delete m_pBitmapinfo;
		m_pBitmapinfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapinfo->bmiHeader.biPlanes = 1;
	m_pBitmapinfo->bmiHeader.biBitCount = bpp;
	m_pBitmapinfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapinfo->bmiHeader.biSizeImage = 0;
	m_pBitmapinfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapinfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapinfo->bmiHeader.biClrUsed = 0;
	m_pBitmapinfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapinfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapinfo->bmiHeader.biWidth = w;
	m_pBitmapinfo->bmiHeader.biHeight = -h;

}

//템플릿 매칭 실행 함수
void COpenCVwithMFCDlg::OnBnClickedBtnMatch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (image.empty()) {
		AfxMessageBox(_T("입력 이미지 업로드 실패"));
		return;
	}

	UpdateData(TRUE);  // 라디오 버튼 값 갱신

	switch (m_matchMethod)
	{
		case 0:
			AfxMessageBox(_T("matchEdge 방식 선택됨"));
			matchEdge(image, temple);;
			break;
		case 1:
			AfxMessageBox(_T("matchTemplateByEdgeSimple 방식 선택됨"));
			matchEdgeSimple(image, temple);
			break;
		case 2:
			AfxMessageBox(_T("matchTemplateByContour 방식 선택됨"));
			matchContours(image, temple);
			break;
		case 3:
			AfxMessageBox(_T("matchTemplateByOrb 방식 선택됨"));
			matchOrb(image, temple);
		case 4:
			AfxMessageBox(_T("ObjectDetectionByyolov8n"));
			objectdetectionByyolon8(image);
			break;
		default:
			AfxMessageBox(_T("선택된 방식 없음"));
			break;
	}
	/*
	//그레이스케일 적용
	Mat image_gray, temple_gray;
	cvtColor(image, image_gray, COLOR_BGR2GRAY);
	cvtColor(temple, temple_gray, COLOR_BGR2GRAY);

	// 히스토그램 평활화 적용
	//equalizeHist(image_gray, image_gray);
	//equalizeHist(temple_gray, temple_gray);

	//템플릿 이미지 축소
	//resize(temple_gray, temple_gray, Size(), 0.8, 0.8, INTER_LINEAR);

	//가우시안블러 적용
	Mat image_blurr, temple_blurr;
	GaussianBlur(image_gray, image_blurr, Size(5, 5), 0);
	GaussianBlur(temple_gray, temple_blurr, Size(5, 5), 0);

	//템플릿 매칭 시작
	matchTemplate(image_gray, temple_gray, result, TM_CCOEFF_NORMED);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	//매칭 위치 찾기
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	//TM_CCOEFF_NORMED는 최대값 사용
	matchLoc = maxLoc;

	Mat matchedRegion = image(Rect(matchLoc, Size(temple.cols, temple.rows)));

	//캐니 연산자로 엣지 추출
	Mat image_edge;
	Canny(matchedRegion, image_edge, 50, 150);

	//컨투어 찾기
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image_edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat image_result;
	image.copyTo(image_result);

	// BGR 이미지가 아니라면 변환
	if (image_result.channels() == 1) {
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);
	}

	//컨투어를 결과 이미지 위에 그리기
	drawContours(image_result, contours, -1, Scalar(255, 255, 0), 2, LINE_8, hierarchy, 0, matchLoc);

	DrawImageRESULT(image_result);
	DrawImageRESULTSORT(image_result, matchLoc, temple.size());
	*/
}

//Match_Edge
void COpenCVwithMFCDlg::matchEdge(Mat image, Mat temple)
{
	// 그레이스케일 적용
	Mat image_gray, temple_gray;
	if (image.channels() != 1)
		cvtColor(image, image_gray, COLOR_BGR2GRAY);
	else
		image_gray = image;

	if (temple.channels() != 1)
		cvtColor(temple, temple_gray, COLOR_BGR2GRAY);
	else
		temple_gray = temple;

	//가우시안블러 적용
	Mat image_blurr, temple_blurr;
	GaussianBlur(image_gray, image_blurr, Size(5, 5), 0);
	GaussianBlur(temple_gray, temple_blurr, Size(5, 5), 0);
}

//Match_Edge_Simple
/*void COpenCVwithMFCDlg::matchEdgeSimple(Mat image, Mat temple)
{


	TickMeter tm;
	tm.start();
	// 그레이스케일 적용
	Mat image_gray, temple_gray;
	if (image.channels() != 1)
		cvtColor(image, image_gray, COLOR_BGR2GRAY);
	else
		image_gray = image;

	if (temple.channels() != 1)
		cvtColor(temple, temple_gray, COLOR_BGR2GRAY);
	else
		temple_gray = temple;

	//가우시안블러 적용
	Mat image_blurr, temple_blurr;
	GaussianBlur(image_gray, image_blurr, Size(5, 5), 0);
	GaussianBlur(temple_gray, temple_blurr, Size(5, 5), 0);

	//Sobel 연산자로 엣지 추출
	Mat image_dx, image_dy, temple_dx, temple_dy;
	Sobel(temple_blurr, temple_dx, CV_32F, 1, 0, 3);
	Sobel(temple_blurr, temple_dy, CV_32F, 0, 1, 3);
	Sobel(image_blurr, image_dx, CV_32F, 1, 0, 3);
	Sobel(image_blurr, image_dy, CV_32F, 0, 1, 3);

	//템플릿 매칭 시작
	Mat result_x, result_y;
	matchTemplate(image_dx, temple_dx, result_x, cv::TM_CCORR_NORMED);
	matchTemplate(image_dy, temple_dy, result_y, cv::TM_CCORR_NORMED);
	result = (result_x + result_y) * 0.5;

	//매칭 위치 추출
	double maxVal;
	Point maxLoc;
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);

	//매칭된 영역 자르기
	Rect roi(maxLoc.x, maxLoc.y, temple.cols, temple.rows);

	Mat matchedRegion = image_gray(roi);

	//Sobel로 엣지 → magnitude → threshold
	Mat sobel_x, sobel_y, magnitude_mat, binary_edge;
	Sobel(matchedRegion, sobel_x, CV_32F, 1, 0, 3);
	Sobel(matchedRegion, sobel_y, CV_32F, 0, 1, 3);

	magnitude(sobel_x, sobel_y, magnitude_mat);
	magnitude_mat.convertTo(magnitude_mat, CV_8U);
	threshold(magnitude_mat, binary_edge, 50, 255, THRESH_BINARY);

	//컨투어 찾기
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary_edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	tm.stop();  // 시간 측정 종료
	double elapsed = tm.getTimeMilli();  // 밀리초 단위
	CString str;
	str.Format(_T("Contour Matching 실행 시간: %.2f ms"), elapsed);
	MessageBox(str, _T("실행 시간"));

	//결과 이미지 복사 및 색 변환
	Mat image_result;
	image.copyTo(image_result);
	if (image_result.channels() == 1)
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);

	//컨투어 그리기
	drawContours(image_result, contours, -1, Scalar(255, 255, 0), 2, LINE_8, hierarchy, 0, maxLoc);

	//시각화 함수 호출
	DrawImageRESULT(image_result);
	DrawImageRESULTSORT(image_result, maxLoc, temple.size());
}
*/

void COpenCVwithMFCDlg::matchEdgeSimple(Mat image, Mat temple)
{
	TickMeter tm;
	tm.start();

	// 1. Grayscale 변환
	Mat image_gray, temple_gray;
	if (image.channels() != 1)
		cvtColor(image, image_gray, COLOR_BGR2GRAY);
	else
		image_gray = image;

	if (temple.channels() != 1)
		cvtColor(temple, temple_gray, COLOR_BGR2GRAY);
	else
		temple_gray = temple;

	// 2. 이미지 블러 및 Gradient 계산
	Mat image_blurr;
	GaussianBlur(image_gray, image_blurr, Size(5, 5), 0);
	Mat image_dx, image_dy;
	Sobel(image_blurr, image_dx, CV_32F, 1, 0, 3);
	Sobel(image_blurr, image_dy, CV_32F, 0, 1, 3);

	// 결과 저장용 변수
	double bestScore = -1.0;
	Point bestLoc;
	int bestAngle = 0;
	double bestScale = 1.0;
	Size bestSize;

	// === 크기 및 회전 탐색 ===
	for (double scale = 0.5; scale <= 1.5; scale += 0.1) {
		Mat scaledTemple;
		resize(temple, scaledTemple, Size(), scale, scale, INTER_LINEAR);
		if (scaledTemple.cols > image.cols || scaledTemple.rows > image.rows)
			continue;

		for (int angle = -30; angle <= 30; angle += 5) {
			// 회전
			Point2f center(scaledTemple.cols / 2.0f, scaledTemple.rows / 2.0f);
			Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
			Mat rotatedTemple;
			warpAffine(scaledTemple, rotatedTemple, rotMat, scaledTemple.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

			// grayscale & gradient
			Mat grayTemple;
			if (rotatedTemple.channels() != 1)
				cvtColor(rotatedTemple, grayTemple, COLOR_BGR2GRAY);
			else
				grayTemple = rotatedTemple;

			GaussianBlur(grayTemple, grayTemple, Size(5, 5), 0);
			Mat t_dx, t_dy;
			Sobel(grayTemple, t_dx, CV_32F, 1, 0, 3);
			Sobel(grayTemple, t_dy, CV_32F, 0, 1, 3);

			// template matching
			Mat result_x, result_y, result;
			matchTemplate(image_dx, t_dx, result_x, TM_CCORR_NORMED);
			matchTemplate(image_dy, t_dy, result_y, TM_CCORR_NORMED);
			result = 0.5 * (result_x + result_y);

			double maxVal;
			Point maxLoc;
			minMaxLoc(result, nullptr, &maxVal, nullptr, &maxLoc);

			if (maxVal > bestScore) {
				bestScore = maxVal;
				bestLoc = maxLoc;
				bestAngle = angle;
				bestScale = scale;
				bestSize = rotatedTemple.size();
			}
		}
	}

	tm.stop();
	double elapsed = tm.getTimeMilli();
	CString str;
	str.Format(_T("Template Matching 실행 시간: %.2f ms"), elapsed);
	MessageBox(str, _T("실행 시간"));

	// === 결과 시각화 ===
	Mat image_result;
	image.copyTo(image_result);
	if (image_result.channels() == 1)
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);

	// 인식된 위치에 사각형 표시
	Rect matchRect(bestLoc, bestSize);
	rectangle(image_result, matchRect, Scalar(0, 255, 0), 2);

	// 시각화
	DrawImageRESULT(image_result);
	DrawImageRESULTSORT(image_result, bestLoc, bestSize);
}


//Match_Contours
void COpenCVwithMFCDlg::matchContours(Mat image, Mat temple)
{
	TickMeter tm;
	tm.start();
	// 그레이스케일 적용
	Mat image_gray, temple_gray;
	if (image.channels() != 1)
		cvtColor(image, image_gray, COLOR_BGR2GRAY);
	else
		image_gray = image;

	if (temple.channels() != 1)
		cvtColor(temple, temple_gray, COLOR_BGR2GRAY);
	else
		temple_gray = temple;

	// 가우시안 블러 적용
	Mat image_blurr, temple_blurr;
	GaussianBlur(image_gray, image_blurr, Size(5, 5), 0);
	GaussianBlur(temple_gray, temple_blurr, Size(5, 5), 0);

	// 이진화
	Mat temple_threshold, image_threshold;
	threshold(temple_blurr, temple_threshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
	threshold(image_blurr, image_threshold, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// 컨투어 검출
	vector<vector<Point>> temple_contours, image_contours;
	vector<Vec4i> hierarchy1, hierarchy2;
	findContours(temple_threshold, temple_contours, hierarchy1, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	findContours(image_threshold, image_contours, hierarchy2, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// Contour 매칭
	double minDist = 1;
	int index = -1;
	for (int i = 0; i < image_contours.size(); i++) {
		for (int j = 0; j < temple_contours.size(); j++) {
			double dist = cv::matchShapes(temple_contours[j], image_contours[i], CONTOURS_MATCH_I3, 0);
			if (dist < minDist) {
				index = i;
				minDist = dist;
			}
		}
	}

	// 중심 좌표 계산 (matchLoc 대체용)
	Moments image_moment = moments(image_contours[index]);
	Point2d matchCenter = (image_moment.m00 != 0)
		? Point2d(image_moment.m10 / image_moment.m00, image_moment.m01 / image_moment.m00)
		: Point2d(0, 0);

	tm.stop();  // 시간 측정 종료
	double elapsed = tm.getTimeMilli();  // 밀리초 단위
	CString str;
	str.Format(_T("Contour Matching 실행 시간: %.2f ms"), elapsed);
	MessageBox(str, _T("실행 시간"));

	// 결과 이미지 그리기
	Mat image_result;
	image.copyTo(image_result);
	if (image_result.channels() == 1) {
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);
	}
	drawContours(image_result, image_contours, index, Scalar(0, 255, 0), 2, LINE_8, hierarchy2, 0);

	// 중심 좌표 전달 (matchLoc → matchCenter)
	DrawImageRESULT(image_result);
	DrawImageRESULTSORT_CONTOURS(image_result, Point(matchCenter.x, matchCenter.y), temple.size());
}

//Match_Orb
void COpenCVwithMFCDlg::matchOrb(Mat image, Mat temple)
{
	// 그레이스케일 적용
	Mat image_gray, temple_gray;
	if (image.channels() != 1)
		cvtColor(image, image_gray, COLOR_BGR2GRAY);
	else
		image_gray = image;
	if (temple.channels() != 1)
		cvtColor(temple, temple_gray, COLOR_BGR2GRAY);
	else
		temple_gray = temple;

	// 가우시안블러 적용 (옵션)
	bool useGaussianBlur = false; // 필요시 true로 설정
	Mat image_processed = image_gray;
	Mat temple_processed = temple_gray;

	if (useGaussianBlur) {
		GaussianBlur(image_gray, image_processed, Size(5, 5), 0);
		GaussianBlur(temple_gray, temple_processed, Size(5, 5), 0);
	}

	// ORB 객체 생성 (클래스 멤버 변수로 이동 고려)
	Ptr<ORB> orb = ORB::create();

	// 특징점 및 디스크립터
	vector<KeyPoint> keypoints_temple, keypoints_image;
	Mat descriptors_temple, descriptors_image;

	orb->detectAndCompute(temple_processed, Mat(), keypoints_temple, descriptors_temple);
	orb->detectAndCompute(image_processed, Mat(), keypoints_image, descriptors_image);

	// 키포인트 및 디스크립터 검사
	if (keypoints_temple.empty() || keypoints_image.empty()) {
		AfxMessageBox(_T("특징점을 찾을 수 없습니다."));
		return;
	}

	if (descriptors_temple.empty() || descriptors_image.empty()) {
		AfxMessageBox(_T("특징점 디스크립터가 비어 있습니다."));
		return;
	}

	// 매칭기 생성 (Hamming 거리, crossCheck 켜기)
	BFMatcher matcher(NORM_HAMMING, true);
	vector<DMatch> matches;
	matcher.match(descriptors_temple, descriptors_image, matches);

	if (matches.size() < 4) { // 호모그래피에는 최소 4개 필요
		AfxMessageBox(_T("매칭된 점이 너무 적습니다."));
		return;
	}

	// 좋은 매칭 선택: 거리 기반 방식
	const float distance_threshold = 50.0f; // 조정 가능한 임계값
	vector<DMatch> good_matches;

	for (const auto& m : matches) {
		if (m.distance < distance_threshold) {
			good_matches.push_back(m);
		}
	}

	// 충분한 매칭이 없으면 원래 방식 사용
	if (good_matches.size() < 4) {
		// 거리 기준 정렬
		sort(matches.begin(), matches.end(), [](const DMatch& a, const DMatch& b) {
			return a.distance < b.distance;
			});

		// 상위 30% 매칭 선택
		int numGoodMatches = max(4, (int)(matches.size() * 0.3));
		good_matches.assign(matches.begin(), matches.begin() + numGoodMatches);
	}

	// 호모그래피 계산을 위한 점 추출
	vector<Point2f> obj_points, scene_points;
	for (const auto& match : good_matches) {
		obj_points.push_back(keypoints_temple[match.queryIdx].pt);
		scene_points.push_back(keypoints_image[match.trainIdx].pt);
	}

	// RANSAC을 사용한 호모그래피 계산
	Mat H;
	vector<char> inliers;
	if (obj_points.size() >= 4 && scene_points.size() >= 4) {
		H = findHomography(obj_points, scene_points, RANSAC, 3.0, inliers);
	}

	// 유효한 호모그래피 확인
	if (H.empty()) {
		// 기존 방식으로 대체: 매칭된 점들의 경계 상자 이용
		vector<Point2f> matchedPoints;
		for (const auto& m : good_matches) {
			matchedPoints.push_back(keypoints_image[m.trainIdx].pt);
		}

		if (matchedPoints.empty()) {
			AfxMessageBox(_T("좋은 매칭점이 없습니다."));
			return;
		}

		Rect boundingBox = boundingRect(matchedPoints);
		if (boundingBox.width <= 0 || boundingBox.height <= 0 ||
			boundingBox.x < 0 || boundingBox.y < 0 ||
			boundingBox.x + boundingBox.width > image_gray.cols ||
			boundingBox.y + boundingBox.height > image_gray.rows) {
			AfxMessageBox(_T("bounding box 오류"));
			return;
		}

		Mat roi = image_gray(boundingBox);
		Mat roi_thresh;
		threshold(roi, roi_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

		vector<vector<Point>> image_contours;
		vector<Vec4i> hierarchy2;
		findContours(roi_thresh, image_contours, hierarchy2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		if (image_contours.empty()) {
			AfxMessageBox(_T("컨투어를 찾을 수 없습니다."));
			return;
		}

		int index = -1;
		double maxArea = 0;
		for (int i = 0; i < image_contours.size(); ++i) {
			double area = contourArea(image_contours[i]);
			if (area > maxArea) {
				maxArea = area;
				index = i;
			}
		}

		if (index == -1) {
			AfxMessageBox(_T("유효한 컨투어가 없습니다."));
			return;
		}

		Moments mu = moments(image_contours[index]);
		Point2d matchCenter;
		if (mu.m00 != 0)
			matchCenter = Point2d(mu.m10 / mu.m00 + boundingBox.x, mu.m01 / mu.m00 + boundingBox.y);
		else
			matchCenter = Point2d(0, 0);

		// 좌표 전체 이미지 기준으로 옮기기
		for (auto& pt : image_contours[index]) {
			pt.x += boundingBox.x;
			pt.y += boundingBox.y;
		}

		Mat image_result;
		image.copyTo(image_result);
		if (image_result.channels() == 1) {
			cvtColor(image_result, image_result, COLOR_GRAY2BGR);
		}

		drawContours(image_result, image_contours, index, Scalar(0, 255, 0), 2, LINE_8, hierarchy2, 0);

		// 결과 출력
		DrawImageRESULT(image_result);
		DrawImageRESULTSORT_CONTOURS(image_result, Point(matchCenter.x, matchCenter.y), temple.size());
	}
	else {
		// 호모그래피를 사용한 템플릿 영역 표시
		Mat image_result;
		image.copyTo(image_result);
		if (image_result.channels() == 1) {
			cvtColor(image_result, image_result, COLOR_GRAY2BGR);
		}

		// 템플릿 모서리 계산
		vector<Point2f> obj_corners(4);
		obj_corners[0] = Point2f(0, 0);
		obj_corners[1] = Point2f((float)temple_gray.cols, 0);
		obj_corners[2] = Point2f((float)temple_gray.cols, (float)temple_gray.rows);
		obj_corners[3] = Point2f(0, (float)temple_gray.rows);

		// 호모그래피로 변환
		vector<Point2f> scene_corners(4);
		perspectiveTransform(obj_corners, scene_corners, H);

		// 변환된 사각형 그리기
		line(image_result, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 2);
		line(image_result, scene_corners[1], scene_corners[2], Scalar(0, 255, 0), 2);
		line(image_result, scene_corners[2], scene_corners[3], Scalar(0, 255, 0), 2);
		line(image_result, scene_corners[3], scene_corners[0], Scalar(0, 255, 0), 2);

		// 중심점 계산
		Point2d matchCenter = (scene_corners[0] + scene_corners[1] + scene_corners[2] + scene_corners[3]) * 0.25;

		// 매칭 시각화 (옵션)
		Mat img_matches;
		drawMatches(temple, keypoints_temple, image, keypoints_image, good_matches, img_matches,
			Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		// 결과 출력
		DrawImageRESULT(image_result);
		DrawImageRESULTSORT_CONTOURS(image_result, Point(matchCenter.x, matchCenter.y), temple.size());
	}
}

//ObjectDetectionByyolon8
void COpenCVwithMFCDlg::objectdetectionByyolon8(Mat image)
{
	// YOLOv8 ONNX 모델 경로
	string modelPath = "best.onnx";

	// 네트워크 로드
	dnn::Net net = dnn::readNetFromONNX(modelPath);

	// 입력 크기 (YOLOv8은 보통 640x640을 사용)
	Size modelShape = Size(640, 640);

	// blob 생성
	Mat blob;
	dnn::blobFromImage(image, blob, 1.0 / 255.0, modelShape, Scalar(), true, false);
	net.setInput(blob);

	// 모델 추론
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	// 출력 처리
	int rows = outputs[0].size[1];
	int dimensions = outputs[0].size[2];
	bool yolov8 = false;

	if (dimensions > rows)
	{
		yolov8 = true;
		rows = outputs[0].size[2];
		dimensions = outputs[0].size[1];
		outputs[0] = outputs[0].reshape(1, dimensions);
		transpose(outputs[0], outputs[0]);
	}

	float* data = (float*)outputs[0].data;

	// 감지 결과 변수들
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;

	// 클래스 이름
	vector<string> classes = { "target", "other_left", "other_rigth"};

	float modelScoreThreshold = 0.2f;
	float modelNMSThreshold = 0.45f;

	float x_factor = (float)image.cols / (float)modelShape.width;
	float y_factor = (float)image.rows / (float)modelShape.height;

	for (int i = 0; i < rows; ++i)
	{
		float* row = data + i * dimensions;

		float x = row[0];
		float y = row[1];
		float w = row[2];
		float h = row[3];

		float left = (x - 0.5f * w) * x_factor;
		float top = (y - 0.5f * h) * y_factor;
		float width = w * x_factor;
		float height = h * y_factor;

		float* scores = row + 4;
		Mat scoresMat(1, classes.size(), CV_32FC1, scores);
		Point class_id_point;
		double max_class_score;
		minMaxLoc(scoresMat, 0, &max_class_score, 0, &class_id_point);

		if (max_class_score > modelScoreThreshold)
		{
			class_ids.push_back(class_id_point.x);
			confidences.push_back((float)max_class_score);
			boxes.emplace_back((int)left, (int)top, (int)width, (int)height);
		}
	}

	// NMS (비최대 억제)
	vector<int> nms_result;
	dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

	// 시각화
	for (int i : nms_result)
	{
		Rect box = boxes[i];
		rectangle(image, box, Scalar(0, 255, 0), 2);

		string label = format("%s: %.2f", classes[class_ids[i]].c_str(), confidences[i]);
		int baseLine = 0;
		Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		int top = max(box.y, labelSize.height);

		rectangle(image, Point(box.x, top - labelSize.height),
			Point(box.x + labelSize.width, top + baseLine), Scalar::all(255), FILLED);
		putText(image, label, Point(box.x, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(), 1);
	}

	// 결과 이미지 출력
	DrawImageRESULT(image);
	imwrite("detection_result.jpg", image);
}

//템플릿 이미지 출력
void COpenCVwithMFCDlg::DrawImageTemplate()
{
	CClientDC dc(GetDlgItem(IDC_PICTURE_TEMPLATE));

	CRect rect;
	GetDlgItem(IDC_PICTURE_TEMPLATE)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, temple.cols, temple.rows, temple.data, m_pBitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

//입력 이미지 출력
void COpenCVwithMFCDlg::DrawImageIMG()
{
	CClientDC dc(GetDlgItem(IDC_PICTURE_IMAGE));

	CRect rect;
	GetDlgItem(IDC_PICTURE_IMAGE)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, image.cols, image.rows, image.data, m_pBitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

//템플릿 매칭 결과 이미지 출력
void COpenCVwithMFCDlg::DrawImageRESULT(Mat image_result)
{
	CClientDC dc(GetDlgItem(IDC_PICTURE_RESULT));

	CRect rect;
	GetDlgItem(IDC_PICTURE_RESULT)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, image_result.cols, image_result.rows, image_result.data, m_pBitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

//결과 이미지 중앙으로 정렬
void COpenCVwithMFCDlg::DrawImageRESULTSORT(Mat image_result, Point matchLoc, Size templateSize)
{
	CClientDC dc(GetDlgItem(IDC_PICTURE_RESULT_SORT));

	CRect rect;
	GetDlgItem(IDC_PICTURE_RESULT_SORT)->GetClientRect(&rect);

	// PICTURE CONTROL 크기
	int picWidth = rect.Width();
	int picHeight = rect.Height();

	// 이미지 크기
	int imgWidth = image_result.cols;
	int imgHeight = image_result.rows;

	// 매칭된 영역의 중심 좌표 계산
	Point matchCenter(matchLoc.x + templateSize.width / 2, matchLoc.y + templateSize.height / 2);

	// 결과 이미지 크기 설정 (원본 이미지 크기와 동일하게)
	Size resultSize(imgWidth, imgHeight);

	// 중앙에 맞춰진 결과 이미지 생성
	Mat image_result_sort = Mat::zeros(resultSize, image_result.type());

	// 이미지가 중앙에 오도록 이동할 오프셋 계산
	int offsetX = (resultSize.width / 2) - matchCenter.x;
	int offsetY = (resultSize.height / 2) - matchCenter.y;

	// 이동 변환 행렬 생성
	Mat translationMat = (Mat_<double>(2, 3) << 1, 0, offsetX, 0, 1, offsetY);

	// 이동 변환 적용
	warpAffine(image_result, image_result_sort, translationMat, resultSize);
	
	// 배경을 검은색으로 채우기
	FillRect(dc.GetSafeHdc(), &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, picWidth, picHeight, 0, 0, imgWidth, imgHeight, image_result_sort.data, m_pBitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

void COpenCVwithMFCDlg::DrawImageRESULTSORT_CONTOURS(Mat image_result, Point2d matchCenter, Size templateSize)
{
	CClientDC dc(GetDlgItem(IDC_PICTURE_RESULT_SORT));

	CRect rect;
	GetDlgItem(IDC_PICTURE_RESULT_SORT)->GetClientRect(&rect);

	// PICTURE CONTROL 크기
	int picWidth = rect.Width();
	int picHeight = rect.Height();

	// 이미지 크기
	int imgWidth = image_result.cols;
	int imgHeight = image_result.rows;

	// 중심 좌표 오차 줄이기 위해 double 사용
	Point2d imageCenter(imgWidth / 2.0, imgHeight / 2.0);

	// 이동할 거리 계산 (소수점 포함)
	double offsetX = imageCenter.x - matchCenter.x;
	double offsetY = imageCenter.y - matchCenter.y;

	// 변환 행렬 생성
	Mat translationMat = (Mat_<double>(2, 3) << 1, 0, offsetX, 0, 1, offsetY);

	// 이동 후 잘리는 걸 막기 위해 더 큰 캔버스에 저장
	Mat image_result_sort;
	warpAffine(image_result, image_result_sort, translationMat, image_result.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));  // 검정 배경

	// 배경을 검은색으로 초기화
	FillRect(dc.GetSafeHdc(), &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, picWidth, picHeight, 0, 0, image_result_sort.cols, image_result_sort.rows,
		image_result_sort.data, m_pBitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

void COpenCVwithMFCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}