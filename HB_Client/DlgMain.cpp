// DlgMain.cpp: 구현 파일


#include "pch.h"
#include "HB_Client.h"
#include "HB_ClientDlg.h"
#include "afxdialogex.h"

#include "CameraManager.h" // CameraManager 헤더 포함
#include "LightControlBySerial.h"

using namespace cv;

// CDlgMain 대화 상자

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)

// 메시지 매핑
BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
    ON_MESSAGE(WM_CLIENT_IMG_RECV, &CDlgMain::OnReceiveTemp)
    ON_MESSAGE(WM_SERVER_CLOSE, &CDlgMain::OnServerDisconnect)
    //ON_MESSAGE(WM_APP + 1, &CDlgMain::OnDisplayFrame)
    ON_WM_CLOSE()
    ON_WM_TIMER() // 타이머 메시지 매핑
    ON_COMMAND(ID_MATCH_CONTOURS, &CDlgMain::OnMatchContours)
    ON_COMMAND(ID_MATCH_EDGE_SIMPLE, &CDlgMain::OnMatchEdgeSimple)
    ON_COMMAND(ID_MATCH_ORB, &CDlgMain::OnMatchOrb)
    ON_COMMAND(ID_DETECT_YOLO, &CDlgMain::OnDetectYolo)
    ON_COMMAND(ID_DETECT_YOLO_TARGET, &CDlgMain::OnDetectYoloTarget)
    ON_COMMAND(ID_BEFORE_UPLOAD, &CDlgMain::OnBeforeUpload)
    ON_COMMAND(ID_MATCH_START, &CDlgMain::OnMatchStart)
    ON_COMMAND(ID_CAMERA_ON, &CDlgMain::OnCameraOnClicked)
    ON_COMMAND(ID_CAMERA_OFF, &CDlgMain::OnCameraOffClicked)
    ON_COMMAND(ID_LIGHTCONTROL_LIGHTSTART, &CDlgMain::OnLightStartClicked)
    ON_COMMAND(ID_LIGHTCONTROL_LIGHTSETTING, &CDlgMain::OnLightSettingsClicked)
    ON_COMMAND(ID_LIGHTCONTROL_LIGHTRESET, &CDlgMain::OnLightResetClicked)
    ON_BN_CLICKED(IDC_SAVE_BTN, &CDlgMain::OnBnClickedSaveBtn)
    
    ON_NOTIFY(UDN_DELTAPOS, IDC_LIGHT_SPIN, &CDlgMain::OnDeltaposLightSpin)
END_MESSAGE_MAP()


// 생성자
CDlgMain::CDlgMain(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MAIN_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 소멸자
CDlgMain::~CDlgMain()
{
}

// DDX
void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEMP_PIC, m_templatePic);
    DDX_Control(pDX, IDC_CAMERA_PIC, m_cameraPic);
    DDX_Control(pDX, IDC_AFTER_PIC, m_afterPic);
    DDX_Control(pDX, IDC_LOG_EDIT, m_logText);
    DDX_Control(pDX, IDC_CAMERA_HISTO, m_camera_histoPic);
    DDX_Control(pDX, IDC_TEMPLATE_HISTO, m_template_histoPic);

}

// 대화상자 초기화
BOOL CDlgMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);  // 큰 아이콘
    SetIcon(m_hIcon, FALSE); // 작은 아이콘

    CHBClientDlg mainDlg;
    if (mainDlg.DoModal() == IDCANCEL) {
        PostQuitMessage(0);
    }

    m_client.SetWnd(this->m_hWnd);
    m_md.SetPicCtrl(&m_afterPic);

    CEdit* m_pLogText = (CEdit*)GetDlgItem(IDC_LOG_EDIT);
    CLogger::GetInstance().DisplayLog(m_pLogText);
    CLogger::GetInstance().SetEditControl(m_pLogText);


    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
