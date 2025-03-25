// MFCApplication1Dlg.cpp: 구현 파일
#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <mysql.h>  // MySQL 헤더 추가


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
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원입니다.
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

// CMFCApplication1Dlg 대화 상자
CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MENU1);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // List Control과 연결
    DDX_Control(pDX, IDC_LIST_HBS, m_listHBS);
}


// 데이터베이스 연결 함수
MYSQL* CMFCApplication1Dlg::ConnectToDatabase()
{
    MYSQL* conn = mysql_init(NULL);
    bool reconnect = true;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);
    if (conn == NULL) {
        AfxMessageBox(_T("❌ MySQL 초기화 실패!"));
        return NULL;
    }

    // MySQL 연결 시도
    if (!mysql_real_connect(conn, "127.0.0.1", "root", "infonet", "ABC", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("❌ MySQL 연결 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        mysql_close(conn);
        return NULL;
    }
    AfxMessageBox(_T("✅ DB 연결 성공!"), MB_ICONEXCLAMATION | MB_OK);  // 연결 성공 메시지
    mysql_close(conn);

    return conn;
}

// CMFCApplication1Dlg 메시지 처리기
BOOL CMFCApplication1Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CMenu menu;
    menu.LoadMenu(IDR_MENU1);
    SetMenu(&menu);

    // 기존 코드 유지
    ConnectToDatabase();

    // 시스템 메뉴에 "정보..." 메뉴 항목 추가
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

    // 이 대화 상자의 아이콘을 설정합니다.
    SetIcon(m_hIcon, TRUE);   // 큰 아이콘 설정
    SetIcon(m_hIcon, FALSE);  // 작은 아이콘 설정

    // List Control 초기화
    m_listHBS.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // 필요한 만큼 열 추가 (hbs 데이터베이스의 테이블 구조에 따라 수정 필요)
    m_listHBS.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
    m_listHBS.InsertColumn(1, _T("이름"), LVCFMT_LEFT, 100);
    m_listHBS.InsertColumn(2, _T("정보"), LVCFMT_LEFT, 150);
    // 필요에 따라 더 많은 열 추가 가능
    m_pictureCtrl.SubclassDlgItem(IDC_PICTURE, this);

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 버튼을 추가할 경우 아이콘을 그리려면
void CMFCApplication1Dlg::OnPaint()
{
    CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

    if (InpImg.empty()) {
        CDialogEx::OnPaint(); // 기본 그리기 호출
        return;
    }

    // OpenCV Mat → CImage 변환
    cv::Mat temp;
    cv::cvtColor(InpImg, temp, cv::COLOR_BGR2BGRA);  // OpenCV는 BGR, MFC는 BGRA 필요

    CImage img;
    img.Create(temp.cols, temp.rows, 32);
    uchar* pDest = (uchar*)img.GetBits();
    int stride = img.GetPitch();  // 한 줄의 바이트 크기

    for (int y = 0; y < temp.rows; y++) {
        memcpy(pDest + y * stride, temp.ptr(y), temp.cols * 4);
    }

    // 클라이언트 영역 크기 가져오기
    CRect rect;
    GetClientRect(&rect);

    // 이미지 중앙 정렬
    int x = (rect.Width() - img.GetWidth()) / 2;
    int y = (rect.Height() - img.GetHeight()) / 2;

    // 이미지 출력
    img.Draw(dc.m_hDC, x, y);
}
// 사용자가 최소화된 창을 끌 때 커서가 표시되도록 시스템에서 이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 조회 버튼 클릭 이벤트 처리
void CMFCApplication1Dlg::OnBnClickedButtonSelect()
{
    FetchAndDisplayHBSData();
}

// HBS 데이터 조회 및 표시 함수
void CMFCApplication1Dlg::FetchAndDisplayHBSData()
{
    // 기존 데이터 삭제
    m_listHBS.DeleteAllItems();

    // DB 연결
    MYSQL* conn = mysql_init(NULL);
    bool reconnect = true;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

    if (!mysql_real_connect(conn, "127.0.0.1", "root", "infonet", "ABC", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("❌ MySQL 연결 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        return;
    }

    // 쿼리 실행 (image_data는 제외)
    const char* query = "SELECT id, created_at FROM ABC";
    if (mysql_query(conn, query)) {
        CString errMsg;
        errMsg.Format(_T("❌ 쿼리 실행 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        mysql_close(conn);
        return;
    }

    // 결과 가져오기
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        CString errMsg;
        errMsg.Format(_T("❌ 결과 가져오기 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        mysql_close(conn);
        return;
    }


    // 기존 컬럼 삭제
    while (m_listHBS.DeleteColumn(0)) {}

    // 새 컬럼 추가
    m_listHBS.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
    m_listHBS.InsertColumn(1, _T("생성 날짜"), LVCFMT_LEFT, 150);

    // 데이터 표시
    MYSQL_ROW row;
    int nItem = 0;
    while ((row = mysql_fetch_row(result))) {
        // ID 컬럼 (NULL 처리)
        CString strID = row[0] ? CString(row[0]) : _T("NULL");
        int newItem = m_listHBS.InsertItem(nItem, strID);

        // 생성 날짜 컬럼 (NULL 처리)
        CString strCreatedAt = row[1] ? CString(row[1]) : _T("NULL");
        m_listHBS.SetItemText(newItem, 1, strCreatedAt);

        nItem++;
    }

    // 열 너비 자동 조정
    for (int i = 0; i < 2; i++) {
        m_listHBS.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }

    // 이벤트 핸들러 추가
    m_listHBS.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_listHBS.ModifyStyle(0, LVS_REPORT);
    m_listHBS.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    // 기존 이벤트 핸들러 제거 후 새 핸들러 등록
    m_listHBS.SetItemState(-1, 0, LVIS_SELECTED);
    m_listHBS.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

    // 결과 및 연결 해제
    mysql_free_result(result);
    mysql_close(conn);
}

void CMFCApplication1Dlg::OnItemClickListHBS(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int selectedRow = pNMItemActivate->iItem;
    if (selectedRow < 0) return;

    CString strID = m_listHBS.GetItemText(selectedRow, 0);
    int selectedID = _ttoi(strID); // 문자열을 정수로 변환

    LoadImageFromDB(selectedID); // 선택된 ID의 이미지 로드
}


void CMFCApplication1Dlg::LoadImageFromDB(int id)
{
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "127.0.0.1", "root", "infonet", "abc", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("❌ MySQL 연결 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        return;
    }

    // 쿼리 실행
    CString query;
    query.Format(_T("SELECT image_data FROM ABC WHERE id = %d"), id);
    if (mysql_query(conn, CT2A(query))) {
        CString errMsg;
        errMsg.Format(_T("❌ 이미지 조회 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        mysql_close(conn);
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        AfxMessageBox(_T("❌ 이미지 데이터 없음!"));
        mysql_close(conn);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    unsigned long* lengths = mysql_fetch_lengths(result);
    if (!row || !row[0] || lengths == NULL) {
        AfxMessageBox(_T("❌ 이미지 데이터가 없습니다!"));
        mysql_free_result(result);
        mysql_close(conn);
        return;
    }

    std::vector<uchar> imgData(row[0], row[0] + lengths[0]);
    InpImg = cv::imdecode(imgData, cv::IMREAD_COLOR);
    if (InpImg.empty()) {
        AfxMessageBox(_T("❌ 이미지 디코딩 실패!"));
    }
    else {
        DisplayImage();  // 이미지 표시
    }

    mysql_free_result(result);
    mysql_close(conn);
}

// 리스트 컨트롤에 대한 메시지 매핑 추가 (새로운 매핑 추가)
BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SELECT, &CMFCApplication1Dlg::OnBnClickedButtonSelect)
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
    ON_NOTIFY(NM_CLICK, IDC_LIST_HBS, &CMFCApplication1Dlg::OnItemClickListHBS)
    ON_COMMAND(ID_FILE_OPEN, &CMFCApplication1Dlg::OnFileFileopen)  // 추가
END_MESSAGE_MAP()


void CMFCApplication1Dlg::DisplayImage()
{
    if (InpImg.empty()) return;

    cv::Mat temp;
    cv::cvtColor(InpImg, temp, cv::COLOR_BGR2BGRA);

    CImage img;
    img.Create(temp.cols, temp.rows, 32);
    uchar* pDest = (uchar*)img.GetBits();
    int stride = img.GetPitch();
    for (int y = 0; y < temp.rows; y++) {
        memcpy(pDest + y * stride, temp.ptr(y), temp.cols * 4);
    }

    CRect rect;
    m_pictureCtrl.GetClientRect(&rect);

    CClientDC dc(&m_pictureCtrl);
    m_pictureCtrl.Invalidate();
    img.StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
}

void CMFCApplication1Dlg::OnFileFileopen()
{
    AfxMessageBox(_T("파일 선택 창을 실행합니다."));

    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        CString fileExt = dlg.GetFileExt();
        if (fileExt.CompareNoCase(_T("jpg")) != 0 &&
            fileExt.CompareNoCase(_T("png")) != 0 &&
            fileExt.CompareNoCase(_T("bmp")) != 0)
        {
            MessageBox(_T("지원되지 않는 파일 형식입니다. (jpg, png, bmp만 가능)"), _T("알림"), MB_OK);
            return;
        }

        selectedFilePath = CT2A(dlg.GetPathName(), CP_UTF8); // 파일 경로 저장
        InpImg = cv::imread(selectedFilePath, IMREAD_COLOR);

        if (InpImg.empty()) {
            MessageBox(_T("이미지를 불러올 수 없습니다."), _T("오류"), MB_OK | MB_ICONERROR);
            return;
        }

        DisplayImage(); // 화면에 이미지 출력
    }
}


bool CMFCApplication1Dlg::UploadImageToDB(const cv::Mat& img)
{
    if (img.empty()) {
        AfxMessageBox(_T("❌ 이미지가 비어 있습니다!"));
        return false;
    }

    // 이미지를 바이너리 데이터로 변환
    std::vector<uchar> buf;
    cv::imencode(".jpg", img, buf);
    unsigned long imgSize = buf.size();
    const uchar* imgData = buf.data();

    // MySQL 연결
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "127.0.0.1", "root", "infonet", "ABC", 3306, NULL, 0)) {
        CString errMsg;
        errMsg.Format(_T("❌ MySQL 연결 실패: %S"), mysql_error(conn));
        AfxMessageBox(errMsg);
        return false;
    }

    // 쿼리 준비
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    const char* query = "INSERT INTO ABC (image_data) VALUES (?)";
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        AfxMessageBox(_T("❌ 쿼리 준비 실패!"));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    // 바인딩 설정
    MYSQL_BIND bind = { 0 };
    bind.buffer_type = MYSQL_TYPE_BLOB;
    bind.buffer = (void*)imgData;
    bind.buffer_length = imgSize;

    if (mysql_stmt_bind_param(stmt, &bind)) {
        AfxMessageBox(_T("❌ 바인딩 실패!"));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    // 실행
    if (mysql_stmt_execute(stmt)) {
        CString errMsg;
        errMsg.Format(_T("❌ 실행 실패: %S"), mysql_stmt_error(stmt));
        AfxMessageBox(errMsg);
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
    return true;
}



void CMFCApplication1Dlg::OnBnClickedButton1()
{
    if (InpImg.empty()) {
        AfxMessageBox(_T("이미지를 먼저 불러와 주세요!"));
        return;
    }

    if (selectedFilePath.empty()) {
        AfxMessageBox(_T("파일이 정상적으로 선택되지 않았습니다."));
        return;
    }

    if (UploadImageToDB(InpImg)) {  // 파일 경로가 아니라 InpImg 사용
        AfxMessageBox(_T("✅ 이미지 업로드 성공!"));
    }
    else {
        AfxMessageBox(_T("❌ 이미지 업로드 실패!"));
    }
}