// 타이머 콜백: 카메라 영상 받아서 Picture Control에 출력,히스토그램 출력
void CDlgMain::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_cameraManager.IsOpened()) {
        Mat frame;
        if (m_cameraManager.GetFrame(frame)) {
            frame.copyTo(m_beforeMat); // 현재 프레임 저장
            m_client.DisplayImage(m_cameraPic, frame); // 카메라 프레임 표시
            DisplayHistogram(frame, m_camera_histoPic); // 해당 프레임 히스토그램 출력
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CDlgMain::OnCameraOnClicked()
{
    if (!m_cameraManager.IsOpened()) {
        if (!m_cameraManager.Open()) {
            AfxMessageBox(L"Unable to open the camera.");
            return;
        }
    }

    // 이미 타이머가 설정되어 있는 경우 중복 방지
    UINT_PTR result = SetTimer(1, 33, NULL);
    if (result != 0) {
        m_bCameraStreaming = true; //타이머 시작
        AfxMessageBox(L"Camera started!");
        CLogger::GetInstance().Log(L" camera grabbing start. ", LOG_LEVEL_INFO);
    }
    else {
        AfxMessageBox(L"timer is duplicatied");
    }
    
}
// 카메라 OFF 메뉴 클릭 시
void CDlgMain::OnCameraOffClicked()
{
    if (m_bCameraStreaming) {
        KillTimer(1); // 타이머 중지
        m_bCameraStreaming = false;
    }

    // 카메라 종료
    if (m_cameraManager.IsOpened()) {
        m_cameraManager.Close();
    }

    // 화면 비우기
    Mat blank = Mat::zeros(480, 640, CV_8UC3); // 해상도에 맞게 조정
    m_client.DisplayImage(m_cameraPic, blank);
    CLogger::GetInstance().Log(L" camera grabbing stop. ", LOG_LEVEL_INFO);
}

void CDlgMain::OnLightSettingsClicked()
{
    if (!m_cameraManager.IsOpened()) {
        if (!m_cameraManager.Open()) {
            AfxMessageBox(L"start camera first.");
            return;
        }
    }

    m_hSerial = OpenSerialPort(L"COM3");

    // 기준 히스토그램 계산
    cv::Mat grayTemplate;
    cv::cvtColor(m_tempMat, grayTemplate, cv::COLOR_BGR2GRAY);
    m_templateHist = ComputeHistogram(grayTemplate);

    //이전발기 저장
    GetCurrentBrightness(m_hSerial, m_prevBrightness);
    // 밝기 0 설정
    SetBrightness(m_hSerial, 0);
    // 히스토그램 표시
    DisplayHistogram(m_tempMat, m_template_histoPic); // 주의: m_templatePic 말고 히스토그램 컨트롤일 수 있음
}

void CDlgMain::OnLightStartClicked()
{
    if (m_templateHist.empty()) {
        AfxMessageBox(L"active LightSetting first.");
        return;
    }

    double maxSimilarity = -1.0;
    int bestBrightness = 0;
    const double dropThreshold = 0.1; // 유사도 급감 시 조기 종료 임계값


    for (int br = 0; br <= 30; br += 1) {
        /*std::wstring msg = L"current brightness: " + std::to_wstring(br);
        CLogger::GetInstance().Log(msg.c_str(), LOG_LEVEL_DEBUG);*/
        SetBrightness(m_hSerial, br);

        cv::Mat frame;
        if (!m_cameraManager.GetFrame(frame) || frame.empty())
            continue;

        DisplayImage(m_cameraPic, frame);
        DisplayHistogram(frame, m_camera_histoPic);

        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        Mat currentHist = ComputeHistogram(grayFrame);
        double sim = compareHist(m_templateHist, currentHist, HISTCMP_CORREL);
        

        // 유사도 급감 시 루프 종료
        if (maxSimilarity > 0 && sim < maxSimilarity - dropThreshold) {
            break;
        }


        if (sim > maxSimilarity) {
            maxSimilarity = sim;
            bestBrightness = br;
        }
    }
    std::wstring msg = L"bestBtightness: " + std::to_wstring(bestBrightness);
    CLogger::GetInstance().Log(msg.c_str(), LOG_LEVEL_DEBUG);
    std::wstring simMsg = L"similarity: " + std::to_wstring(maxSimilarity);
    CLogger::GetInstance().Log(simMsg.c_str());

    // 최적 밝기 설정
    SetBrightness(m_hSerial, bestBrightness);

    m_crruentBrightness = bestBrightness;
}

void CDlgMain::OnLightResetClicked()
{
    if (!m_cameraManager.IsOpened()) {
        AfxMessageBox(L"start camera first.");
        return;
    }
    SetBrightness(m_hSerial, m_prevBrightness);
}




void CDlgMain::DisplayImage(CStatic& pictureCtrl, const cv::Mat& image)
{
    if (image.empty()) return;

    // Picture Control 크기 가져오기
    CRect rect;
    pictureCtrl.GetClientRect(rect);
    int width = rect.Width();
    int height = rect.Height();

    // 이미지 크기 조정
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(width, height));

    // BGR -> RGB 변환
    cv::Mat rgbImage;
    cv::cvtColor(resizedImage, rgbImage, cv::COLOR_BGR2RGB);

    // Bitmap 생성
    CImage cimg;
    cimg.Create(rgbImage.cols, rgbImage.rows, 24);

    // 픽셀 복사
    for (int y = 0; y < rgbImage.rows; ++y) {
        BYTE* dest = (BYTE*)cimg.GetPixelAddress(0, y);
        BYTE* src = (BYTE*)rgbImage.ptr(y);
        memcpy(dest, src, rgbImage.cols * 3);
    }

    // DC 얻고 출력
    CDC* pDC = pictureCtrl.GetDC();
    cimg.Draw(pDC->GetSafeHdc(), 0, 0);
    pictureCtrl.ReleaseDC(pDC);
    cimg.Destroy();
}

// 실시간 영상에 대한 히스토그램을 계산하고 출력하는 함수
void CDlgMain::DisplayHistogram(Mat& frame, CStatic& targetPicCtrl)
{
    // 그레이스케일로 변환
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    // 히스토그램 계산
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    Mat hist;
    calcHist(&grayFrame, 1, 0, Mat(), hist, 1, &histSize, &histRange);
    normalize(hist, hist, 0, 255, NORM_MINMAX);

    // 히스토그램 이미지 생성
    int histWidth = 512;
    int histHeight = 400;
    int binWidth = cvRound((double)histWidth / histSize);
    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(255, 255, 255));

    for (int i = 1; i < histSize; i++) {
        line(histImage,
            Point(binWidth * (i - 1), histHeight - cvRound(hist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(hist.at<float>(i))),
            Scalar(0, 0, 0), 2, 8, 0);
    }

    // Picture Control 크기에 맞게 히스토그램 resize 
    CRect rect;
    targetPicCtrl.GetClientRect(&rect);
    Mat resizedHistImage;
    resize(histImage, resizedHistImage, Size(rect.Width(), rect.Height()));

    // 히스토그램을 화면에 출력
    m_client.DisplayImage(targetPicCtrl, resizedHistImage);
}





LRESULT CDlgMain::OnReceiveTemp(WPARAM wParam, LPARAM lParam)
{
    ImageData* pImgData = (ImageData*)lParam;

    Mat* pImg = pImgData->pImg;
    CString optionStr = pImgData->optionStr;

    if (pImg && !pImg->empty())
    {
        m_tempMat = *pImg;
        m_client.DisplayImage(m_templatePic, *pImg);

        m_selOptionIndex = pImgData->optionIndex;
        SetDlgItemText(IDC_OPTION_EDIT, pImgData->optionStr);

        if (m_selOptionIndex == 1) //Match_Contours
        {
            GetDlgItem(IDC_LOTATION_CHECK)->ShowWindow(SW_SHOW);
        }
    }

    delete pImg;
    return LRESULT();
}

LRESULT CDlgMain::OnServerDisconnect(WPARAM wParam, LPARAM lParam)
{
    AfxMessageBox(L"Server Disconnect!");
    OnClose();
    return 0;
}

void CDlgMain::OnClose()
{
    CWnd* homeWnd = AfxGetMainWnd();
    if (homeWnd)
    {
        homeWnd->PostMessage(WM_CLOSE);  // 전체 종료
    }

    CLogger::GetInstance().Log(L"Shutting down the client programme", LOG_LEVEL_INFO);

    DestroyWindow();
}

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
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

void CDlgMain::OnBnClickedSaveBtn()
{
    CString strLog;
    m_logText.GetWindowText(strLog);

    if (m_beforeMat.empty() && m_afterMat.empty()) {
        AfxMessageBox(L"No images to send!");
        return;
    }

    if (strLog.IsEmpty()) {
        AfxMessageBox(L"No Log to send");
        return;
    }

    m_client.SendImage(m_beforeMat, m_afterMat);
    AfxMessageBox(L"DB saving complete");
}

void CDlgMain::OnMatchContours()
{
    if (m_beforeMat.empty() || m_tempMat.empty()) {
        AfxMessageBox(L"Image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();
    Mat templateMat = m_tempMat.clone();

    bool lotation = ((CButton*)GetDlgItem(IDC_LOTATION_CHECK))->GetCheck() == BST_CHECKED;

    m_afterMat = m_md.matchContours(m_beforeMat, m_tempMat, lotation);
}

void CDlgMain::OnMatchEdgeSimple()
{
    if (m_beforeMat.empty() || m_tempMat.empty()) {
        AfxMessageBox(L"Image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();
    Mat templateMat = m_tempMat.clone();

    m_afterMat = m_md.matchEdgeSimple(m_beforeMat, m_tempMat);
}

void CDlgMain::OnMatchOrb()
{
    if (m_beforeMat.empty() || m_tempMat.empty()) {
        AfxMessageBox(L"Image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();
    Mat templateMat = m_tempMat.clone();

    m_afterMat = m_md.matchORB(m_beforeMat, m_tempMat);
}

void CDlgMain::OnDetectYolo()
{
    if (m_beforeMat.empty()) {
        AfxMessageBox(L"Image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();

    m_afterMat = m_md.matchByYOLO(m_beforeMat);
}

void CDlgMain::OnDetectYoloTarget()
{
    if (m_beforeMat.empty()) {
        AfxMessageBox(L"Image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();

    m_afterMat = m_md.matchByTarget(m_beforeMat);
}


void CDlgMain::OnBeforeUpload()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
    if (fileDlg.DoModal() == IDOK)
    {
        CString path = fileDlg.GetPathName();

        CT2CA pszString(path);
        std::string strPath(pszString);

        m_beforeMat = imread(strPath, IMREAD_COLOR);

        m_client.DisplayImage(m_cameraPic, m_beforeMat);

        CLogger::GetInstance().Log(L"Before image upload");
    }
    else
    {
        AfxMessageBox(_T("Image load fail"));
        return;
    }
}


void CDlgMain::OnMatchStart()
{
    if (m_beforeMat.empty() || m_tempMat.empty()) {
        AfxMessageBox(L"Before image not found!");
        return;
    }

    if (m_tempMat.empty() && m_selOptionIndex != 2) {
        AfxMessageBox(L"Template image not found!");
        return;
    }

    Mat beforeMat = m_beforeMat.clone();
    Mat templateMat = m_tempMat.clone();

    bool lotation = ((CButton*)GetDlgItem(IDC_LOTATION_CHECK))->GetCheck() == BST_CHECKED;

    switch (m_selOptionIndex)
    {
    case 0:
        m_afterMat = m_md.matchEdgeSimple(beforeMat, templateMat);
        break;
    case 1:
        m_afterMat = m_md.matchContours(beforeMat, templateMat, lotation);
        break;
    case 2:
        m_afterMat = m_md.matchByYOLO(beforeMat);
        break;
    case 3:
        m_afterMat = m_md.matchORB(beforeMat, templateMat);
        break;
    case 4:
        m_afterMat = m_md.matchORB(beforeMat, templateMat);
        break;
    default: break;
    }
}

void CDlgMain::OnDeltaposLightSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if (pNMUpDown->iDelta < 0)
    {
        // 위로 눌렀을 때 (증가 방향)
        AdjustLight(true);
    }
    else if (pNMUpDown->iDelta > 0)
    {
        // 아래로 눌렀을 때 (감소 방향)
        AdjustLight(false);
    }
    *pResult = 0;
}

void CDlgMain::AdjustLight(bool increase) {

    if (!m_cameraManager.IsOpened()) {
        if (!m_cameraManager.Open()) {
            AfxMessageBox(L"Turn on the Camera.");
            return;
        }
    }

    CString log, str;
    int brightness;

    if (increase && m_crruentBrightness < 30) {
        brightness = m_crruentBrightness + 1;
        str = L"increase";
    }
    else if (!increase && m_crruentBrightness > 0) {
        brightness = m_crruentBrightness - 1;
        str = L"decrease";
    }
    else {
        return;
    }
    log.Format(L"Bright [%s], Current Brightness : %d", str, brightness);
    CLogger::GetInstance().Log(log);

    SetBrightness(m_hSerial, brightness);
    m_crruentBrightness = brightness;
}